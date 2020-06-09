import numpy as np
import csv
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
import pandas as pd

df_plik = pd.read_csv("/home/michal/PycharmProjects/Lista5/venv/ratings.csv")
users = df_plik.loc[df_plik.movieId == 1, "userId"].to_numpy()

def get_data(m: int):
    Y = df_plik.loc[(df_plik.userId.isin(users)) & (df_plik.movieId == 1), ["rating"]].to_numpy()
    Xinfo = df_plik.loc[(df_plik.userId.isin(users)) & (df_plik.movieId > 1) & (df_plik.movieId <= m), ["userId", "movieId", "rating"],].to_numpy()
    X = np.zeros((len(users), m))

    for userId, movieId, rating in Xinfo:
        poczatek = np.where(users == userId)[0][0]
        X[poczatek, int(movieId) - 2] = rating

    return X, Y

#------------------------------------------------main--------------------------------------------

size1 = 200
size2 = 215

#parametryzacje m mozna zrobic recznie poprzez zakomentowanie petli i wpisanie m=....
for m in (10,100,10000):
    X, Y = get_data(m)
    model = LinearRegression().fit(X, Y)

    poczatek = np.arange(size1, size2)

    predicted = plt.plot(
        poczatek+ 0.15, #dalem '+' bo czasem sie na siebie nachodza i nie widac
        [model.predict([X[i]])[0][0] for i in range(size1, size2)],
        'ro',
        label='dane przewidziane'
    )
    actual = plt.plot(
        poczatek,
        [Y[i][0] for i in range(size1, size2)],
        'bo',
        label = 'dane prawidłowe'
    )

    plt.xlim(size1-1,size2)
    plt.ylim(0,6)
    plt.xlabel("id filmu")
    plt.ylabel("ocena")
    plt.legend()
    plt.title("m = %i" % m)
    plt.gcf().canvas.set_window_title("1 podpunkt")
    plt.show()

print("--- 2 podpunkt ---")
for m in (10, 100, 200, 500, 1000, 10000):
    X, Y = get_data(m)
    X2, Y2 = X[:size1], Y[:size1]
    model = LinearRegression().fit(X2, Y2)

    print("m = ",m)
    for i in range(size1, size2):
        print(i + 1, ":", model.predict([X[i]])[0][0], ' | ', Y[i])
