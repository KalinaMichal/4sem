macierz= ["1.1 2.2 3.3", "4.4 5.5 6.6", "7.7 8.8 9.9"]

transpo=([liczba for liczba in (' '.join(macierz[j].split()[i] for j in range(len(macierz))) for i in range(len(macierz)))])

print(transpo)
