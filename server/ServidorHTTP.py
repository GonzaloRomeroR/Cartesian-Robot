from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import os
import base64
import ReconocimientoMicro
import socket

#Se crea una clase HTTPRequestHandler propia
def MakeHandler(cl):

    class MiHTTPRequestHandler(BaseHTTPRequestHandler):

      def do_POST(self):
            #Recibe la informacion desde el cliente
            print ("Se ha recibido la solicitud")
            #Recibe el tamano de la informacion
            content_length = int(self.headers['Content-Length'])
            #Recibe la informacion en cuestion
            data_received = self.rfile.read(content_length)
            #Decodifica el string en base 64
            imgdata = base64.b64decode(data_received[23:])
            #Escribe el archivo
            f = open('Imagen/imagen.jpg', 'wb')
            f.write(imgdata)
            f.close()
            #Senal de respuesta
            print ("Se ha realizado la trasferencia exitosamente")
            self.wfile.write("200")
            ReconocimientoMicro.configuracionInicial(cl)
    return MiHTTPRequestHandler

class Socket:
    def iniciar(self, puerto):
        print ("Inicio del programa")
        print ("Esperando conexiones...")
        addr = socket.getaddrinfo('0.0.0.0', puerto)[0][-1]
        s = socket.socket()
        s.bind(addr)
        s.listen(1)
        cl, addr = s.accept()
        print('Cliente conectado desde', addr)
        print('Escuchando en', addr)
        return cl



def correrServidor(IP, puerto):
  print ("Inicia el Servidor")
  print('Servidor http comenzando...')
  server_address = (IP, puerto)
  socket = Socket()
  cl = socket.iniciar(30000)
  Handler = MakeHandler(cl)
  httpd = HTTPServer(server_address, Handler)
  print('El servidor http esta corriendo...')
  httpd.serve_forever()

if __name__ == '__main__':
  correrServidor('10.32.11.180', 4000)
