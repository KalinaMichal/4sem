import random

class Node(object):
    counter = 0
    def __init__(self, key):
       #Constructor
        self.childrens = []
        self.key = key

    def add_child(self, child): #Add child to Node
        self.childrens.append(child)
        self.counter += 1

#TWORZENIE DRZEWA
def maketree(h):#making trunk
    prakorzen=Node(0)
    if(h==1):
        return prakorzen
    tmp = []
    if(h>2):
        for i in range (h-1):
            tmp.append(Node(random.randint(0,1000)))
        for i in range(h - 1-1):
            tmp[i].add_child(tmp[i+1])
        prakorzen.add_child(tmp[0])
    prakorzen.add_child(make_wnuki(0,h-1))
    return prakorzen
def make_wnuki(cur,max):
    ilosc=random.randint(0,4)
    cur+=1
    pien=Node(random.randint(0,100))
    if(cur<max-1):
        for i in range (ilosc):
            pien.add_child(make_wnuki(cur,max))
    if(cur==max-1):
        for i in range (ilosc):
            tmp = random.randint(0, 100)
            pien.add_child (Node(tmp))
    return pien

#DRUKOWANIE DRZEWA
def print_tree(tablica, pien):
    tablica.append(pien.key)

    for i in range(pien.counter):
        tablicanowa = []
        tablicanowa.append(print_tree(tablicanowa, pien.childrens[i]))
        tablica.append(tablicanowa)
    return tablica
#PRZESZUKIWANIA
def dfs_walk(pien):
    for i in range(pien.counter):
        print(pien.childrens[i].key," ",end = '')
        dfs_walk(pien.childrens[i])

def bfs_walk(tree):
    queue = []
    queue.append(tree)
    counter = 0
        # while queue its not empty yield node and add its children
    while counter < len(queue):
        if queue[counter] is not None:
            yield queue[counter].key
            for child in queue[counter].childrens:
                queue.append(child)
        counter += 1
    return queue

def get_childrens(tree):
    lista=[]
    for i in range(tree.counter):
        lista.append(tree.childrens[i])
    return lista

tree=maketree(6)#losowe drzewo o maksywalnej wielkosci 5
#drukowanie drzewa
print(print_tree([], tree))
#print("")
dfs_walk(tree)
