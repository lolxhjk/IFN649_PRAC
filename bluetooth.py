import serial
import time
import string

ser=serial.Serial("/dev/rfcomm0",9600)
ser.write(str.encode('Star\r\n'))

while True:
  if ser.in_waiting>0:
  rawserial=ser.readline()
  cookedserial=rawserial.decode('utf-8').strip('\r\n')
  print(cookedserial)
