import random

def find_shortest_path(graph, start, end, path=[]):
    path = path + [start]
    if start == end:
        return path
    if not start in graph:
        return None
    shortest = None
    for node in graph[start]:
        if node not in path:
            newpath = find_shortest_path(graph, node, end, path)
            if newpath:
                if not shortest or len(newpath) < len(shortest):
                    shortest = newpath
    return shortest

def makeWay(i,j,N,c):
    path = find_shortest_path(graph, str(i), str(j))
    #print(i, "=>", j, "ma:", path)
    suma=0
    m=0
    for k in range(len(path) - 1):
        a[int(path[k])][int(path[k+1])] = N[int(path[k])][int(path[k + 1])] + N[int(path[0])][int(path[len(path) - 1])]
        m+=a[int(path[k])][int(path[k+1])]
        #print("a[", int(path[k]), "][", int(path[k + 1]), "]= ", a[int(path[k])][int(path[k + 1])])
    m=m/len(path)
    for k in range(len(path) - 1):
        i=int(path[k])
        j=int(path[k+1])
        if(i<j):
            suma += a[i][j] / ((c[i][j] / m) - a[i][j])
        else:
            suma += a[i][j] / ((c[j][i] / m) - a[i][j])
   # print("t=", suma)
    return suma

def isGraphOK(graph,N,c):
    # Licze u dla wszystkich mozliwych sciezek:
    for i in range(1, 21):
        for j in range(1, 21):
            path = find_shortest_path(graph, str(i), str(j))
            if(i!=j):
                if (path == None):
                    #print("aaa")
                    return 0
                for k in range(len(path) - 1):
                    if(int(path[k])<int(path[k+1])):
                        ce=c[int(path[k])][int(path[k+1])]
                    else:
                        ce = c[int(path[k+1])][int(path[k])]
                    if(ce <= N[int(path[k])][int(path[k + 1])] + N[int(path[0])][int(path[len(path) - 1])]):
                        return 0
    return 1
def losujgraf(N,c,Tmax,G):
    graf = []
    graf.append([])
    graf.append(['20', '2','11'],)#1
    graf.append(['3', '1'])
    graf.append(['4', '2', '13', '4','8'])
    graf.append(['5', '3', '18'])#4
    graf.append(['6', '4'])#5
    graf.append(['7', '5', '16'])
    graf.append(['8', '6', ])
    graf.append(['9', '7', '14', '3','3'])
    graf.append(['10', '8', '19'])#9
    graf.append( ['11', '9'])#10
    graf.append( ['12', '10', '1'])
    graf.append( ['13', '11'])
    graf.append( ['12', '14', '3'])
    graf.append(['13', '15', '19', '8'])#14
    graf.append(['14', '16'])#15
    graf.append(['15', '17', '6'])
    graf.append(['16', '18'])#17
    graf.append(['17', '19', '4'])#18
    graf.append(['18', '20', '9','14'])#19
    graf.append(['1', '19'])#20
    #Tworze nowy graf
    for i in range(1,20):
        j=i+1
        r=random.random()
        if(r>0.8):
            graf[i].remove(str(j))
            graf[j].remove(str(i))
    i=1
    j=20
    if (random.random() > 0.8):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=1
    j=11
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=3
    j=8
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=3
    j=13
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=4
    j=18
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=6
    j=16
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=8
    j=14
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=9
    j=19
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))
    i=14
    j=19
    if (random.random() > 0.7):
        graf[i].remove(str(j))
        graf[j].remove(str(i))

    tmpgraph={    '1': graf[1],
                '2': graf[2],
             '3': graf[3],
             '4': graf[4],
             '5': graf[5],
             '6': graf[6],
             '7': graf[7],
             '8': graf[8],
             '9': graf[9],
             '10': graf[10],
             '11': graf[11],
             '12': graf[12],
             '13': graf[13],
             '14': graf[14],
             '15': graf[15],
             '16': graf[16],
             '17': graf[17],
             '18': graf[18],
             '19': graf[19],
             '20': graf[20]}
    return(tmpgraph)

graph = {    '1': ['20', '2','11'],
            '2': ['3', '1'],
         '3': ['4', '2', '13','4'],
         '4': ['5', '3', '18'],
         '5': ['6', '4',],
         '6': ['7', '5', '16'],
         '7': ['8', '6',],
         '8': ['9', '7', '14','3'],
         '9': ['10', '8', '19'],
         '10': ['11', '9'],
         '11': ['12', '10', '1'],
         '12': ['13', '11'],
         '13': ['12', '14', '3'],
         '14': ['13', '15', '19','8'],
         '15': ['14', '16'],
         '16': ['15', '17', '6'],
         '17': ['16', '18'],
         '18': ['17', '19', '4'],
         '19': ['18', '20', '9','14'],
         '20': ['1', '19']}

"""tworze tablice t"""
"""t[z kądś...][do kądś....] i vice versa"""
c=[] # to t to jest moim c
N=[]
p=[]
a=[]
for i in range(21):
    c.append([])
    N.append([])
    p.append([])
    a.append([])
    for j in range(21):
        c[i].append(0)
        N[i].append(0)
        p[i].append(0)
        a[i].append(0)
m=0
for i in range(1,20):
    c[i][i+1]=10

c[1][20]=20
c[1][11]=20
c[3][8]=20
c[3][13]=20
c[4][18]=20
c[6][16]=20
c[8][14]=20
c[9][19]=20
c[14][19]=20    #sciezka e: od 14 do 19
#-----------------------
#c[1][5]=14
#c[16][20]=14
#c[8][20]=14
#----------------------


for i in range(1,21):
    for j in range(1,21):
        if(i!=j):
            N[i][j]=1
for i in range(1, 20):
    N[i][i + 1] = 2
for i in range(1, 19):
    N[i][i + 2] = 3
for i in range(1, 16):
    N[i+5][i] = 5
for i in range(2, 21):
    N[i][1] = 4

for i in range(1,21):
    for j in range(1,21):
        if(i!=j):
            N[i][j]=N[i][j]*1
            m+=N[i][j]
            c[i][j]=c[i][j]*1000
G=m

#-----------------------------------------------------
#Opoznienie srednie
#-----------------------------------------------------
T=0
licznik=0
#Licze T dla wszystkich mozliwych sciezek:
for i in range(1,21):
    for j in range(1,21):
        if(i!=j):
            tmp=(1 / G) * makeWay(i, j, N, c)
            T += tmp
            licznik+=1

if(T>0):
    T=T/licznik
    print("srednie opoznienie: ",T)
else:
    print("inf")

#-----------------------------------------------------
#Niezawodnosc
#-----------------------------------------------------


####### METODA MONTE CARLO #############
dobre_grafy=0
graf=[]
prob=100
Tmax=10
for p in range(prob):
    tmpgraf=losujgraf(N,c,Tmax,G)
    if(isGraphOK(tmpgraf,N,c)>0):
        T = 0
        # Licze T dla wszystkich mozliwych sciezek:
        for i in range(1, 21):
            for j in range(1, 21):
                if (i != j):
                    T += (1 / G) * makeWay(i, j, N, c)
                    licznik += 1
        T = T / licznik
        print(T)
        if (T < Tmax):
            dobre_grafy +=1
print(dobre_grafy/prob)
