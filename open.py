import serial
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=2)
#ser.open()
ser.read()
ser.write(b'open')
ser.close()
