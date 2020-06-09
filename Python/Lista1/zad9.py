from math import sin

x=1
y=1
operators = {"+": x + y, "-": x - y, "*": x * y, "/": x / y, "^": x ** y, "s": sin(x)}

wyrazenie = []
liczby = []
dzialania = []
wyrazenie = input('Podaj operacje: ')
list(wyrazenie)
last = False
liczba=0
weHaveSinus=False
i=0
                #UKLADANIE WSZYSTKIEGO DO LIST#
for i in range (0,len(wyrazenie)):
    if (wyrazenie[i] == "0" or wyrazenie[i] == "1" or wyrazenie[i] == "2" or wyrazenie[i] == "3"  or wyrazenie[i] == "4" or wyrazenie[i] == "5" or wyrazenie[i] == "6" or wyrazenie[i] == "7" or wyrazenie[i] == "8" or wyrazenie[i] == "9"):
        if(last is False):
            liczba=int(wyrazenie[i])
            last=True
        else:
            liczba=liczba*10+int(wyrazenie[i])
            last=True
        if(i==len(wyrazenie)-1):
            liczby.append(liczba)
    if(wyrazenie[i] == "s"):
        if(len(wyrazenie)-i>3):
            if(wyrazenie[i+1]=="i" and wyrazenie[i+2]=="n" and wyrazenie[i+3]=="("):
                i=i+3
                weHaveSinus = True
                last=False
        else:
            print("zle dane1")

    if(weHaveSinus and last and wyrazenie[i] == ")"):
        liczby.append(liczba)
        last = False
        weHaveSinus = False
        dzialania.append("s")

    if(wyrazenie[i] in operators and wyrazenie[i]!="s"):
        dzialania.append(wyrazenie[i])
        if(last):
            liczby.append(liczba)
            last=False
        if (i == len(wyrazenie) - 1):
            print("zle dane2")
        if(weHaveSinus):
            print("zle dane3")

            #ROBIENIE DZIAŁAŃ#
while("s" in dzialania ):
    for i in range (0,len(dzialania)):
        if(dzialania[i]=="s"):
            liczby[i]=sin(liczby[i])
            del dzialania[i]
            break

while("*" in dzialania):
    for i in range (0,len(dzialania)):
        if(dzialania[i]=="*"):
            liczby[i]=liczby[i]*liczby[i+1]
            del liczby[i+1]
            del dzialania[i]
            break

while("/" in dzialania):
    for i in range (0,len(dzialania)):
        if(dzialania[i]=="/"):
            liczby[i]=liczby[i]/liczby[i+1]
            del liczby[i+1]
            del dzialania[i]
            break

while("+" in dzialania):
    for i in range (0,len(dzialania)):
        if(dzialania[i]=="+"):
            liczby[i]=liczby[i]+liczby[i+1]
            del liczby[i+1]
            del dzialania[i]
            break

while("-" in dzialania):
    for i in range (0,len(dzialania)):
        if(dzialania[i]=="-"):
            liczby[i]=liczby[i]-liczby[i+1]
            del liczby[i+1]
            del dzialania[i]
            break

while("^" in dzialania):
    for i in range (0,len(dzialania)):
        if(dzialania[i]=="^"):
            liczby[i]=liczby[i]**liczby[i+1]
            del liczby[i+1]
            del dzialania[i]
            break


""""metoda na tzw. pałe:
for i in range (0,len(dzialania)):
    x=liczby[0]
    if(len(liczby)>0):
        y=liczby[1]
    operators2 = {"+": x + y, "-": x - y, "*": x * y, "^": x ** y, "s": sin(x)}
    if(dzialania[i]!="/"):
        x=operators2[dzialania[i]]
    else:
        if(y==0):
            print("nie mozna dzielic przez 0")
        else:
            x=x/y
    liczby[0]=x
    if(dzialania[i]!="s"):
        del liczby[1]
        """

print(liczby)