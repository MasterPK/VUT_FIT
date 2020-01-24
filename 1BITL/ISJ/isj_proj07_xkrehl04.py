#!/usr/bin/env python3

import math
import os

def ordered_merge(*args,selector=[]):
    """Vraci generator"""
    try:
        slovnik={key:iter(value) for key, value in enumerate(args)}
        for key in selector:
            yield next(slovnik.get(key))
    except NameError:
        return None

class TooManyCallsError(Exception):
    """Vyjimka"""
    pass

def limit_calls(max_calls=2,error_message_tail="function \"pyth\" - called too often"):
    """Dekorator, ktery limituje pocty volani"""
    def _limit_calls(funkce):
        def __limit_calls(*args,**kwargs):
            try:
                if __limit_calls.calls<max_calls:
                    ret = funkce(*args,**kwargs)
                    __limit_calls.calls += 1
                else:
                    raise TooManyCallsError(error_message_tail)
            except NameError: 
                pass
            return ret
        __limit_calls.calls = 0
        return __limit_calls    return _limit_calls

@limit_calls()
def pyth(a,b):
    """Mat vypocet"""
    c = math.sqrt(a**2 + b ** 2)
    return c

class Log:
    """Trida pro zapis logu do txt souboru"""
    filename=""
    def __init__(self,filename):
        """Inicializuje tridu"""
        self.filename=filename
    def __enter__(self):
        """Pri vstupu do with smaze soubor"""
        try:
            os.remove(self.filename)
        except OSError:
            pass
        with open(self.filename, "a") as myfile:
            myfile.write("Begin")
            myfile.write("\n")
        return self
    def __exit__(self, type, value, traceback):
       """Pri vystupu zapise end"""
       with open(self.filename, "a") as myfile:
            myfile.write("End")
            myfile.write("\n")

    def logging(self,str):
        """loguje zaznamy na konec souboru"""
        with open(self.filename, "a") as myfile:
            myfile.write(str)
            myfile.write("\n")

def test():
    """Testovaci funkce"""
    print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11), selector = [2,3,0,1,3,1])))
    print([],list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11))))
    print(pyth(3,4))
    print(pyth(6,8))

    with Log('mylog.txt') as logfile:
        logfile.logging('Test1')
        logfile.logging('Test2')
        a = 1/0
        logfile.logging('Test3')

if __name__ == '__main__':
    test()
