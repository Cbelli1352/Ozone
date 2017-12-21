import socket
import time

HOST = '127.0.0.1'
PORT = 5000  
s = socket.socket()
s.connect((HOST, PORT))
s.send('GET /../client.py')
print(s.recv(4096))
time.sleep(10)
s.close()
