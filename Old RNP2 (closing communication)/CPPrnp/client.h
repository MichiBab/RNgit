#ifndef client_h
#define client_h
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
//int client_routine_CHATMODE(struct datapack* mypaket)
int init_client(struct sockaddr_in* serveraddress, int* socketfd );
int close_client(int* socketfd);
int client_routine(char* ip, char* message, int portnumber);
int changeMessage(char* msg);
int connect_to_server(char* inetAddress, int portnumber, struct sockaddr_in* serveraddress, int* socketfd );

void* clientSendHello(void* arg);
void* clientSendUpdate(void* arg);
void* clientSentMessage(void* arg);
void* clientSentBye(void* arg);

void* clientSendHelloReply(void* arg);
void* clientSendUpdateReply(void* arg);
void* clientSentMessageReply(void* arg);
#endif