#ifndef client_h
#define client_h
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 

int init_client();
int connect_to_server(char* inetAddress, int portnumber);
int testing_sendMessage(char* message);
int close_client();
int client_routine(char* ip, char* message, int portnumber);
#endif