# IPP project - interpret.py
# Author: Petr Křehlík (xkrehl04)
# Date: 10/4/2019
from .error import *
import re
import sys

# Frames
GF={}
LF=None
TF=None

# Stacks
FramesStack=[]
DataStack=[]

# Otherss
Labels={}
CallStack=[]

# Pack to easy access
frames={"GF":GF,"LF":LF,"TF":TF,"FramesStack":FramesStack,"DataStack":DataStack,"Labels":Labels,"CallStack":CallStack}

# Global var to determine input
InputSource=None

# Set InputSource
def CreateInputSource(source):
    global InputSource
    InputSource=source

# List of all instructions and their parameters
ListOfInstructions= {
	"MOVE" : ["var","symb"],
	"CREATEFRAME" : [],
	"PUSHFRAME" : [],
	"POPFRAME" : [],
	"DEFVAR" : ["var"],
	"CALL" : ["label"],
	"RETURN" : [],
	"PUSHS" : ["symb"],
	"POPS" : ["var"],
	"ADD" : ["var","symb","symb"],
	"SUB" : ["var","symb","symb"],
	"MUL" : ["var","symb","symb"],
	"IDIV" : ["var","symb","symb"],
	"LT" : ["var","symb","symb"],
	"GT" : ["var","symb","symb"],
	"EQ" : ["var","symb","symb"],
	"AND" : ["var","symb","symb"],
	"OR" : ["var","symb","symb"],
	"NOT" : ["var","symb"],
	"INT2CHAR" : ["var","symb"],
	"STRI2INT" : ["var","symb","symb"],
	"READ" : ["var","type"],
	"WRITE" : ["symb"],
	"CONCAT" : ["var","symb","symb"],
	"STRLEN" : ["var","symb"],
	"GETCHAR" : ["var","symb","symb"],
	"SETCHAR" : ["var","symb","symb"],
	"TYPE" : ["var","symb"],
	"LABEL" : ["label"],
	"JUMP" : ["label"],
	"JUMPIFEQ" : ["label","symb","symb"],
	"JUMPIFNEQ" : ["label","symb","symb"],
	"EXIT" : ["symb"],
	"DPRINT" : ["symb"],
	"BREAK" : []
    }

# Important class that holds everything about instruction such as: order, opcode, ...
class Instruction:
    def __init__(self, order, opcode, arg_list):
        self.order = order
        self.opcode = opcode.upper()
        self.arg_list=arg_list
        self.CheckOpcodeSyntax()

    def __str__(self):

        text="Order:{0}, Opcode:{1}".format(self.order,self.opcode)
        i=1
        for x in self.arg_list:
            if x is not None:
                text=text+", Arg{0}:".format(i)+x.text
            i=i+1
        return text


    def CheckOpcodeSyntax(self):
        if self.opcode not in ListOfInstructions:
            err_exit(32,"Unrecognized operand code!")

    def CheckArgumentsSyntax(self):
        base_arg_list=ListOfInstructions[self.opcode]
        if len(base_arg_list)!=self.GetArgumentsCount():
            err_exit(32,"Wrong count of arguments!")

        
        i=0
        arg_list=self.arg_list

        for base_arg in base_arg_list:
            if base_arg is None:
                if arg_list[i] is not None:
                    err_exit(32,"Wrong arguments!")
                else:
                    continue
            
            if arg_list[i] is None:
                err_exit(32,"Wrong arguments!")
            
            
            if base_arg == "var":
                match=re.match(r"^(GF|LF|TF)@([a-zA-Z]|[_!?*%$&-]\w)([a-zA-Z0-9]|[_!?*%$&-])*$",arg_list[i].text)
                if arg_list[i].attrib["type"]!="var" or match is None:
                    err_exit(32,"Wrong argument format!")

            elif base_arg == "symb":
                
                if arg_list[i].attrib["type"]=="string" and re.match(r'^([^\s#\\]|\\[0-9]{3})*$',arg_list[i].text) is not None:
                    n=0
                    a=arg_list[i].text
                    while n<len(a):
                        if repr(a[n])[1]=='\\':
                            escape=chr(int(a[n+1:n+4]))
                            a=a.replace(a[n:n+4],escape)
                            n+=4
                            continue
                        n+=1
                    arg_list[i].text=a
                elif arg_list[i].attrib["type"]=="bool" and re.match(r"^(true|false)$",arg_list[i].text) is not None:
                    pass
                elif arg_list[i].attrib["type"]=="int" and re.match(r"^[+|-]?[0-9]+$",arg_list[i].text) is not None:
                    pass
                elif arg_list[i].attrib["type"]=="nil" and re.match(r"^nil$",arg_list[i].text) is not None:
                    pass
                elif arg_list[i].attrib["type"]=="var" and re.match(r"^(GF|LF|TF)@([a-zA-Z]|[_!?*%$&-]\w)([a-zA-Z0-9]|[_!?*%$&-])*$",arg_list[i].text) is not None:
                    pass
                else:
                    err_exit(32,"Wrong argument format!")

            elif base_arg == "type":
                match=re.match(r"^(int|bool|string)$",arg_list[i].text)
                if match is None:
                    err_exit(32,"Wrong argument format!")
            
            elif base_arg == "label":
                match=re.match(r"^([a-zA-Z]|[_!?*%$&-]\w)([a-zA-Z0-9]|[_!?*%$&-])*",arg_list[i].text)
                if match is None:
                    err_exit(32,"Wrong argument format!")
               

            i=i+1

    def GetArgumentsCount(self):
        i=0
        for x in self.arg_list:
            if x is not None:
                i=i+1
        return i

    def GetNextInstructionOrder(self):
        return self.order+1

# Converting string to correct type
def TypeConverter(var):

    if var=="nil":
        return None
    
    if var=="false" or var=="False" or var==False:
        return bool(False)
    elif var=="true" or var=="True" or var==True:
        return bool(True)

    try:
        return int(var)
    except:
        pass

    try:
        return str(var)
    except:
        err_exit(32,"Cant determine type!")

# Next is functions of each instruction

def MOVE(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")
        
    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")

        frames[arg1[0]][arg1[1]]=TypeConverter(frames[arg2[0]][arg2[1]])
    else:
        frames[arg1[0]][arg1[1]]=TypeConverter(instruction.arg_list[1].text)

    return instruction.GetNextInstructionOrder()

def CREATEFRAME(*args):
    instruction=args[0]
    global frames
    frames["TF"]={}

    return instruction.GetNextInstructionOrder()

def PUSHFRAME(*args):
    instruction=args[0]

    if frames["TF"] == None:
        err_exit(55,"Temporary frame does not exist!")

    frames["FramesStack"].append(frames["TF"])
    if len(frames["FramesStack"]) ==1:
        frames["LF"]=frames["FramesStack"][0]
    else:
        if len(frames["FramesStack"]) !=0:
            frames["LF"]=frames["FramesStack"][-1]
        else:
            frames["LF"]=None

    frames["TF"]=None

    return instruction.GetNextInstructionOrder()

def POPFRAME(*args):
    instruction=args[0]
    

    if frames["TF"] == None:
        err_exit(55,"Temporary frame does not exist!")

    if frames["LF"] == None:
        err_exit(55,"Local frame does not exist!")

    try:
        frames["TF"]=frames["FramesStack"].pop()
        if len(frames["FramesStack"]) !=0:
            frames["LF"]=frames["FramesStack"][-1]
        else:
            frames["LF"]=None
    except IndexError:
        err_exit(55,"No local frame exists!")

    return instruction.GetNextInstructionOrder()

def DEFVAR(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] in frames[arg1[0]]:
        err_exit(52,"Variable already declared!")
    
    frames[arg1[0]].update({arg1[1]:None})

    return instruction.GetNextInstructionOrder()

def CALL(*args): #TODO
    instruction=args[0]
    arg1=instruction.arg_list[0].text

    if arg1 not in Labels:
        err_exit(52,"Label doesnt exist!")

    CallStack.append(instruction.order+1)
    
    return Labels[arg1]

def RETURN(*args): #TODO
    instruction=args[0]
    
    if len(CallStack) == 0:
        err_exit(55,"Call stack is empty!")
    
    return CallStack.pop()

def PUSHS(*args): #TODO
    instruction=args[0]
    

    if instruction.arg_list[0].attrib["type"]=="var":
        arg1=instruction.arg_list[0].text.split("@")

        if frames[arg1[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg1[1] not in frames[arg1[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg1[0]][arg1[1]] == None:
            err_exit(56,"Variable is not assigned to value!")

        DataStack.append(frames[arg1[0]][arg1[1]])
    else:
        DataStack.append(TypeConverter(instruction.arg_list[0].text))
    
    return instruction.GetNextInstructionOrder()

def POPS(*args): #TODO
    instruction=args[0]
    
    if len(DataStack) == 0:
        err_exit(56,"DataStack is empty!")
    
    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")
        
    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    frames[arg1[0]][arg1[1]]=DataStack.pop()

    return instruction.GetNextInstructionOrder()

def ADD(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]
    else:
        if instruction.arg_list[1].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=frames[arg3[0]][arg3[1]]
    else:
        if instruction.arg_list[2].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)

    print(arg2_value)
    print(arg3_value)

    if type(arg2_value) is not int or type(arg3_value) is not int:
        err_exit(53,"Operands must be same type and integers!")

    frames[arg1[0]][arg1[1]]=arg2_value+arg3_value

    return instruction.GetNextInstructionOrder()

def SUB(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]
    else:
        if instruction.arg_list[1].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=frames[arg3[0]][arg3[1]]
    else:
        if instruction.arg_list[2].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not int or type(arg3_value) is not int:
        err_exit(53,"Operands must be same type and integers!")

    frames[arg1[0]][arg1[1]]=arg3_value-arg2_value

    return instruction.GetNextInstructionOrder()

def MUL(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]
    else:
        if instruction.arg_list[1].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=frames[arg3[0]][arg3[1]]
    else:
        if instruction.arg_list[2].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not int or type(arg3_value) is not int:
        err_exit(53,"Operands must be same type and integers!")

    frames[arg1[0]][arg1[1]]=arg2_value*arg3_value

    return instruction.GetNextInstructionOrder()

def IDIV(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]
    else:
        if instruction.arg_list[1].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=frames[arg3[0]][arg3[1]]
    else:
        if instruction.arg_list[2].attrib["type"]!="int":
            err_exit(53,"Operands must be integers!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not int or type(arg3_value) is not int:
        err_exit(53,"Operands must be same type and integers!")

    if arg3_value==0:
        err_exit(57,"Zero division exception!")

    frames[arg1[0]][arg1[1]]=arg2_value//arg3_value

    return instruction.GetNextInstructionOrder()

def LT(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]
    else:
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        arg3_value=TypeConverter(instruction.arg_list[2].text)

    if instruction.arg_list[1].attrib["type"]=="nil":
        err_exit(53,"Cant compare nil in LT!")

    if type(TypeConverter(arg2_value)) is not type(TypeConverter(arg3_value)):
        err_exit(53,"Operands must be same type!")

    frames[arg1[0]][arg1[1]]=arg2_value<arg3_value

    return instruction.GetNextInstructionOrder()

def GT(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]
    else:
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        arg3_value=TypeConverter(instruction.arg_list[2].text)

    if instruction.arg_list[1].attrib["type"]=="nil":
        err_exit(53,"Cant compare nil in LT!")

    if type(TypeConverter(arg2_value)) is not type(TypeConverter(arg3_value)):
        err_exit(53,"Operands must be same type!")

    frames[arg1[0]][arg1[1]]=arg2_value>arg3_value

    return instruction.GetNextInstructionOrder()

def EQ(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]
    else:
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        arg3_value=TypeConverter(instruction.arg_list[2].text)

    if type(arg2_value) is not type(arg3_value):
        err_exit(53,"Operands must be same type!")

    frames[arg1[0]][arg1[1]]=arg2_value==arg3_value

    return instruction.GetNextInstructionOrder()

def AND(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]

    else:
        if instruction.arg_list[1].attrib["type"]!="bool":
            err_exit(53,"Operand 1 must be bool!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        if instruction.arg_list[2].attrib["type"]!="bool":
            err_exit(53,"Operand 2 must be bool!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not bool or type(arg3_value) is not bool:
        err_exit(53,"Operands must be bool!")

    if type(arg2_value) is not type(arg3_value):
        err_exit(53,"Operands must be same type and bool!")

    frames[arg1[0]][arg1[1]]=bool(arg2_value and arg3_value)
    

    return instruction.GetNextInstructionOrder()

def OR(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]

    else:
        if instruction.arg_list[1].attrib["type"]!="bool":
            err_exit(53,"Operand 1 must be bool!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        if instruction.arg_list[2].attrib["type"]!="bool":
            err_exit(53,"Operand 2 must be bool!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not bool or type(arg3_value) is not bool:
        err_exit(53,"Operands must be bool!")

    if type(arg2_value) is not type(arg3_value):
        err_exit(53,"Operands must be same type and bool!")


    frames[arg1[0]][arg1[1]]=arg2_value or arg3_value
    

    return instruction.GetNextInstructionOrder()

def NOT(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]

    else:
        if instruction.arg_list[1].attrib["type"]!="bool":
            err_exit(53,"Operand must be bool!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if type(arg2_value) is not bool:
        err_exit(53,"Operand must be bool!")


    frames[arg1[0]][arg1[1]]=not arg2_value
    

    return instruction.GetNextInstructionOrder()

def INT2CHAR(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]

    else:
        if instruction.arg_list[1].attrib["type"]!="int":
            err_exit(53,"Operand must be int!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if type(arg2_value) is not int:
        err_exit(53,"Operand must be int!")

    if arg2_value<0 or arg2_value>255:
        err_exit(58,"Convertion from int to char failed! Value out of range!")

    try:
        frames[arg1[0]][arg1[1]]=chr(arg2_value)
    except ValueError:
        err_exit(58,"Convertion from int to char failed! Value out of range!")

    return instruction.GetNextInstructionOrder()

def STRI2INT(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=frames[arg2[0]][arg2[1]]

    else:
        if instruction.arg_list[1].attrib["type"]!="string":
            err_exit(53,"Operand 1 must be string!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        if instruction.arg_list[2].attrib["type"]!="int":
            err_exit(53,"Operand 2 must be integer!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not str or type(arg3_value) is not int:
        err_exit(53,"Operands must be string and integer!")

    try:
        frames[arg1[0]][arg1[1]]=ord(arg2_value[arg3_value])
    except IndexError:
        err_exit(58,"Index out of range!")

    return instruction.GetNextInstructionOrder()

def READ(*args): #TODO
    instruction=args[0]
    
    arg1=instruction.arg_list[0].text.split("@")
    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(52,"Variable does not exist!")

    if InputSource=="stdin":
        var=input()
    else:
        var=InputSource.readline()

    var=var.rstrip("\n")

    type=instruction.arg_list[1].text
    
    if type == "int":
        try:
            var=int(var)
        except:
            var=0
    elif type=="string":
        try:
            var=str(var)
        except:
            var=""
    elif type=="bool":
        if var=="true" or var=="True" or var==True:
            var=True
        else:
            var=False

    frames[arg1[0]][arg1[1]]=var
    
    return instruction.GetNextInstructionOrder()

def WRITE(*args):
    instruction=args[0]
    

    if instruction.arg_list[0].attrib["type"]=="var":
        arg1=instruction.arg_list[0].text.split("@")
        
        if frames[arg1[0]] == None:
            err_exit(55,"Frame does not exist!")

        if arg1[1] not in frames[arg1[0]]:
            err_exit(52,"Variable does not exist!")

        if frames[arg1[0]][arg1[1]] == None:
            err_exit(56,"Variable is not assigned to value!")

        if type(frames[arg1[0]][arg1[1]]) is bool:
            if frames[arg1[0]][arg1[1]]:
                print("true",end="")
            else:
                print("false",end="")
        else:
            print(frames[arg1[0]][arg1[1]],end="")

    elif type(instruction.arg_list[0].text) is bool:
        if instruction.arg_list[0].text:
            print("true",end="")
        else:
            print("false",end="")

    else:
        print(instruction.arg_list[0].text,end="")
    
    return instruction.GetNextInstructionOrder()

def CONCAT(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=TypeConverter(frames[arg2[0]][arg2[1]])

    else:
        if instruction.arg_list[1].attrib["type"]!="string":
            err_exit(53,"Operand 1 must be string!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        if instruction.arg_list[2].attrib["type"]!="string":
            err_exit(53,"Operand 2 must be string!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not str or type(arg3_value) is not str:
        err_exit(53,"Operands must be string!")

    frames[arg1[0]][arg1[1]]=arg2_value+arg3_value

    return instruction.GetNextInstructionOrder()

def STRLEN(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=TypeConverter(frames[arg2[0]][arg2[1]])

    else:
        if instruction.arg_list[1].attrib["type"]!="string":
            err_exit(53,"Operand must be string!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if type(arg2_value) is not str:
        err_exit(53,"Operand must be string!")

    
    frames[arg1[0]][arg1[1]]=len(arg2_value)

    return instruction.GetNextInstructionOrder()

def GETCHAR(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=TypeConverter(frames[arg2[0]][arg2[1]])

    else:
        if instruction.arg_list[1].attrib["type"]!="string":
            err_exit(53,"Operand 1 must be string!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        if instruction.arg_list[2].attrib["type"]!="int":
            err_exit(53,"Operand 2 must be integer!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg2_value) is not str or type(arg3_value) is not int:
        err_exit(53,"Operands must be string and integer!")

    try:
        frames[arg1[0]][arg1[1]]=str(arg2_value[arg3_value])
    except IndexError:
        err_exit(58,"Index out of range!")

    return instruction.GetNextInstructionOrder()

def SETCHAR(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if frames[arg1[0]][arg1[1]] == None:
        err_exit(56,"Variable is not assigned to value!")
    arg1_value=TypeConverter(frames[arg1[0]][arg1[1]])

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=TypeConverter(frames[arg2[0]][arg2[1]])

    else:
        if instruction.arg_list[1].attrib["type"]!="int":
            err_exit(53,"Operand 2 must be integer!")
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        if instruction.arg_list[2].attrib["type"]!="string":
            err_exit(53,"Operand 3 must be string!")
        arg3_value=TypeConverter(instruction.arg_list[2].text)


    if type(arg1_value) is not str or type(arg2_value) is not int or type(arg3_value) is not str:
        err_exit(53,"Operands must be string, integer, string!")

    try:
        frames[arg1[0]][arg1[1]]=frames[arg1[0]][arg1[1]][:arg2_value]+str(arg3_value)+frames[arg1[0]][arg1[1]][arg2_value+1:]
    except IndexError:
        err_exit(58,"Index out of range!")

    return instruction.GetNextInstructionOrder()

def TYPE(*args):
    instruction=args[0]
    

    arg1=instruction.arg_list[0].text.split("@")

    if frames[arg1[0]] == None:
        err_exit(55,"Frame does not exist!")

    if arg1[1] not in frames[arg1[0]]:
        err_exit(54,"Variable does not exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            arg2_value=None
        else:
            arg2_value=TypeConverter(frames[arg2[0]][arg2[1]])

    else:
        arg2_value=TypeConverter(instruction.arg_list[1].text)
    
    if type(arg2_value) is str:
        frames[arg1[0]][arg1[1]]="string"
    elif type(arg2_value) is int:
        frames[arg1[0]][arg1[1]]="int"
    elif type(arg2_value) is bool:
        frames[arg1[0]][arg1[1]]="bool"
    elif arg2_value == None:
        frames[arg1[0]][arg1[1]]=""

    return instruction.GetNextInstructionOrder()

def LABEL(*args): #TODO Kontrolu provádět před interpretací?
    instruction=args[0]
    

    if instruction.arg_list[0].text in frames["Labels"]:
        if instruction.order > frames["Labels"][instruction.arg_list[0].text]:
            err_exit(52,"Label already exists!")
        else:
            return instruction.GetNextInstructionOrder()
    
    frames["Labels"][instruction.arg_list[0].text]=instruction.order

    return instruction.GetNextInstructionOrder()

def JUMP(*args):
    instruction=args[0]
    

    if instruction.arg_list[0].text not in frames["Labels"]:
        err_exit(52,"Label doesnt exist!")

    return frames["Labels"][instruction.arg_list[0].text]

def JUMPIFEQ(*args):
    instruction=args[0]
    

    if instruction.arg_list[0].text not in frames["Labels"]:
        err_exit(52,"Label doesnt exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=TypeConverter(frames[arg2[0]][arg2[1]])
    else:
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        arg3_value=TypeConverter(instruction.arg_list[2].text)

    if type(arg2_value) is not type(arg3_value):
        err_exit(53,"Operands must be same type!")

    if arg2_value == arg3_value:
        return frames["Labels"][instruction.arg_list[0].text]
    else:
        return instruction.GetNextInstructionOrder()
        
def JUMPIFNEQ(*args):
    instruction=args[0]
    

    if instruction.arg_list[0].text not in frames["Labels"]:
        err_exit(52,"Label doesnt exist!")

    if instruction.arg_list[1].attrib["type"]=="var":
        arg2=instruction.arg_list[1].text.split("@")

        if frames[arg2[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg2[1] not in frames[arg2[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg2[0]][arg2[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg2_value=TypeConverter(frames[arg2[0]][arg2[1]])
    else:
        arg2_value=TypeConverter(instruction.arg_list[1].text)

    if instruction.arg_list[2].attrib["type"]=="var":
        arg3=instruction.arg_list[2].text.split("@")

        if frames[arg3[0]] == None:
            err_exit(55,"Frame does not exist!")
            
        if arg3[1] not in frames[arg3[0]]:
            err_exit(54,"Variable does not exist!")

        if frames[arg3[0]][arg3[1]] == None:
            err_exit(56,"Variable is not assigned to value!")
        arg3_value=TypeConverter(frames[arg3[0]][arg3[1]])
    else:
        arg3_value=TypeConverter(instruction.arg_list[2].text)

    if type(arg2_value) is not type(arg3_value):
        err_exit(53,"Operands must be same type!")

    if arg2_value != arg3_value:
        return frames["Labels"][instruction.arg_list[0].text]
    else:
        return instruction.GetNextInstructionOrder()

def EXIT(*args):
    instruction=args[0]
    
    code=TypeConverter(instruction.arg_list[0].text)

    if type(code) is not int:
        err_exit(57,"Operand must be integer!")

    exit(code)

def DPRINT(*args):
    instruction=args[0]
    

    if instruction.arg_list[0].attrib["type"]=="var":
        arg1=instruction.arg_list[0].text.split("@")
        
        if arg1[1] not in frames[arg1[0]]:
            err_exit(52,"Variable does not exist!")

        if frames[arg1[0]][arg1[1]] == None:
            err_exit(56,"Variable is not assigned to value!")

        if type(frames[arg1[0]][arg1[1]]) is bool:
            if frames[arg1[0]][arg1[1]]:
                print("true",end="",file=sys.stderr)
            else:
                print("false",end="",file=sys.stderr)
        else:
            print(frames[arg1[0]][arg1[1]],end="",file=sys.stderr)

    elif type(instruction.arg_list[0].text) is bool:
        if instruction.arg_list[0].text:
            print("true",end="",file=sys.stderr)
        else:
            print("false",end="",file=sys.stderr)

    else:
        print(instruction.arg_list[0].text,end="",file=sys.stderr)
    
    return instruction.GetNextInstructionOrder()

def BREAK(*args):
    instruction=args[0]
    

    print("\n\nBREAK:",file=sys.stderr)
    print("Instruction order:"+str(instruction.order),file=sys.stderr)
    print("GlobalFrame:",file=sys.stderr)
    print(frames["GF"],file=sys.stderr)
    print("LocalFrame:",file=sys.stderr)
    print(frames["LF"],file=sys.stderr)
    print("TemporaryFrame:",file=sys.stderr)
    print(frames["TF"],file=sys.stderr)
    print("FramesStack:",file=sys.stderr)
    print(frames["FramesStack"],file=sys.stderr)
    print("DataStack:",file=sys.stderr)
    print(frames["DataStack"],file=sys.stderr)
    print("Labels:",file=sys.stderr)
    print(frames["Labels"],file=sys.stderr)
    print("CallStack:",file=sys.stderr)
    print(frames["CallStack"],file=sys.stderr)
    print("End of BREAK\n",file=sys.stderr)

    return instruction.GetNextInstructionOrder()


    