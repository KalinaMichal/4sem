#!/usr/bin/env python3.8
import os, sys, hashlib

names= []
sizes = []
hashes = []
for root, dirs, files in os.walk(".", topdown=False):
    for name in files:
        sizes.append(os.path.getsize(os.path.join(root, name))) #rozmiar
        names.append(os.path.join(root, name))                  #nazwa
        plik = open((os.path.join(root, name)))                 #otwieram plik
        try:
            data = plik.read()
        finally:
            plik.close()
        hashes.append(hashlib.md5(data.encode()).hexdigest())   #hash

taken =[]
for i in range(len(sizes)):
    taken.append(0)

print("******************")
for i in range(len(sizes)):
        if(taken[i]==0):
            taken[i] = 1;
            tmp=0
            for j in range(len(sizes)):
                if(sizes[i]==sizes[j] and hashes[i]==hashes[j]):
                    if(taken[j]==0):
                        taken[j]=1;
                        print(names[j])
                        tmp=1
            if(tmp==1):
                print(names[i])
                print("******************")