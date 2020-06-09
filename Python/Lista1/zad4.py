def prime_factors(n):
    prime = []
    i = 2
    while i*i <= n:
        counter = 0         
        while n % i == 0:
            n //= i
            counter += 1
        if counter != 0:
            prime.append((i,counter))
        i += 1
    if n > 1:
        prime.append((n,1))
    return prime

x = prime_factors(10)
print("(liczba, potega)")
for i in range (len(x)):
        if(i==len(x)-1):
            print(x[i])
        else:
            print(x[i],"* ", end = '')