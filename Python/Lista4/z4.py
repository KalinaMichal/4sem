#!/usr/bin/python3
import math
from inspect import getfullargspec


def overload(function):
    MyFunc.functions[(function.__name__, len(getfullargspec(function).args))] = function

    def overrider(*args, **kwargs):
        return MyFunc(function.__name__)(*args, **kwargs)

    return overrider


class MyFunc:
    functions = {}

    def __init__(self, func_name):
        self.func_name = func_name

    def __call__(self, *args, **kwargs):
        x = MyFunc.functions.get((self.func_name, args.__len__()))
        if x is None:
            raise Exception("Nie ma takiej funkcji")
        return x(*args, **kwargs)


@overload
def norm(x, y):
    return math.sqrt(x * x + y * y)


@overload
def norm(x, y, z):
    return abs(x) + abs(y) + abs(z)

#if __name__=='__main__':
#    print(f"norm(2,4) = {norm(2, 4)}")
#    print(f"norm(2,3,4) = {norm(2, 3, 4)}")

@overload
def test():
 print(0)

@overload
def test(a):
 print(1)

@overload
def test(a,b):
 print(2)

test()
test(1)
test(1, 2)