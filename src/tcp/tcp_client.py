import socket
import time
import sys

target_host = '0.0.0.0'
target_port = int(sys.argv[1])

while True:
	client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	client.connect((target_host,target_port))
	command = raw_input()
	client.send(command)
	response = client.recv(4096)
	print response	
	client.close()