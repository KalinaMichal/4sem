#!/usr/bin/python3
import numpy as np
import collections
import operator
import pandas as pd

df_ratings = pd.read_csv("/home/michal/PycharmProjects/Lista5/venv/ratings.csv")
df_movies= pd.read_csv("/home/michal/PycharmProjects/Lista5/venv/movies.csv")

"""*****************************"""
"""**** Elementy pomocnicze ****"""
"""*****************************"""

def flatten(it: list):
    for x in it:
        if (isinstance(x, collections.Iterable) and
            not isinstance(x, str)):
            for y in flatten(x):
                yield y
        else:
            yield x
np.seterr(divide='ignore', invalid='ignore')


def get_x(m: int):

    ratings=df_ratings.loc[df_ratings.movieId <= m, ["userId","movieId", "rating"]].to_numpy()
    X = [[0 for i in range(m+1)] for j in range(max(df_ratings.loc[df_ratings.movieId == 1, "userId"].to_numpy())+1)]

    for i in range(len(ratings)):
        X[int(ratings[i][0])][int(ratings[i][1])] = ratings[i][2]

    return X

def get_movies():

    movies_tmp = df_movies.loc[df_movies.movieId >= 0, ["movieId", "title"]].to_numpy()
    movies = ["NULL" for k in range(movies_tmp.max(0)[0] + 1)]

    for i in range(len(movies_tmp)):
        movies[int(movies_tmp[i][0])] = movies_tmp[i][1]
    return movies


#------------------------------------------------main--------------------------------------------

"""*****************************"""
"""****  Pobieranie danych  ****"""
"""*****************************"""

m = 10000
x = get_x(m)
movies=get_movies()

my_ratings = np.zeros((10001, 1))
my_ratings[2571] = 5      # patrz movies.csv  2571 - Matrix
my_ratings[32] = 4        # 32 - Twelve Monkeys
my_ratings[260] = 5       # 260 - Star Wars IV
my_ratings[1097] = 4

y = my_ratings

"""*****************************"""
"""****     Obliczenia      ****"""
"""*****************************"""

# obliczamy normę wektorów kolumnowych
np.linalg.norm(x, axis=0)

# obliczamy podobieństwo cosinusowe z każdym użytkownikiem (skorzystamy z mnożenia macierzowego)
z = np.dot(np.nan_to_num(x / np.linalg.norm(x, axis=0)), np.nan_to_num(np.array(y) / np.linalg.norm(y)))

# normalizujemy otrzymany wektor (będzie on reprezentował nasz profil filmowy)
z / np.linalg.norm(z)

# znajdowanie filmow pasujacych do naszego profilu
X = np.nan_to_num(x / np.linalg.norm(x, axis=0))
Z = np.nan_to_num(z / np.linalg.norm(z))

"""*****************************"""
"""****    Wyswietlanie     ****"""
"""*****************************"""

result = list(flatten(np.dot(X.T, Z)))

posortowana=sorted(
    { i : result[i] for i in range(0, len(result) ) }.items(),
    key=operator.itemgetter(1),
    reverse=True)

for i in range (len(posortowana)):
    try:
        if(movies[posortowana[i][0]]!="NULL"):
            print(posortowana[i],movies[posortowana[i][0]])
    except:
        print("error - cos poszlo nie tak", end="")
