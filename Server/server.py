"""
    Authors: 
    Goal: Implement a Server that can communicate with Arduino
          using sockets
"""
from socket import *
import sys

SERVER_PORT = 12000
MAX_NUM_BYTES = 1024
ERROR_CODE_HDR = "HTTP/1.1 404 Not Found\r\n\r\n"
ERROR_HTML = "<html><head></head><body><h1>404 Not Found</h1></body></html>"

def connection_manager(connection_socket):
    """
        Running server manager that wait for a request from
        Arduino client side, if request fail then close connection
    """    
    while True:
        try:
            request = connection_socket.recv(1024)
            if request:
                print("Request accepted....")
                print(request)
                file_name = request.split()[1][1:]
                print(file_name)
                file = open(file_name,'rb')
                file_data = f.read()

                http_header = 'HTTP/1.0 200 Ok\n\nHi'
                connection_socket.send(http_header.encode())


                for i in range(0, len(file_data)):
                    connection_socket.send(file_data[i].encode())
                connection_socket.send("\r\n".encode())
                connection_socket.close()
                return

        except IOError:
            print("Error: 404 Connection fail")
            connection_socket.close()
            return

def main():
    server_socket = socket(AF_INET,SOCK_STREAM)

    # Bind with current IP address
    server_socket.bind(('',SERVER_PORT))
    server_socket.listen(1)

    while True:
        print('Server running.....')

        # Set Connection
        connection_socket, add = server_socket.accept()

        # Handle the connection
        connection_manager(connection_socket)

    # Close the open socket and terminate program
    server_socket.close()
    sys.exit()

if __name__=='__main__':
    main()