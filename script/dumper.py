try:
    import serial
except ImportError:
    print("Import error! Install PySerial")
    exit()

import signal
import sys
import time

workWhileTrue = True

outFile = None 

def openPort(s):
    s.baudrate = 38400
    s.port = 'COM8'
    s.open()
    if s.is_open:
        return True
    return False

def readData(s):
    symbols = 0
    if s.is_open:
        while workWhileTrue:
            if s.in_waiting:
                res = s.read()
                print(res)
                if type(outFile) != None:
                    outFile.write(res)
                    symbols += 1
        #print("%d bytes dumped", symbols)



def Exit_gracefully(signal, frame):
    workWhileTrue = False
    if type(outFile) != None:
        outFile.close()
    sys.exit(0)

if __name__ == '__main__':
    signal.signal(signal.SIGINT, Exit_gracefully)
    s = serial.Serial()
    openPort(s)
    filename =  time.strftime("Dump-%Y%m%d-%H%M%S.bin")
    outFile = open(filename, "wb")
    readData(s)

