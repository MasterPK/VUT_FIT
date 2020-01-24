# IPP project - interpret.py
# Author: Petr Křehlík (xkrehl04)
# Date: 10/4/2019
import xml.etree.ElementTree as ET
from .error import *
import sys

# read XML file and return object to iteration
def CreateXMLSource(file):
    try:
        if file=="stdin":
            file=sys.stdin.read()
        else:
            file=open(file,"r").read()
    except:
        err_exit(11,"Opening source code failed! Check if file exists!")

    try:
        root = ET.fromstring(file)
    except:
        err_exit(31,"Opening source code failed! Check if file is in correct format!")

    return root