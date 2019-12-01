#ifndef client_h
#define client_h
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
//int client_routine_CHATMODE(struct datapack* mypaket)
int init_client();
int connect_to_server(char* inetAddress, int portnumber);
int close_client();
int client_routine(char* ip, char* message, int portnumber);

#endif