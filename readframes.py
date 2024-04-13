import serial
from scapy.all import Ether, sendp

ser = serial.Serial('/dev/ttyUSB1')  # replace with your serial port
while True:
    try:
        frame = ser.readline()[:-1]
        # frame = ser.read(1)
        packet = Ether(frame)
        # print(frame)
        print(packet.summary())
        sendp(frame, iface="vethmp0")
    except Exception as e:
        continue