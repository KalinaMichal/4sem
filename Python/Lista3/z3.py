
sciezka_do_pliku='/home/michal/4SEM/Pthon/Lista3/test.txt'
wynik = [sum(int(line.split()[len(line.split())-1]) for line in open(sciezka_do_pliku))]
print("calkowita liczba bajtow:",wynik[0])
