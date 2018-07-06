import serial
import time

def isBoardReady(ser, timeout = 10):
    timeTimeOut = time.time()
    while True:
        ser.write(b'?')
        inputByte = ser.read(1)
        if inputByte == b'r':
            return True
        if (time.time()-timeTimeOut) > 10:
            return False

def sendCommandWithInitCheck(command):
    try:
        ser = serial.Serial('COM4', 9600, timeout=0.2)
        ready = isBoardReady(ser)
        if not ready:
            return 0
        ser.write(bytes(command,'ASCII'))
        inputByte = ser.read(1)
        ser.close()
        return inputByte
    except:
        return 0