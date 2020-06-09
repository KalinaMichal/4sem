liczby= [2,1,3]
seq=liczby

#def powerset(seq): # To jest pierwsza metoda
 #   return [x for ps in powerset(seq[1:]) for x in ([seq[0]] + ps, ps)] if seq else [[]]

#Tu jest lepsza metoda
wynik=([([liczby[j] for j in range(len(liczby)) if (i & (1 << j)) ])for i in range(1 << len(liczby))])
print(wynik)


