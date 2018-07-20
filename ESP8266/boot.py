import network
import machine
from machine import Pin, I2C
import socket
import time


IP = '192.168.1.42'
puerto = 30000
ssid = 'Telefonica'
contrasena = '20582589'
pinSCL = 14
pinSDA = 12

sta_if = network.WLAN(network.STA_IF)
ap_if = network.WLAN(network.AP_IF)
sta_if.active(True)
ap_if.active(False)
sta_if.connect(ssid, contrasena)
while sta_if.isconnected()==False:
	pass
print("Connected")
serverIp = IP
tcpPort = puerto
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

while True:
	try:
		s.connect((serverIp, tcpPort))
		break
	except:
		time.sleep(1)
		continue
print("Connection with server reached")

i2c = I2C(scl=Pin(pinSCL), sda=Pin(pinSDA), freq=100000)

while True:
	try:
		cadenaRecibida = s.recv(8)
		if len (cadenaRecibida) == 8:
			print (cadenaRecibida)
			i2c.writeto(8, cadenaRecibida)
	except:
		print ("Cant reach I2C")
		time.sleep(2)
		continue
