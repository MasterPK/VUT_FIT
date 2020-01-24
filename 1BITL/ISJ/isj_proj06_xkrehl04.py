#!/usr/bin/env python3

import itertools
import collections


def first_nonrepeating(vstup):
    """Najde prvni znak ktery se neopakuje"""

    if not isinstance(vstup, str):
        return None
    if vstup == '' or vstup == '':
        return None
    pole = dict()
    for x in vstup:
        if x.isspace():
            continue
        if x in pole:
            pole[x] = pole[x] + 1
        else:
            pole[x] = 1
    for x in pole:
        if pole[x] == 1:
            return x
    return None


def combine4(vstup, cil):
    """Kombinace 4 cisel a zakladnich operatoru tak aby se == cil"""

    if len(vstup) != 4:
        return None

    if cil == '':
        return None

    permutace_cisla = list(itertools.permutations(vstup))  # vsechyn varianty usporadani cisel
    variace_op = list(itertools.product(['+', '-', '*', '/'], repeat=3))  # vsechny varianty usporadani znamenek
    variace_zavorky=[
        # _A ? _B_ ? _C_ ? D_
        ["","","","","",""],
        ["(","",")","","",""],
        ["","","","(","",")"],
        ["","(","","",")",""],
        ["(","",")","(","",")"],
        ["(","","","",")",""],
        ["","(","","","",")"],
        ["((","",")","",")",""],
        ["(","(","","","))",""],
        ["","((","","",")",")"],
        ["","(","","(","","))"],
        ]
    moznosti = list()
    for x in permutace_cisla:  # prochazeni vsech variant
        x = list(x)
        for z in variace_op:
            for y in variace_zavorky:
                tmp = str(y[0]) + str(x[0]) + str(z[0]) + str(y[1]) \
                    + str(x[1]) + str(y[2]) + str(z[1]) + str(y[3]) \
                    + str(x[2]) + str(y[4]) + str(z[2]) + str(x[3]) \
                    + str(y[5])
                try:
                    cisla = eval(tmp)
                except ZeroDivisionError:
                    continue
                except:
                    return
                if cisla == cil:
                    moznosti.append(str(y[0]) + str(x[0]) + str(z[0])
                                    + str(y[1]) + str(x[1]) + str(y[2])
                                    + str(z[1]) + str(y[3]) + str(x[2])
                                    + str(y[4]) + str(z[2]) + str(x[3])
                                    + str(y[5]))
    return list(collections.OrderedDict.fromkeys(moznosti))


def test():
    """Testovaci funkce"""

    print(combine4([6, 6, 5, 2], 17))

if __name__ == '__main__':
    test()
