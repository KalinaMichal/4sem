
def silnia(n):     #"""Obliczanie silni iteracyjnie""", moglem uzyc factorial
    silnia_tmp=1 #zmienna pomocnicza
    if n in (0,1):  #gdy n = 0 lub 1 zwroc 1
        return 1
    else:
        for i in range(2,n+1):  #gdy n>1 mnoz przez kolejne liczby od 2 do n
            silnia_tmp = silnia_tmp*i
    return silnia_tmp

def fraczero(n):
    x=silnia(n)
    licznik=0
    x=str(x)
    x=list(x)
    x.reverse()

    i=0
    while(x[i]=='0'):
        licznik+=1
        i+=1

    return licznik

print(fraczero(100))
