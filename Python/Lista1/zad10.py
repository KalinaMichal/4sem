import math
from math import sin
from math import cos

def policz(n,wyrazenie):
    x=n
    try:   #try catch jest dla przypadku 0
        return str(eval(wyrazenie))
    except:
        return "*"

funkcja = input('Podaj funkcje: ')
a = input('Podaj poczatek przedzialu: ')
b = input('Podaj koniec przedzialu: ')
listo=[[" "]*24 for i in range(80)]         #wstawianie do tablicy krzyza
for i in range(24):
    listo[40][i] = "|"
for i in range(80):
    listo[i][12] = "-"

if(a=="pi"):
    a=str(math.pi)
if(a=="-pi"):
    a=str((math.pi)*(-1))
if(b=="pi"):
    b=str(math.pi)
if(b=="-pi"):
    a=str((math.pi)*(-1))

krotka=(float(b)-float(a))/80
wynik = []


max=float(policz((float(a)), funkcja))
min=max
wynik.append(max)

for i in range(1,80):                                   #liczenie !=0
    if((policz((float(a)+krotka*i),funkcja))!="*"):
        x=float(policz((float(a)+krotka*i),funkcja))
        if(x>max):
            max=x
        if(x<min):
            min=x
        wynik.append(str(x))
    else:
        wynik.append(" ")

print("min: ",min)
print("max: ",max)
#RYSOWANIE

#ustalam zakres:
pion=(max-min)/24

#print("pion: ",pion)
for i in range(80):
    tmp=pion
    j=1
    if(wynik[i]!=" "):  #wstawianie do tablic * w odpowiednie miejsca
        while((float(wynik[i])-min)>(j*tmp)):
            j=j+1
        listo[i][24-j] = "*"  # dla 0

 #Drukowanie funkcji
for i in range(24):
    for j in range(78):
        print(listo[j][i], end = '')
    print("")
