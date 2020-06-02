def cached_input( ):
    inp=input()
    cache.append(inp)
    return inp 

def buffered_input():
    inp=buffer.pop(0)
    return inp
