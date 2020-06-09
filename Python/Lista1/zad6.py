from random import randint


def srednia(n):
    srednia=0
    for i in range(0, len(n)):
        srednia+=n[i]
    return srednia/len(n)

def max2(lista):
    toDel = lista.index(max(lista))
    del lista[toDel]
    return max(lista)

def min2(lista):
    toDel = lista.index(min(lista))
    del lista[toDel]
    return min(lista)



lista = []
parzyste=0
for i in range(0,19):           #losowanie liczb
    lista.append(randint(1,100))
    print(lista[i])
    if(lista[i] % 2 ==0):
        parzyste+=1

print("srednia: ",srednia(lista))
print("max: ",max(lista))
print("2nd max: ",max2(lista))
print("min: ",min(lista))
print("2nd min: ",min2(lista))
print("parzyste: ",parzyste)