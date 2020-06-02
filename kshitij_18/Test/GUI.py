from queue import Queue
from threading import Thread
from tkinter import * 
class Daemon:

    def start(self):
        self.in_queue = Queue()
        self.out_queue = Queue()
        Thread(target=self.run,daemon=True).start()
        return self.in_queue,self.out_queue
        # start console in a different window
        # ### Gives Error tcl handler etc
        #maybe process is needed here
        ###idk

class ConsoleWindow(Daemon):
    def __init__(self,max_output=10,max_input=10):
        self.max_output=max_output
        self.max_input=max_input

    
    def run(self): 
        root=Tk() 
        frame = Frame()
        frame.pack(expand=YES, fill=BOTH) 
        self.frame = frame

        text = Text(frame, relief=SUNKEN)
        text.config(height=1)
        text.pack(side=TOP, expand=NO, fill=X)
        text.bind('<Key-Return>', lambda x:self.input())
        text.insert("0.0","\n")
        text.see(END)
        self.in_text = text

        text = Text(frame, relief=RAISED)
        text.config(height=8)
        text.pack(side=TOP, expand=YES, fill=BOTH)
        self.out_text = text

        self.get_in_queue()

        self.frame.mainloop() # :| im sorry my love. *TARDIS explodes violently*

    def get_in_queue(self):
        if not self.in_queue.empty():
            x = self.in_queue.get()
            if x is None:
                self.quit()
                return
            self.output(x)
        self.id=self.frame.after(100, self.get_in_queue)  # 10 times per sec

    def put_out_queue(self,thing):
        self.out_queue.put(thing)
        
    def output(self,string):
        self.out_text.insert('1.0', string.strip('\n') + '\n')
        self.out_text.delete(str(self.max_output+1)+'.0', END)  # max number 
        
    def input(self):
        lines = self.in_text.get("1.0", END).splitlines()
        del_inp=max(0,len(lines)-self.max_input)
        self.in_text.delete('0.0',str(del_inp)+'.0')  # max number 

        self.put_out_queue( lines[-1])

    def quit(self):
        pass

if __name__=="__main__":
    import time
    c=ConsoleWindow()
    out,inp=c.start()
    while True:
        time.sleep(.5)
        if not inp.empty():
            out.put(inp.get())

