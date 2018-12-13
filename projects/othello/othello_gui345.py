#NAME:Yangguang He  ID:50947171
from Othello_Logic123 import *
game=NEW_Gameboard()

class GUI():
    def __init__(self):
       
        
        self.flag=False
        self.window=tkinter.Tk()
        self.font=('Helvetica', 16)# the script
        self.rule=game.rule# < or >
        self.unit=45# the original unit is 45(both row unit and column unit)
        self.rows=game.rows# the row number that the user chooses
        self.cols=game.columns# the column number that the user chooses 
        if self.rows<=8 or self.cols<=8:# change the unit according to the situation
            self.unit=80
        self.row_unit=self.unit# the original row unit before resizing the window
        self.col_unit=self.unit# the original column unit before resizing the window

        self.left=game.B_W# top_left position
        self.width=self.unit*self.cols # original width of the window
        self.height=self.unit*self.rows# original height of the window
        self.canvas_width=self.width
        self.canvas_height=self.height
        self.canvas=tkinter.Canvas(master=self.window,
                                   width=self.width,height=self.height,
                                   background='light green')
        self.text_turn=tkinter.Label(master=self.window,text='TURN:{:}'.format(game.get_turn()),font=self.font)
        self.text_turn.grid(row=0,column=0)
        self.black_score=tkinter.Label(master=self.window,text='Black:{:}'.format(game.black_score()),font=self.font)
        self.black_score.grid(row=0,column=1)
        self.white_score=tkinter.Label(master=self.window,text='White:{:}'.format(game.white_score()),font=self.font)
        self.white_score.grid(row=1,column=1)

        self._originalboard()
        self._show_text()
        self.canvas.bind('<Button-1>', self._click)
        self.canvas.bind('<Configure>', self._resized)
 
        self.canvas.grid(row=1,column=0, padx = 10, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.E + tkinter.W)
        self.window.rowconfigure(1, weight = 1)
        self.window.columnconfigure(0, weight = 1)
        self.window.mainloop()

    def _originalboard(self):
        row=0
        col=0
        for i in range(self.rows+1):
            self.canvas.create_line(0,row,self.canvas_width,row)
        
            row+=self.row_unit
        for i in range(self.cols+1):
            self.canvas.create_line(col,0,col,self.canvas_height)
            col+=self.col_unit
        x=self.col_unit*self.cols/2
        y=self.row_unit*self.rows/2
        if self.left=='black':
            self.canvas.create_oval(x-self.col_unit,y-self.row_unit,x,y,fill='black')
            self.canvas.create_oval(x+self.col_unit,y-self.row_unit,x,y,fill='white')
            self.canvas.create_oval(x-self.col_unit,y+self.row_unit,x,y,fill='white')
            self.canvas.create_oval(x+self.col_unit,y+self.row_unit,x,y,fill='black')

        else:
            self.canvas.create_oval(x-self.col_unit,y-self.row_unit,x,y,fill='white')
            self.canvas.create_oval(x+self.col_unit,y-self.row_unit,x,y,fill='black')
            self.canvas.create_oval(x-self.col_unit,y+self.row_unit,x,y,fill='black')
            self.canvas.create_oval(x+self.col_unit,y+self.row_unit,x,y,fill='white')
    def _click(self,event: tkinter.Event):
        x= event.x
        y= event.y
        row=y//self.row_unit+1
        col=x//self.col_unit+1
        game.play_the_game(int(row),int(col))
        A=self._end()
        self._drawspot()
        self._show_text()
        if not self.flag and self._end():
            
            winner=tkinter.Label(master=self.window,text='WINNER:{:}'.format(game.the_winner()),font=self.font)
            winner.grid(row=0,column=0)
            self.text_turn.grid_forget()
            
            
            self.flag=True
        if self.flag:
            self.text_turn.grid_forget()
            
        
        
            
    def _drawspot(self):
        screen=game.get_screen()
        
  #      self._show_text()
        for row in range(len(screen)):
            for col in range(len(screen[row])):
                if screen[row][col]==1:
                    self.canvas.create_oval(col*self.col_unit,row*self.row_unit,
                                             (col+1)*self.col_unit,(row+1)*self.row_unit,fill='black')
                if screen[row][col]==-1:
                    self.canvas.create_oval(col*self.col_unit,row*self.row_unit,
                                             (col+1)*self.col_unit,(row+1)*self.row_unit,fill='white')


    def _show_text(self):
        self.text_turn.grid_forget()
        self.text_turn=tkinter.Label(master=self.window,text='TURN:{:}'.format(game.get_turn()),font=self.font)
        self.text_turn.grid(row=0,column=0)
        self.black_score.grid_forget()
        self.black_score=tkinter.Label(master=self.window,text='Black:{:}'.format(game.black_score()),font=self.font)
        self.black_score.grid(row=0,column=1)
        self.white_score.grid_forget()
        self.white_score=tkinter.Label(master=self.window,text='White:{:}'.format(game.white_score()),font=self.font)
        self.white_score.grid(row=1,column=1)
              
    def _end(self):
        if game.all_invalid():
            if game.all_invalid():
                return True
            else:
                return False
        else:
            return False
    def _resized(self,event: tkinter.Event):
        self.canvas.delete(tkinter.ALL)
        self.canvas_width = self.canvas.winfo_width()
        self.canvas_height = self.canvas.winfo_height()
        self.row_unit=(self.canvas_height/self.height)*self.unit
        self.col_unit=(self.canvas_width/self.width)*self.unit
        self._originalboard()
        self._drawspot()

if __name__ == '__main__':
    a=GUI()



