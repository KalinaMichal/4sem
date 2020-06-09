import random

"""##########"""
"""SYGNAL JAM"""
"""##########"""

class JAM_Signal():
    def __init__(self, miejsce_stluczki,n ):
        self.miejsce_stluczki=miejsce_stluczki
        self.droga=[]
        for i in range(n+1):                                # tworze pasmo n+1-elementowe
            self.droga.append(0)
        self.droga[miejsce_stluczki+1]="JAM"
        self.droga[miejsce_stluczki-1] = "JAM"
        self.pozycja_prawa=miejsce_stluczki+1
        self.pozycja_lewa=miejsce_stluczki-1
        self.koniec_internetu = False
        print('\033[91m',"Powstal JAM | miejsce stluczki: ",miejsce_stluczki,'\033[0m')

    def move(self):
        for i in range(1,len(self.droga)-self.miejsce_stluczki):   #w prawo
            self.droga[len(self.droga)-i]=self.droga[len(self.droga)-i-1]
            self.droga[len(self.droga) - i - 1]=0
        self.pozycja_prawa+=1
        for i in range(self.miejsce_stluczki):                #w lewo
            self.droga[i]=self.droga[i+1]
            self.droga[i+1]=0
        self.pozycja_lewa-=1
        if(self.pozycja_lewa<0 and self.pozycja_prawa>len(self.droga)):
            print('\033[90m',"Jeden z sygnalow JAM dotarl do konca kabla, nie bedzie dluzej wyswietlany",'\033[0m')
            self.koniec_internetu=True

    def print(self):
        print("JAM: ",self.droga)

"""#############"""
"""SYGNAL ZWYKLY"""
"""#############"""

class Signal():
    def __init__(self, wiadomosc,n ):
        self.droga=[]
        for i in range(n+1):                                # tworze pasmo n+1-elementowe
            self.droga.append(0)
        self.nie_wyslane = wiadomosc.tresc                #to co mi jeszcze zostalo do wyslania (na poczatku calosc)
        self.wiadomosc=wiadomosc                          #cala wiadomosc
        if (wiadomosc.odbiorca > wiadomosc.nadawca):      #w ktora strone idzie sygnal
            self.prawo = True
        else:
            self.prawo = False
        self.wyslana_cala=False                           #Czy wiadomosc cala wyslana
        self.koniec_internetu=False                       #Zebym w razie kolizji nie wysylal wiadomosci w nieskonczonosc, jak dojdzie do konca kabla to koniec. Dotarla do do nadawcy na 100%
        self.miejsce=wiadomosc.nadawca                    #gdzie jest moja wiadomosc
        self.key=1

    def move(self,t):
        if(self.prawo):
            for i in range(1,len(self.droga)-self.wiadomosc.nadawca):
                self.droga[len(self.droga)-i]=self.droga[len(self.droga)-i-1]
                self.droga[len(self.droga)-i-1]=0
            self.miejsce+=1
            if((self.miejsce-self.wiadomosc.nadawca)==len(self.wiadomosc.tresc)*self.key):
                self.wyslana_cala=True
                print('\033[94m',"wiadomosc uzytkownika:",self.wiadomosc.nadawca,"zostala cala wyslana",'\033[0m')
            if (self.miejsce - len(self.wiadomosc.tresc)*self.key == len(self.droga)):
                print('\033[90m',"Wiadomosc uzytkownika:", self.wiadomosc.nadawca, "dotarala do konca kabla, nie bedzie dluzej wyswietlana",'\033[0m')
                self.koniec_internetu = True
        else:
            for i in range(self.wiadomosc.nadawca+1):
                self.droga[i]=self.droga[i+1]
                self.droga[i+1]=0
            self.miejsce-=1
            if((self.wiadomosc.nadawca-len(self.wiadomosc.tresc)*self.key)==self.miejsce):
                self.wyslana_cala=True
                print('\033[94m',"wiadomosc uzytkownika:",self.wiadomosc.nadawca,"zostala cala wyslana",'\033[0m')
            if(self.miejsce+len(self.wiadomosc.tresc)*self.key==0):
                print('\033[90m',"Wiadomosc uzytkownika:",self.wiadomosc.nadawca,"dotarala do konca kabla, nie bedzie dluzej wyswietlana",'\033[0m')
                self.koniec_internetu=True

        if(len(self.nie_wyslane)!=0 and (t % self.key)==0):
            self.droga[self.wiadomosc.nadawca]=self.nie_wyslane[0]
            self.nie_wyslane=self.nie_wyslane[1:]
            return 0                #uzytkownik w tym ruchu nie moze juz nadawac zadnego innego sygnalu
        return 1                    #w tym ruchu nic nie bylo wsadzane do kanalu wiec uzytkownik moze nadac jeszcze jakis inny sygna;

    def reset(self,id):
        self.nie_wyslane = self.wiadomosc.tresc                #to co mi jeszcze zostalo do wyslania (na poczatku calosc)
        self.miejsce=self.wiadomosc.nadawca                    #gdzie jest moja wiadomosc
        #wylosuj liczbe i za kazdym wysylaniem sprawdzaj czy jest podzielne i gotowe
        self.key=random.randint(len(self.droga), len(self.droga)*int(len(self.droga)/5))
        print('\033[93m',"Uzytkownik",id,"wie ze byla stluczka | wylosowal:",self.key, '\033[0m')

    def print(self,id):
        print("syganl",id,": ",self.droga)
        #print("nie wyslane: ",self.nie_wyslane,"miejsce: ",self.miejsce)