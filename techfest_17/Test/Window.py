class Window:

    def __init__(self,x,y):
        from tkinter import Tk,Canvas
        master = Tk() 
        self._canvas = Canvas(master, width=30*x+10, height=30*y+10)
        self._canvas.pack()
        self.x,self.y=x,y
        self.alive=True
        self._canvas.bind('<Destroy>', lambda x:self.kill())

    def kill(self):
        self.alive=False
        self._canvas.quit()

    def launch(self):
         self._canvas.mainloop()

    def scale(self,node):
        x,y=node
        return 10+30*x,10+30*(self.y-1-y)

    def create_node(self, node,r):
        x,y=self.scale(node)
        self._canvas.create_oval(x-r,y-r,x+r,y+r)
    
    def create_edge(self,node_1,node_2): 
        self._canvas.create_line(*self.scale(node_1),*self.scale(node_2))
        self.create_node( node_1,2)
        self.create_node( node_2,2)


    def update(self):
        self._canvas.delete("all") 




    
