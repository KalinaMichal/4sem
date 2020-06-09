def primes(n):
    nLista = []
    for i in range(2,n+1):    #sprawdzamy od 2 do n
      for x in range(2,i):
        if (i % x) == 0:    #sprawdzam reszte, przy dzieleniu przez mniejsza liczbe
            break           #jak cos sie dzieli to liczba nie jest prime
        else:
            if( x == i-1):
                nLista.append(i)
    print ('Liczby pierwsze: ')
    print (nLista)

primes(7) # tak wywołuje funkcje nLiczby