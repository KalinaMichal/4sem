#!/usr/bin/env python3.8
import os, sys

path = os.path.dirname(os.path.realpath(sys.argv[0]))
path=path + sys.argv[1]
print(path)
print("Pliki w folderze: ")
filenames = os.listdir(path)
print(filenames)
for filename in filenames:
    os.rename(path+"/"+filename, path+"/"+filename.lower())
