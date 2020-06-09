#!/usr/bin/env python3.8
import sys, math, random
#Funkcje pomocnicze
#--------------------------------------------------------
def quick_pow_mod_p(n, e, p):
    """Funcja liczy n^e % p przy urz. log(e) multiplication"""
    result = 1
    # redukuj n
    n = n % p
    while (e > 0):
        # potrzeba exponent
        if (e % 2 == 1):
            result = (result * n) % p

        # dziel e przez bit shifting
        e = int(e) >> 1  # shifting least significiant bit
        n = (n * n) % p

    return result

def nwd(a, b):
    while b:
        a, b = b, a%b
    return a

def miillerTest(d, n):
    a = 2 + random.randint(1, n - 4); #los z [2....n-2]
    x = quick_pow_mod_p(a, d, n);    # a^d % n

    if (x == 1 or x == n - 1):
        return True;

    while (d != n - 1):
        x = (x * x) % n;
        d *= 2;

        if (x == 1):
            return False;
        if (x == n - 1):
            return True;
            # Return
    return False;

def isPrime(n, k):
    # Corner cases
    if (n <= 1 or n == 4):
        return False;
    if (n <= 3):
        return True;
    d = n - 1;
    while (d % 2 == 0):
        d //= 2;

        # iteruj dany numer 'k' razy
    for i in range(k):
        if (miillerTest(d, n) == False):
            return False;
    return True;

def find_d(phi_n,e):
    k = 1
    while True:
        if (k * phi_n + 1) % e == 0:
            return (k * phi_n + 1) // e
        k+=1
#--------------------------------------------------------------------
#Funkcje szyfrujace i deszyfrujace
#--------------------------------------------------------------------
def encription(n, e, msg):
        return quick_pow_mod_p(msg, e, n)  # c = (m ** e) % n


def generate_key(bits):
#Losowanie liczb pierwszych o zadanej dlugosci testem millera
    dlugosc=(int(math.log(2) / math.log(10) * (bits)))+1
    k = 2**(bits);

    primes=[]
    for n in range(k, k+1000):#daje +10000 a nie wiecej bo liczenie zajelo by za dlugo
        if (isPrime(n, 2) and len(str(n))==dlugosc):
            primes.append(n)

    print("Kilka liczb pierwszych o zadaej dlugosci: ")
    print(primes)
    print("")
    prime1 = 53
    prime2 = 53
    while(prime1==prime2):
        prime1 = random.choice(primes)
        prime2 = random.choice(primes)


    print(prime1, "=p1 ,p2= ",prime2)
#Liczenie n-a i reszty
    n = prime1 * prime2
    print("n=",n)
    fi = (prime1 - 1) * (prime2 - 1)  # Euler(n)

#Losowanie e
    e=3
    while(nwd(fi, e)!=1): # e = losowa liczba mala nieparzysta, bez wspolnego dzielnika z fi
        e=2+e

    print(fi,"=fi")
    print("e=",e)

    d=find_d(fi,e)
    print("d=",str(d))

#Od tej lini mam juz { d, e, n }
    plik = open("key.pub", "w")# otwieram plik
    try:
        plik.write(str(n) + ";" +str(e))
    finally:
        plik.close()

    plik = open("key.priv", "w")# otwieram plik
    try:
        plik.write(str(d))
    finally:
        plik.close()

def getKeys():
    plik = open("key.pub", "r")# otwieram plik
    try:
        tekst=plik.read()
    finally:
        plik.close()
    return(tekst.split(";"))

#--------------------------------------------------------------------
#Main
#--------------------------------------------------------------------

alfabet1 = '          ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/,.? '
alfabet2 = {}
for i in range(10, 78):
    alfabet2[alfabet1[i]] = i

if(sys.argv[1]=="--gen-keys"):
    generate_key(int(sys.argv[2]))

if(sys.argv[1]=="--encrypt"):

    m = sys.argv[2]  # wiadomosc do wysalnia
    keys=getKeys()
    for i in range(len(m)):
        msg = int(alfabet2[m[i]])
        print(encription(int(keys[0]), int(keys[1]),msg), end = '_')

if(sys.argv[1]=="--decrypt"):
    keys=getKeys()
    plik = open("key.priv", "r")# otwieram plik
    try:
        privkey=plik.read()
    finally:
        plik.close()
    chiphertext  = sys.argv[2]  # zaszyfrowany tekst
    singlechip = chiphertext.split("_")
    for i in range (len(singlechip)):
        if (singlechip[i] != ""):
            msg = quick_pow_mod_p(int(singlechip[i]), int(privkey), int(keys[0]))  # msg=(c ** d) % n
            # msg = math.fmod(msg, int(keys[0]));
            print(alfabet1[msg], end='')

