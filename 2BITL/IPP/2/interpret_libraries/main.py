# IPP project - interpret.py
# Author: Petr Křehlík (xkrehl04)
# Date: 10/4/2019
from .error import *
from .instructions import *

LoadedInstructions = {}
OrderedLoadedInstructions = {}


def Main(source,input):
    
    if source=="stdin" and input=="stdin":
        err_exit(10,"You dont specify source or input argument!")

    CreateInputSource(input)
    
    # Iterate through instructions
    for instruction in source:
        order=int(instruction.attrib["order"])
        opcode=instruction.attrib["opcode"]
        arg1=None
        arg2=None
        arg3=None
        for arg in instruction:
            if arg.tag=="arg1":
                arg1=arg
            elif arg.tag=="arg2":
                arg2=arg
            elif arg.tag=="arg3":
                arg3=arg
            else:
                err_exit(32,"Wrong arguments format!")
            

        instruction = Instruction(order,opcode,[arg1,arg2,arg3])
        LoadedInstructions[order]=instruction

    # Sort by key(order) and check order
    i=1
    for key in sorted(LoadedInstructions.keys()) :
        if int(key)==i:
            i=i+1
        else:
            err_exit(32,"Wrong instructions order!")
        OrderedLoadedInstructions[key]=LoadedInstructions[key]

    

    #Syntax check (labels)
    order=1
    max_order=len(OrderedLoadedInstructions)
    while order<=max_order:

        tmp=OrderedLoadedInstructions[order]

        tmp.CheckArgumentsSyntax()

        if tmp.opcode == "LABEL":
            if tmp.arg_list[0].text in Labels:
                err_exit(52,"Label alread exists!")
            else:
                Labels[tmp.arg_list[0].text]=tmp.order
            
        order+=1


    # Interpretation
    order=1
    max_order=len(OrderedLoadedInstructions)
    while order<=max_order:
        # Dynamic calling of functions in file instructions.py

        if OrderedLoadedInstructions[order].opcode == "LABEL":
            order+=1
            continue

        order = globals()[OrderedLoadedInstructions[order].opcode](OrderedLoadedInstructions[order])