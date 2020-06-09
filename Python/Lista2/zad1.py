#!/usr/bin/env python3.8
import os, sys

path = os.path.dirname(os.path.realpath(sys.argv[0]))
path=path +"/"+sys.argv[1]
nazwapliku=path

plik = open(nazwapliku)
i=0
maks=0
for linia in plik:
    if(len(linia)>maks):
        maks=len(linia)
    i+=1

print("liczba linii:", i)
print("maksymalna długość linii: ",maks)

plik.close()

plik = open(nazwapliku)#otwieram plik
try:
    data = plik.read()
finally:
	plik.close()

words = data.split()
print('liczba słów: ', len(words))

print("Rozmiar pliku:", os.path.getsize(nazwapliku), "bajtów")
