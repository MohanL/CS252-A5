
Proxy Server

Files:

doit.c          - main routine for multithreading

doit.o

clienterror.c   - return error message to clients

clienterror.h

interclient.c   - forward request to the real server

interclient.h

open_listenfd.c - function prepares a server socket

open_listenfd.h

rio.c           - robust io functions for reading initial request

rio.h

main.c          - driver class

makefile


Run instructions:

./ProxyServer (portnumber)

Implemented functionality:

Parsing the original request and send data blocks backward and forward in between the real server and the real client.

