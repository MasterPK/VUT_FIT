#!/usr/bin/python3
# IPK Project 1
# Author: Petr Křehlík (xkrehl04)
# Date: 16.3.2019

from socket import *
import datetime
from time import sleep
import sys
try:
	port=int(sys.argv[1])
except ValueError:
	raise
except IndexError:
    raise

# Init server socket
serverPort = port
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.bind(('localhost',serverPort))
serverSocket.listen(1)
print("Server is running")

# Get hostname
hostname=gethostname() 

# Get CPU name
cpu_name=""
with open('/proc/cpuinfo') as file:
    for line in file:
        tmp=line.split(":")
        if tmp[0]=="model name\t":
            cpu_name=tmp[1].strip()
            break
file.close()

# Specify requests to find
hostname_find = "GET /hostname"
cpu_find = "GET /cpu-name"
load_find = "GET /load"
load_refresh_find = "GET /load?refresh="

# Default HTTP header
default_msg = """HTTP/1.1 200 OK
Date: {0} GMT
Server: Python Socket
Content-Type: text/plain
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"))

# JSON header
json_msg = """HTTP/1.1 200 OK
Date: {0} GMT
Server: Python Socket
Content-Type: application/json
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"))


# Function detects error code, then change message
def msg_err(msg,type): 
    if msg == "err:400":
        return """HTTP/1.1 400 Bad Request
Date: {0} GMT
Server: Python Socket
Content-Type: {1}
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"),type)

    if msg == "err:404":
        return """HTTP/1.1 404 Not found
Date: {0} GMT
Server: Python Socket
Content-Type: {1}
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"),type)

    if msg == "err:405":
        return """HTTP/1.1 405 Method Not Allowed
Date: {0} GMT
Server: Python Socket
Content-Type: {1}
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"),type)

    if msg == "err:406":
        return """HTTP/1.1 406 Not Acceptable
Date: {0} GMT
Server: Python Socket
Content-Type: {1}
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"),type)

    if msg == "err:505":
        return """HTTP/1.1 505 HTTP Version Not Supported
Date: {0} GMT
Server: Python Socket
Content-Type: {1}
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"),type)

    return msg

# Based on link from task
# Load data from /proc/stat two times and do some calculations
def cpu_load():
    with open('/proc/stat') as file:
        first_line1 = file.readline().strip().split(" ")
        prevuser=int(first_line1[2]);
        prevnice=int(first_line1[3]);
        prevsystem=int(first_line1[4]);
        previdle=int(first_line1[5]);
        previowait=int(first_line1[6]);
        previrq=int(first_line1[7]);
        prevsoftirq=int(first_line1[8]);
        prevsteal=int(first_line1[9]);
    file.close()
    sleep(1)
    with open('/proc/stat') as file:
        first_line2 = file.readline().strip().split(" ")
        user=int(first_line2[2]);
        nice=int(first_line2[3]);
        system=int(first_line2[4]);
        idle=int(first_line2[5]);
        iowait=int(first_line2[6]);
        irq=int(first_line2[7]);
        softirq=int(first_line2[8]);
        steal=int(first_line2[9]);
    file.close()

    PrevIdle = previdle + previowait
    Idle = idle + iowait

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    NonIdle = user + nice + system + irq + softirq + steal

    PrevTotal = PrevIdle + PrevNonIdle
    Total = Idle + NonIdle

    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    CPU_Percentage = (totald - idled)/totald

    return round(CPU_Percentage*100,2);

def json_request(data,msg): # Process JSON request
    print("json");
    msg=msg+"{"
    if data.find(hostname_find) >= 0:
        msg = msg + "\"hostname\":\"" + hostname + "\""

    elif data.find(cpu_find) >= 0:
        msg = msg + "\"cpu-name\":\"" + cpu_name + "\""

    elif data.find(load_refresh_find) >=0:
        # Parse number from refresh
        pos_refresh= data.find("refresh=")+8
        pos_space=data.find(" ",pos_refresh)
        try:
            pos=int(data[pos_refresh:pos_space])
            msg= """HTTP/1.1 200 OK
Date: {0} GMT
Server: Python Socket
Content-Type: application/json
Refresh: {1}
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"),pos)
            msg= msg + "{\"cpu-load\":\"" + str(cpu_load())+"%" + "\""

        except ValueError:
            return "err:400"
    elif data.find(load_find) >= 0:
            msg = msg + "\"cpu-load\":\"" + str(cpu_load())+"%" + "\""
    else:
        return "err:404"
    return msg+"}"
    
def plain_request(data,msg): #Process plain request
    print("plain");
    if data.find(hostname_find) >= 0:
            msg = msg + hostname

    elif data.find(cpu_find) >= 0:
        msg = msg + cpu_name

    elif data.find(load_refresh_find) >=0:
        # Parse number from refresh
        pos_refresh= data.find("refresh=")+8
        pos_space=data.find(" ",pos_refresh)
        try:
            pos=int(data[pos_refresh:pos_space])
            msg= """HTTP/1.1 200 OK
Date: {0} GMT
Server: Python Socket
Content-Type: text/plain
Refresh: {1}
Connection: close

""".format(datetime.datetime.now().strftime("%a, %d %b %Y %H:%M:%S"),pos)
            msg= msg + str(cpu_load())+"%"

        except ValueError:
            return "err:400"
    elif data.find(load_find) >= 0:
            msg = msg + str(cpu_load())+"%"
    else:
        return "err:404"
    return msg

while True:
    try:
        conn, addr = serverSocket.accept()  # Wait for connection
        print("Connection is open")
        data = conn.recv(1024) # Recieve data
        data = data.decode() # Decode from byte array to string
        print (data)
        accept_start=data.find("Accept:")+7
        accept_end=data.find("\n",accept_start)
        accept=data[accept_start:accept_end]

        if data.find("POST /") >=0:
            type="text/plain"
            msg="err:405"
        elif data.find("HTTP/1.1") <0:
        	type="text/plain"
        	msg="err:505"
        else:
	        if accept.find("application/json")>=0:
	            type="application/json"
	            msg=json_request(data,json_msg)
	        elif accept.find("text/plain")>=0:
	            type="text/plain"
	            msg=plain_request(data,default_msg)
	        elif accept.find("*/*")>=0:
	            type="text/plain"
	            msg=plain_request(data,default_msg)
	        else:
	            type="text/plain"
	            msg="err:406"
        
        
        msg=msg_err(msg,type)

        

        conn.send(msg.encode()) # Send encoded message back
        print("Close connection")
        conn.close() # Close connection

    except KeyboardInterrupt:
        try:
            conn.close()
        except NameError:
            pass
        serverSocket.close()
        print("Server shutdown correctly!")
        break
    



