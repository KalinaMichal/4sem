#!/usr/bin/python3
import random
import collections


def treeGenerator(h):
    last = random.randint(1,  2 ** (h - 1))
    return treeP(0, last,h)


def treeP(level, last,  h):
    level += 1
    if  level < h:
        if last > 0:
            if 2 **  level < last:
                right = treeP( level, h, last)
                left = treeP( level, h, 0)
                return [random.randint(0, 100), left, right]
            else:
                right = treeP( level, h, 0)
                left = treeP( level, h, last)
                return [random.randint(0, 100), left, right]
        if random.randint(0, 1) == 1:
            right = treeP(level, h, 0)
            left = treeP(level, h, 0)
            return [random.randint(0, 100), left, right]
        else:
            return None

    return [random.randint(0, 100), None, None]


def DFSgenerator(it):
    for x in it:
        if x is not None:
            if (isinstance(x, collections.Iterable) and
                    not isinstance(x, str)):
                for y in DFSgenerator(x):
                    yield y
            else:
                yield x


def BFSpGenerator(it, h):
    if h > 0:
        for x in it:
            if type(x) == list:
                for y in BFSpGenerator(x, (h - 1)):
                    yield y
    else:
        for x in it:
            if x is not None and type(x) != list:
                yield x


def BFSgenerator(it):
    a = 0
    b = True
    while b:
        b = False
        for x in BFSpGenerator(it, a):
            b = True
            yield x
        a += 1


tree = treeGenerator(5)
print(tree)
print(list(DFSgenerator(tree)))
print(list(BFSgenerator(tree)))