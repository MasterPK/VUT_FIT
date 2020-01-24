# IPP project - interpret.py
# Author: Petr Křehlík (xkrehl04)
# Date: 10/4/2019
import sys
import argparse
from .error import *


#Function to sort out arguments, return list of them
def ProcessArgs():

    if len(sys.argv) < 2:
        err_exit(10,"Missing arguments!")

    parser = argparse.ArgumentParser(description='Program načte XML reprezentaci programu ze zadaného souboru a tento program s využitím standardního vstupu a výstupu interpretuje.')
    parser.add_argument("--source", help="source file with XML input code")
    parser.add_argument("--input", help="file with input data to interpret")
    args = parser.parse_args()
    
    if args.input is None:
        if args.source is None:
            err_exit(10,"Missing arguments!")
        else:
            args.input="stdin"

    if args.source is None:
        if args.input is not None:
            args.source="stdin"
            
    return args

#Determine input file as stdin or file object
def ReadInputFile(file):
    if file=="stdin":
        return "stdin"
    try:
        return open(file,"r")
    except:
        err_exit(11,"Opening source file failed! Check if file exists!")