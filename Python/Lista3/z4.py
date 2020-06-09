
#zakomentowane jest inna metoda
def quick(lista):
    if lista == []:
        return []
    else:
        pivot = lista[0]
        smaller = quick(list(filter(lambda x: x < pivot, lista[1:])))
        #smaller = qsort1([x for x in list[1:] if x < pivot])
        bigger = quick(list(filter(lambda y: y >= pivot, lista[1:])))
       # bigger = qsort1([x for x in lista[1:] if x >= pivot])
        return smaller + [pivot] + bigger

numbers = (1,6,3,32,85,23,9,123,23,336)
print (quick(numbers))



