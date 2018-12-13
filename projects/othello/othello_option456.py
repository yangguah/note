import tkinter


class option():
    def __init__(self):
    
        self.font=('Helvetica', 16)# the script
        self.root_window = tkinter.Tk()
        self._row_number=tkinter.IntVar(value=4)#the original row number is 4
        self._column_number=tkinter.IntVar(value=4)# the original column number is 4
        self._who_is_first=tkinter.StringVar(value='black')#the original first move is blcak
        self._top_left=tkinter.StringVar(value='black')# the original top-left move is black
        self._game_rule=tkinter.StringVar(value='>')# the orginal winning rule is >
        self.root_window.title('Option')# the title of this window is 'Option'
      
        self.canvas = tkinter.Canvas(
            master = self.root_window,
            width = 500, height = 400,
            background = 'white')
    

        self.canvas.grid(row = 0, column = 0, padx = 10, pady = 10,
         sticky = tkinter.N + tkinter.S + tkinter.W + tkinter.E,
           rowspan = 6, columnspan = 2)

        for i in range(5):
            self.root_window.rowconfigure(i,weight=1)
        for i in range(2):
            self.root_window.columnconfigure(i, weight = 1)

        self._run()
        tkinter.mainloop()
    def _rows(self):# the optional row number for the user is between 4-16, which are even numbers
        row=tkinter.Label(master=self.root_window,text='Rows:',font=self.font)
        row.grid(row=0,column=0)
        row_option=tkinter.OptionMenu(self.root_window,self._row_number,4,6,8,10,12,14,16)
        row_option.grid(row = 0, column = 1)
 
    def _columns(self):# the optinal column number for the user is 4-16, which are even numbers
        column=tkinter.Label(master=self.root_window,text='Columns:',font=self.font)
        column.grid(row=1,column=0)
        column_option=tkinter.OptionMenu(self.root_window,self._column_number,4,6,8,10,12,14,16)
        column_option.grid(row=1,column=1)

    def _move_first(self):# the optinal first move for the user is either black or white
        move=tkinter.Label(master=self.root_window,text='Who is first:',font=self.font)
        move.grid(row=2,column=0)
        move_option=tkinter.OptionMenu(self.root_window,self._who_is_first,'black','white')
        move_option.grid(row=2,column=1)

    def _topleft(self):# the optinal top left position for the user is black or white
        topleft=tkinter.Label(master=self.root_window,text='Top left:',font=self.font)
        topleft.grid(row=3,column=0)
        topleft_option=tkinter.OptionMenu(self.root_window,self._top_left,'black','white')
        topleft_option.grid(row=3,column=1)
    def _rule(self):# the optinal winning rule for the user is either > or <
        rule=tkinter.Label(master=self.root_window,text='Game Rule:',font=self.font)
        rule.grid(row=4,column=0)
        rule_option=tkinter.OptionMenu(self.root_window,self._game_rule,'>','<')
        rule_option.grid(row=4,column=1)
    def _start_game(self):# Once the user clicks the 'Start Game' button,the window will close
        startgame=tkinter.Button(self.root_window,text='Start Game',command=self._button)
        startgame.grid(row=5,column=0)
    def _button(self):# close the window
        self.root_window.destroy()    
    def _run(self):
        self._rows()
        self._columns()
        self._move_first()
        self._topleft()
        self._rule()
        self._start_game()
   


if __name__ == '__main__':
    app=option()
        


