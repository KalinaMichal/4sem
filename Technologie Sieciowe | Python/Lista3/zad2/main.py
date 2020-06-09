import numpy as np
import time
import Elementy_kabla.Magistrala as mag

messages=[]

"""Wiadomosci"""
class Wiadomosc():
    def __init__(self, czas, tresc, nadawca, odbiorca):
        self.czas=czas
        self.tresc=tresc
        self.nadawca=nadawca
        self.odbiorca=odbiorca

"""**************************************************************"""
"""**********  SEKCJA DODAWANIA USEROW I WIADOMOSCI *************"""
"""**************************************************************"""
#Jesli sie napisze wiadomosc od uzytkownika ktorego nie to Uzytkownik doda sie automatycznie


"""Przypadek kolizji i losowania odpowiednich wartosci"""

takt=1                                      #czas po jakim ma wyswietlac sie kolejny ruch
pasmo=mag.Magistrala(14)
#pasmo.add_user(0)
pasmo.add_user(10)
messages.append(Wiadomosc(1,'01',5,10))  #czas, widomosc, nadawca, odbiorca
messages.append(Wiadomosc(1,'10',10,5))  #nadawca jest dodawany po to aby bylo wiadomo w ktora strone wyslac sygnal



"""Przypadek proby wyslania sygnalu gdy lacze jest zajete"""
"""
takt=1                                      #czas po jakim ma wyswietlac sie kolejny ruch
pasmo=mag.Magistrala(14)
pasmo.add_user(0)
pasmo.add_user(10)
messages.append(Wiadomosc(0,'11111',0,3))  #czas, widomosc, nadawca, odbiorca
messages.append(Wiadomosc(4,'11111',3,0))
"""


"""Przypadek kolizji spowodawany przez 1 uzytkownika i JAM gdy 1 z uzytkownikow wysyla 2 wiadomosci"""
"""W tym przykladzie widac ze jak 1 uzytkownik wysyle 2 wiadomosci i jest kolizja to on wysle ta druga wiadomosc dopiero wtedy gdy cala 1 wiadomosc zostanie juz wyslana
(mozna to zauwazyc b. dobrze bo pierwsza wiadomosc to same jedynki a druga to same zera)"""
"""
takt=0.1
pasmo=mag.Magistrala(30)
pasmo.add_user(5)
pasmo.add_user(10)
messages.append(Wiadomosc(1,'1111',5,20))
messages.append(Wiadomosc(3,'0000',5,20))
messages.append(Wiadomosc(1,'1111',20,5))
"""


"""Przypadek wysylania 2 wiadomosci przez 1 usera w tym samym czasie"""
"""Nastapi wyslanie najpierw 1 potem 2 a miedzy wiadomosciami bedzie odstep aby bylo wiadomo ze to 2 wiadomosci"""
"""
takt=0.8
pasmo=mag.Magistrala(15)
pasmo.add_user(5)
pasmo.add_user(10)
messages.append(Wiadomosc(1,'1111',5,14))
messages.append(Wiadomosc(1,'0000',5,14))
"""


"""Inny przypadek - wieksze liczby i wiecej uzytkownikow - nic specjalnego, nic nowego"""
"""
takt=0.2
pasmo=mag.Magistrala(35)
pasmo.add_user(5)
pasmo.add_user(10)
pasmo.add_user(15)
pasmo.add_user(20)
messages.append(Wiadomosc(1,'10101',5,15))
messages.append(Wiadomosc(5,'11111',20,10))
"""

"""**************************************************************"""
"""*******************  SEKCJA OBSLUGI KABLA  *******************"""
"""**************************************************************"""

def check_messages(messages):
    for ms1 in messages:#zeby nie bylo sytuacji ze 1 uzytkownik wysyla 2 wiadomosci w tym samym czasie
        for ms2 in messages:
            if(ms1 != ms2):
                if(ms1.nadawca==ms2.nadawca and ms1.czas==ms2.czas):
                    ms2.czas+=1
                    check_messages(messages)
    return messages
messages=check_messages(messages)#naprawinie sytuacji: 1 user wysyla 2 sygnaly dokladnie w tym samym czasie


"""******************"""
#   START PROGRAMU   #
"""******************"""

t=0 #t od time
while(True):
    for msg in messages:
        if(msg.czas==t):
            print('\033[92m',"Wkladam wiadomosc: ",msg.tresc,"| czas: ",t," | miejsce: ",msg.nadawca,'\033[0m')
            if(pasmo.put_message(msg)==0):
                print('\033[93m',"Wiadomosc nadawcy",msg.nadawca,"zostanie wyslana jak sie zwolni lacze",'\033[0m')
                msg.czas+=1
    pasmo.move(t)
    pasmo.check_for_Jams()
    pasmo.Check_for_Users()
    time.sleep(takt)
    t+=1
