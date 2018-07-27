import socket

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

def help():
    print("Usage")
    print("Xnnn -> X[Relative position X]")
    print("Ynnn -> Y[Relative position Y]")
    print("Znnn -> Z[Relative position Z]")
    print("S -> Set synchronization speeds")
    print("G -> Start trajectory")
    print("P -> Start positioning")
    print("h -> help")

def main():
  socket = Socket()
  cl = socket.iniciar(30000)
  print ("Enter command")
  print ("Type h to get help")
  while 1 :
    cadenaEntrada = raw_input("")
    for i in range(8 - len(cadenaEntrada)):
        cadenaEntrada = cadenaEntrada + '-'
    if cadenaEntrada[0] == 'G' or cadenaEntrada[0] == 'P' :
        print ("Enter command")
        print ("Type h to get help")
    if cadenaEntrada[0] == 'h':
        help()
    cl.sendall(cadenaEntrada)



if __name__ == '__main__':
    main()
