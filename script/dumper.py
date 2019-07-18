try:
    import serial
except ImportError:
    print("Import error! Install PySerial")
    exit()

s = serial.Serial()
s.baudrate = 38400
s.port = 'COM8'
s.open()

symbols = 0

if s.is_open:
    while True:
        res = s.read()
        print(res)
        symbols += 1
        if symbols > 100:
            s.close()
            print ("Port closed")
            break


