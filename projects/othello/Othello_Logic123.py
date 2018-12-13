#NAME:Yangguang He  ID:50947171
from othello_option456 import *

def turn_number(b_w:str)->int:
    '''trun str-B to number 1 and turn str-W to -1'''
    if b_w=='black':
        return 1
    else:
        return -1
option=option()
ROW=option._row_number.get()
COL=option._column_number.get()
TURN=turn_number(option._who_is_first.get())
LEFT=option._top_left.get()
RULE=option._game_rule.get()
class NEW_Gameboard:
    def __init__(self):
        self.rule=RULE# > or <
        self.columns=COL
        self.rows=ROW
        self.B_W=LEFT#'black'or'white'
        self.screen=self.new_screen()
        self.turn=TURN# 1 means black   -1 means white
        self.chesses={-1:2,1:2}# to calculus the score of black and white
    def get_opposite(self):
        '''
        change the turn to the opposite one
        '''
        return -self.turn
    def turn_str(self):
        '''
        turn 1 to black and turn -1 to white
        '''
        if self.turn==1:
            return 'black'
        else:
            return 'white'
    def new_screen(self)-> '2D table, list of lists':
        '''Create and return an empty screen:  a list of rows, with each
        row a list of pixels going across that row.  All the pixels
        will be 0 (black).'''
        result=[]
        for i in range(self.rows):
            result.append([0]*self.columns)
        if self.B_W=='black':
            result[int(self.rows/2-1)][int(self.columns/2-1)]=1#1 means black
            result[int(self.rows/2-1)][int(self.columns/2)]=-1#-1 means white
            result[int(self.rows/2)][int(self.columns/2-1)]=-1
            result[int(self.rows/2)][int(self.columns/2)]=1
        if self.B_W=='white':
            result[int(self.rows/2-1)][int(self.columns/2-1)]=-1
            result[int(self.rows/2-1)][int(self.columns/2)]=1
            result[int(self.rows/2)][int(self.columns/2-1)]=1
            result[int(self.rows/2)][int(self.columns/2)]=-1
        return result

    def play_the_game(self,row:int,col:int):
        '''
        drop the chessman if it is valid and change the turn
        '''
        
        is_valid=False
        
        if row>self.rows or col>self.columns or row<=0 or col<=0: # that means a invalid move
            return False
        else:
            if self.screen[row-1][col-1]!=0:# cannot drop if there is either black or white
                return False
            else:
                for dx in range(-1,2):
                    for dy in range(-1,2):
                        if dx!=0 or dy!=0:# It includes 8 directions, for example, dx=1,dy=0 means right direction                               
                            x=col+dx
                            y=row+dy
                            count=0
                            while x>0 and y>0 and x<=self.columns and y<=self.rows:
                                count+=1
                                if self.screen[y-1][x-1]==self.get_opposite():
                                    pass
                                elif self.screen[y-1][x-1]==0:# break if neither is black or white
                                    break
                                else:
                                    if count==1:
                                        break
                                    else:
                                        is_valid=True
                                        while x!=col or y!=row:
                                            x-=dx
                                            y-=dy
                                            self.screen[y-1][x-1]=self.turn
                                            if not (x==col and y==row):# change the scores of white and black
                                                self.chesses[self.get_opposite()]-=1
                                                self.chesses[self.turn]+=1
                                    
                                        break
                                x+=dx
                                y+=dy

                if is_valid==False:
                    return False
                else:
                    self.chesses[self.turn]+=1
                    self.turn=self.get_opposite()# change the turn
                    return True
                
    def all_invalid(self):
        '''
        to find whether the next player has the valid move
        if the next player has no valid turn,the self.turn does not change and return True
        else, the self.turn changes and return False
        '''
        white=self.chesses[-1]# to store up the previous scores of white
        black=self.chesses[1]# to store up the previous scores of black
        B=self.turn# to store up the previous turn of the game
        A=self.copy_screen()# to store up the previous game screen 
        number=0
        for i in range(self.rows):
            for j in range(self.columns):
                if not self.play_the_game(i+1,j+1):
                    number+=1
        if number==self.rows*self.columns:# that means no valid move in the game for next player
            self.turn=self.get_opposite()# the next turn is still previous turn
            return True
        else:# that means the next player has valid move
            # what we do is to return to previous condition
            self.turn=B
            self.chesses[-1]=white
            self.chesses[1]=black
            self.screen=A 
            return False
        
    def print_gameboard(self):
        print('B: {:} W: {:}'.format(self.chesses[1],self.chesses[-1]))
        for row in range(len(self.screen)):
            #print the row cross
            for col in range(len(self.screen[0])):
                if self.screen[row][col]==0:
                    pixel='.'
                if self.screen[row][col]==1:
                    pixel='B'
                if self.screen[row][col]==-1:
                    pixel='W'
                print(pixel,end=' ')
            print()## Newline for the end of this row
        if self.all_invalid():
            if not self.all_invalid():
                print('TURN: {:}'.format(self.turn_str()))
        else:
            print('TURN: {:}'.format(self.turn_str()))
        return
    def get_screen(self):
        return self.screen
    def get_turn(self):
        if self.turn==1:
            return 'black'
        else:
            return 'white'
    def black_score(self):# show the scores of black
        return self.chesses[1]
    def white_score(self):#show the scores of white
        return self.chesses[-1]


                
    def copy_screen(self):
        '''
        copy the previous game screen 
        '''
        result=[]
        for i in range(self.rows):
            result.append(self.screen[i][:])
        return result
    def the_winner(self):#When the game is over, show the winner according to winning rule
         
        if self.chesses[-1]==self.chesses[1]:
            return 'NONE'
        else:
            if self.rule=='>':
                if self.chesses[-1]>self.chesses[1]:
                    return'WHITE'
                else:
                    return'BLACK'
            else:
                if self.chesses[-1]>self.chesses[1]:
                    return 'BLACK'
                else:
                    return'WHITE'



        
