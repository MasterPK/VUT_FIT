#!/usr/bin/env python3

import collections

def add(self,other):
    """Soucet dvou polynomů, v pripade potreby rozsiruje"""
    len1=len(self)
    len2=len(other)
    if len1<len2:
        for i in range(0,len2-len1):
            self.append(0)
        len1=len2
    if len2<len1:
        for i in range(0,len1-len2):
            other.append(0)
        len2=len1
    if len1==len2:
        seznam=list()
        i=0
        for x in self:
            seznam.append(x+other[i])
            i=i+1
    return seznam

def nasobeni_polynomu(self,other):
    """Nasobeni dvou polynomu, v pripade potreby rozsiruje"""
    len1=len(self)
    len2=len(other)
    if len1<len2:
        for i in range(0,len2-len1):
            self.append(0)
        len1=len2
    if len2<len1:
        for i in range(0,len1-len2):
            other.append(0)
        len2=len1
    seznam = []
    i=0
    for i in range(len(self)): 
        seznam =add(seznam,nasobeni_jeden(self,other[i],i))
    return seznam

def nasobeni_jeden(self,other,i):
    """Nasobeni jednoho prvku, nasobi cislo a exponent"""
    seznam = [0]*i
    for x in self: 
        seznam.append(x*other)
    return seznam

def power(p,e):
    """Umocneni polynomu exponentem"""
    assert int(e) == e, "Mocnitel musi byt cele cislo"
    seznam = [1]
    i=0
    for i in range(e): 
        seznam = nasobeni_polynomu(seznam,p)
    return seznam

class Polynomial:
    """Trida polynomu, uchovava polynom jako list od nejmensiho prvku"""
    polynomial=list();

    def __init__(self, *argv, **kwargs):
        """Konstruktor tridy, zpracuje argumenty"""
        count_1=len(argv)
        count_2=len(kwargs)
        if count_1==0:
            if count_2==0:
                self.polynomial=list(0)
            else:
                self.polynomial=list()
                sort=collections.OrderedDict(sorted(kwargs.items(), key=lambda t: t[0]))
                i=0
                while True:
                    tmp=sort.get("x"+str(i))
                    if tmp is not None:
                        self.polynomial.append(tmp)
                    else:
                        self.polynomial.append(0)
                    i=i+1
                    if i==len(sort)+1:
                        break

        elif type(argv[0]) is list:
            self.polynomial=argv[0]
        elif type(argv) is tuple:
            self.polynomial=list(argv)
        for x in reversed(self.polynomial):
            if x==0:
               self.polynomial.pop()
            else:
                break

    def __str__(self):
        """Prevadi polynom ve tvrau listu na string, na zaklade podminek doplnuje operatory"""
        first=True
        i=len(self.polynomial)-1
        string=""
        empty=True
        for x in reversed(self.polynomial):
            operator=""
            cislo_str=str(abs(x))
            exponent="x^"+str(i)
            
            if i==1:
                exponent="x"
            elif i==0:
                exponent=""

            i=i-1

            if first:
                first=False
                if x>1:
                    operator=""
                elif x<-1:
                    operator="- "
                elif x==1:
                    cislo_str=""
                elif x==-1:
                    operator="- "
                    cislo_str=""
                elif x==0:
                    first=True
                    continue
            else:
                if x>1:
                    operator=" + "
                elif x<-1:
                    operator=" - "
                elif x==1:
                    cislo_str=""
                    if i==-1:
                        cislo_str="1"
                    operator=" + "
                elif x==-1:
                    operator=" - "
                    cislo_str=""
                    if i==-1:
                        cislo_str="1"
                elif x==0:
                    continue

            string=string+operator+cislo_str+exponent
            empty=False
        if empty:
            string="0"

        return string
    def __eq__(self, other):
        """Testuje jestli se 2 polynomy rovnaji, v pripade potreby rozsiruje"""
        len1=len(self.polynomial)
        len2=len(other.polynomial)
        if len1<len2:
            for i in range(0,len2-len1):
                self.polynomial.append(0)
            len1=len2
        if len2<len1:
            for i in range(0,len1-len2):
                other.polynomial.append(0)
            len2=len1
        if len1!=len2:
            return False
        else:
            i=0
            for x in self.polynomial:
                if x!=other.polynomial[i]:
                    return False
                i=i+1
         
        return True
    def __add__(self,other):
        """Secte 2 polynomy"""
        return Polynomial(add(self.polynomial,other.polynomial))
        
    
    def __pow__(self,e): 
        """Umocni polynom konstantou"""
        return Polynomial(power(self.polynomial,e))

    def derivative(self):
        """Derivace celého polynomu"""
        i=0
        seznam=list()
        for x in self.polynomial:
            seznam.append(x*i)
            i=i+1
        del(seznam[0])
        return Polynomial(seznam)

    def at_value(self,*cislo):
        """Vypocita hodnotu polynomu pro zadané x, pokud jsou zadane 2 cisla, vysledna hodnota je druhy vysledek - prvni"""
        if len(cislo)==1:
            try:
                cislo = float(cislo[0])
            except ValueError:
                return False
            vysledek = 0
            i = 0 
            for x in self.polynomial:
                vysledek = vysledek + (x*(cislo**i))
                i=i+1
        elif len(cislo)==2:
            try:
                cislo1 = float(cislo[0])
                cislo2 = float(cislo[1])
            except ValueError:
                return False
            vysledek1 = 0
            i = 0 
            for x in self.polynomial:
                vysledek1 = vysledek1 + (x*(cislo1**i))
                i=i+1

            vysledek2 = 0
            i = 0 
            for x in self.polynomial:
                vysledek2 = vysledek2 + (x*(cislo2**i))
                i=i+1
            vysledek=vysledek2-vysledek1
        else:
            return False

        return vysledek


def test():
    """Testovací funkce"""
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()
