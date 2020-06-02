from utility import get

def get_edge():
    edge_n=get()[0]
    edgelist=[]
    for row in  range(edge_n):
        edgelist.append(get())
    return edgelist

from collections import defaultdict

def edge2adj(edgelist):
    adj_m=defaultdict(list)
    for row in  edgelist:
        u,v=row
        adj_m[u].append(v)
        adj_m[v].append(u)
    return adj_m
     
def edge2adjdir(edgelist):
    adj_m=defaultdict(list)
    for row in  edgelist:
        u,v=row
        adj_m[u].append(v) 
    return adj_m

def edge2adjm(edgelist):
    adj_m=defaultdict(lambda :defaultdict(lambda:False))
    for row in  edgelist:
        u,v=row
        adj_m[u][v] =adj_m[v][u]=True
        
    return adj_m

if __name__ =="__main__":
    e=edge2adjm(get_edge())
    for i in range(10):
        for j in range (10):
            if e[i][j]:
                print('x',end='')
            else:
                print(' ',end='')
        print()
            
