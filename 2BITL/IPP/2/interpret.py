# IPP project - interpret.py
# Author: Petr Křehlík (xkrehl04)
# Date: 10/4/2019

from interpret_libraries import arguments
from interpret_libraries import xml
from interpret_libraries import main

#Main command file
#Only calls important functions from subfiles

args=arguments.ProcessArgs()

source=xml.CreateXMLSource(args.source)
input=arguments.ReadInputFile(args.input)

main.Main(source,input)

