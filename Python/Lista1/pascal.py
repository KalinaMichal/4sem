import math

def combination(n, r): # n po k fatorial-silnia
    return int((math.factorial(n)) / ((math.factorial(r)) * math.factorial(n - r)))

def pascal(row):
    result = []
    for i in range(row+1): # rows=rzedy
        result.append(combination(row, i))
    return result

n=10
# drukowanie resultu:
for i in range(n):
    for j in range (n-i):
        print("  ", end = '')
    print(pascal(i))
