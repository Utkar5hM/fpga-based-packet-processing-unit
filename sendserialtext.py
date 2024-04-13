import serial

ser = serial.Serial('/dev/ttyUSB1') 

ser.write(b"H")
ser.write(b"T")
ser.write(b"\n")