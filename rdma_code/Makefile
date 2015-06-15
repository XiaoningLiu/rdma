# Makefile for the socket programming example
#

simple_server_objects = ServerSocket.o Socket.o simple_server_main.o
simple_client_objects = ClientSocket.o Socket.o simple_client_main.o

server_objects = rdma_server.o ServerSocket.o Socket.o server.o rdma_common.o
client_objects = rdma_client.o ClientSocket.o Socket.o client.o rdma_common.o

#all : simple_server simple_client server client
all : server client

#simple_server: $(simple_server_objects)
#g++ -o simple_server $(simple_server_objects)

#simple_client: $(simple_client_objects)
#g++ -o simple_client $(simple_client_objects)

server: $(server_objects)
	g++ -o server $(server_objects)

client: $(client_objects)
	g++ -o client $(client_objects)

Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
rdma_server: rdma_server.cpp
rdma_client: rdma_client.cpp
simple_server_main: simple_server_main.cpp
simple_client_main: simple_client_main.cpp
rdma_common: rdma_common.cpp

clean:
	rm -f *.o simple_server simple_client server client
