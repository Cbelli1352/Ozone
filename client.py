import socket
import time

HOST = '127.0.0.1'
PORT = 5000  
s = socket.socket()
s.connect((HOST, PORT))
s.send('Hello, world')
print(s.recv(1024))
s.close()
