import socket
import sys

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket successfully created")
except socket.error as e:
    print("Socket creation failed with error ", str(e))

port = 80

try:
    host_ip = socket.gethostbyname("www.google.com")
except socket.gaierror:
    print("Error resolving the host")
    sys.exit()

s.connect((host_ip, port))

print("the socket has succesffully connected to google")
ip = ""

serverPort = 12000
serverSocket = socket.socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(("", serverPort))

serverSocket.listen(5)
print("server is ready to receive")

while True:
    message, clientAddres = serverSocket.recvfrom(2048)
    print(message.upper())
