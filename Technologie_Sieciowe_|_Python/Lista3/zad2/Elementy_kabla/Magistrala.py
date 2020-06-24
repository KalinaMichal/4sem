import Elementy_kabla.Signals as signals

class Magistrala():
    def __init__(self, n):
        self.n = n
        self.signals = []
        self.messages = []
        self.users = []
        self.jams = []

    def put_message(self, wiadomosc):
        for signal in self.signals:
            if (signal.droga[wiadomosc.nadawca] == ('1' or "JAM" or '0')):
                return 0  # Zwracam 0 aby zwiekszyc czas wiadomosci +1

        self.messages.append(wiadomosc)
        if (wiadomosc.nadawca not in self.users):
            self.users.append(wiadomosc.nadawca)
        self.signals.append(signals.Signal(wiadomosc, self.n))
        return 1  # Zwracam 1 ze wszystko OK, widomosc moze zaczac byc wysylana

    def move(self,t):
        i = 1
        ci_co_w_tym_ruchu_juz_cos_wyslali=[]    #potrzebne aby np. po wylosowaniu jakiejs liczby 1 uzytkownik nie wysylal 2 wiadomosci na raz
        for sig in self.signals:
            if (sig.koniec_internetu == False):
                if(sig.wiadomosc.nadawca not in ci_co_w_tym_ruchu_juz_cos_wyslali): #jesli uzytkownik w tym ruchu nic nie wysylal to moze sb wyslac
                    if(sig.move(t)==0):         #Odstep miedzy 2 wiadomosciami od tego samego uzytkownika to 1 kratka
                        ci_co_w_tym_ruchu_juz_cos_wyslali.append(sig.wiadomosc.nadawca)
                    sig.print(i)

            i += 1
        for jam in self.jams:
            if (jam.koniec_internetu == False):
                jam.move()
                jam.print()

    def add_user(self, uzytkownik):
        if (uzytkownik not in self.users):
            self.users.append(uzytkownik)
        else:
            print("Ten uzytkownik byl juz raz dodany")

    def check_for_Jams(self):   #sprawdzam czy nie ma gdzies stluczki
        for signal1 in self.signals:
            for signal2 in self.signals:
                if (signal1 != signal2):
                    for i in range(self.n):
                        if ((((signal1.droga[i]=='1') or (signal1.droga[i]=='0')) and ((signal2.droga[i] == '1') or (signal2.droga[i]=='0'))) and signal1.koniec_internetu == False and signal2.koniec_internetu == False):
                            self.jams.append(signals.JAM_Signal(i, self.n))
                            return

    def Check_for_Users(self):
        for user in self.users:  # Sprawdzam kazdego uzytkownika
            for jam in self.jams:  # Czy nie ma pod nim syg. JAM
                if (jam.droga[user] == "JAM"):
                    for sig in self.signals:
                        if (sig.koniec_internetu == False and sig.wiadomosc.nadawca == user):  # Jesli syganla nie jest przeterminowany i jest tego nadawcy to trzeba dzialac
                            sig.reset(user)
