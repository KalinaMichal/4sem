#!/usr/bin/python3

import collections

def flatten(it):
    for x in it:
        if (isinstance(x, collections.Iterable) and
            not isinstance(x, str)):
            for y in flatten(x):
                yield y
        else:
            yield x

l = [[1, 2, ["a", 4, "b", 5, 5, 5]], [4, 5, 6 ], 7, [[9, [123, [[123]]]], 10]]
print(list(flatten(l)))