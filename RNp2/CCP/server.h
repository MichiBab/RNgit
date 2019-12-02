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

int init_server();
int close_server();
int get_my_ip(char* input);
extern char* serverip_address;
int set_serverip_address(char* msg);
#endif