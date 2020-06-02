from Program import Program
import utility
from utility import get 
from graph import get_edge
from Window import Window
from collections import defaultdict
from threading import Thread

def re(x):
    return x.real,x.imag


class IO:
    def input(self):
        print('I: ',end='')
        return input()
    def print(self,x):
        print('O: '+x)

class BotTester:
    base_x = 10
    base_y = 10

    def __init__(self,x,y,edge,start,goal,executable,auto):
        self.x,self.y=x,y
        self.edge=edge
        self.auto=auto
        self.goal=complex(*self[goal])
        
        self.bot_pos=self.start=complex(*self[start])
        self.bot_angle=complex(0,1)
        self.window=Window(self.x,self.y)
        self.c_program=Program(executable)
        #self.c_program=IO()
        Thread(target=self.refresh,daemon=False).start()
        self.window.launch()

    def refresh(self):
            import time,sys
            while True:
                if not self.window.alive:
                    sys.exit()
                self.window.update()
                for i in self.edge:
                    self.window.create_edge(self[i[0]],self[i[1]])
                    self.window.create_node(re(self.bot_pos),5)
                    self.window.create_node(re(self.bot_pos+self.bot_angle/10),1)
                time.sleep(.2)

                self.get_bot()

    def __getitem__(self,n):
        return n%self.x,n//self.x

    def s(self,x,y):
        return int(x+y*self.x)
    
    def at_edge(self,a):
            n_e=[self.s(*re(a)),self.s(*re(self.bot_pos))]
            for e in self.edge:
                if e in  (n_e,n_e[::-1]):
                    return True
            return False
    
    def at_node(self,a):
            for e in self.edge:
                if self.s(*re(a)) in e:
                    return True
            return False
              
    def get_bot(self):
            while True:
                inp=self.c_program.input()
                if '>' in inp:
                    if 'Further' in inp:
                        if self.auto:
                            ans='y'
                            if self.goal==self.bot_pos:
                                ans='g'
                                self.goal=None
                                self.auto=False
                        else:
                            ans=input() 
                        self.c_program.print(ans)
                    elif 'Distance' in inp:
                        i=0
                        while i<100:
                            i+=1
                            self.bot_pos+=self.bot_angle
                            if self.at_node(self.bot_pos):
                                self.c_program.print(str(10*i))
                                return
                        print("Failed")
                        exit()
                    elif 'Ways' in inp:
                        ans=[]
                        for i in complex(1,0),complex(0,-1),complex(-1,0),complex(0,1):
                            ans.append(str(int(self.at_edge(self.bot_angle*i+self.bot_pos))))
                        self.c_program.print(''.join(ans))
                        return
                elif ':' in inp:
                    if 'Turn' in inp:
                        if 'Right' in inp:
                            self.bot_angle*=complex(0,-1)
                        elif 'Left' in inp:
                            self.bot_angle*=complex(0,1)
                        elif 'Back' in inp:
                            self.bot_angle*=complex(-1,0)
                    elif 'Reset'  in inp:
                            self.bot_pos=self.start
                            self.bot_angle=complex(0,1)
                    return
                
def cached_input( ):
    inp=input()
    cache.append(inp)
    return inp 

def buffered_input():
    inp=buffer.pop(0)
    return inp

def get_map():
        new_print(">> Graph Dimensions [x y] ?")
        x,y=get()
        
        new_print(">> Start?")
        start=get()[0]

        new_print(">> Goal?")
        goal=get()[0]

        new_print(">> Number of edges(n) ? and n*[1 2] of edges?")
        edgelist=get_edge()
        return x,y,edgelist,start,goal
          

if __name__=="__main__":
    from argparse import ArgumentParser
    argparser=ArgumentParser(description="Bot tester\n test bot on the graph")
    argparser.add_argument('file',type=str,nargs='?',default='map1',help='enter graph file')
    argparser.add_argument('executable',type=str,nargs='?',default='../tester',help='put an executable filename')
    argparser.add_argument('--a',dest='auto',action='store_true',default=False,help='automatic mode')
    args=argparser.parse_args() 
    if args.file:
        utility.new_input=buffered_input
        new_print=lambda x:0
        with open(args.file,mode='r') as bufferfile:
                buffer=bufferfile.readlines()
    else:
        utility.new_input=cached_input
        new_print=print
        cache=[]
    imap=get_map()

    if not args.file:
            with open('cache',mode='w') as cachefile:
                cachefile.write('\n'.join(cache))
    try:
        b=BotTester(*imap,args.executable,args.auto)
    except KeyboardInterrupt:
            print("Forcefully",end=" ")
    finally:
            print("Exiting")

            


