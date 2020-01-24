#!/usr/bin/env python3


def can_be_a_set_member_or_frozenset(item): # V Pythonu nemůže být prvkem množiny jiná množina 
    if isinstance(item, list)==True: # pokud je prvek list nemůže být podmnožinou
        return frozenset(item)
    if isinstance(item, set)==True: # pokud je prvek množina nemůže být podmnožinou
        return frozenset(item)
    if isinstance(item, dict)==True: # pokud je prvek slovník nemůže být podmnožinou
        return frozenset(item)
    return item

def all_subsets(lst):
    var=[[]] # vložení prázdného prvku
    for elem in lst: # projde každý prvek lst
        for sub_set in var: # přidá novou podmnožinu skládající se z podmnožiny na konec
            var=var+[list(sub_set)+[elem]]
    return var


def all_subsets_excl_empty(*arg, exclude_empty=""):
    lst=list(arg) #vytvoření listu z proměnných argumentů
    var=[[]] # vložení prázdného prvku
    for elem in lst: # projde každý prvek lst
        for sub_set in var: # přidá novou podmnožinu skládající se z podmnožiny na konec
            var=var+[list(sub_set)+[elem]]

    if exclude_empty==False: #pokud je False vrací s prázdným prvkem
        return var
    del var[0]
    return var

def test():
    assert can_be_a_set_member_or_frozenset(1) == 1
    assert can_be_a_set_member_or_frozenset((1,2)) == (1,2)
    assert can_be_a_set_member_or_frozenset([1,2]) == frozenset([1,2])
    assert all_subsets(['a', 'b', 'c']) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
    assert all_subsets_excl_empty('a', 'b', 'c') == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
    assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = True) == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
    assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = False) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]

if __name__ == '__main__':
    test()

