class Program:
    import os
    
    def __init__(self,file):
        from subprocess import Popen ,PIPE 
        self.ps=Popen(self.os.getcwd()+'/'+file,stdin=PIPE,stdout=PIPE,shell=False)

    def print(self,inp):
        print("Inp: "+inp.strip('\n')) 
        inp=bytes(inp,'ascii')
        self.ps.stdin.write(inp+b'\n')
        self.ps.stdin.flush()

    def input(self):
        out=[]
        while True:        
            s=self.ps.stdout.read(1)
            out.append(s)
            if s in( b'\n' ,b'?'):
                break
        out=b''.join(out).decode('ascii')
        print("Out: "+out.strip('\n'))
        return out

    def __del__(self):
        self.kill()

    def kill(self):
        self.ps.kill() 

if __name__=='__main__':
    p=Program('tester')
    
    while True:
            print(p.input())
            p.print(input())

    
