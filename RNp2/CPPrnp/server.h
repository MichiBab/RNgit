#ifndef server_h
#define server_h
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
int readFromSocket(int socket, struct sockaddr_in clientdata);
int init_server();
int close_server();


#endif