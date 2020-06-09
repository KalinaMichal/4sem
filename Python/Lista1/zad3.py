
def uniq(n):
    for i in range(len(n)):
        for j in range((i+1),len(n)):
            if(n[i]==n[j]):
                del n[j]
                uniq(n)
                break
    return n

nLista = [1,1,2,2,2,3,3,5,5,5,4,4,4,0,1,4]
print(nLista)
print(uniq(nLista))
