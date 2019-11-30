#include "client.h"
#include "generic.h"
#include "ccp.h"

int socketfd;
int connectionfd;
struct sockaddr_in serveraddress; 

int client_routine(char* ip, char* message, int portnumber){
    printf("uebergebene ip:%s\n",ip);
    printf("uebergebene message:%s\n",message);
    printf("uebergebene portnum:%d\n",portnumber);
    init_client();
    connect_to_server(ip, portnumber);
    testing_sendMessage(message);
    close_client();
    printf("client has send the message and closed itself\n");
    return 0;
    }

static int createSocket(){
    //AF_INET = Address familiy, here IPF4; SOCK STREAM Means TCP connection
    //TODO last input means Protocoll type, 0 means automatically choosen?
    socketfd = socket(AF_INET, SOCK_STREAM, 0); //returns a socket file descriptor
    if (socketfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    //fill the struct serveraddress with 0's
    bzero(&serveraddress, sizeof(serveraddress)); 
    return 0;
    }
    
static int assign_IP_PORT(char* inetAddress, int portnumber){
    //AF_INET = Address familiy
    serveraddress.sin_family = AF_INET; 
    serveraddress.sin_addr.s_addr = inet_addr(inetAddress);
    //SET MY PORT
    serveraddress.sin_port = htons(PORT); 
    return 0;
    }

static int build_connection(){
    /* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
    For connectionless socket types, just set the default address to send to
    and the only address from which to accept transmissions.
    Return 0 on success, -1 for errors. */
    if (connect(socketfd, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) != 0) { 
        printf("connection with the server failed...\n"); 
        //exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
    return 0;
    }


int init_client(){
    createSocket();
    return 0;
    }
    
int connect_to_server(char* inetAddress, int portnumber){
    assign_IP_PORT(inetAddress,portnumber);
    build_connection();
    return 0;
    }
    
int testing_sendMessage(char* message){
    write(socketfd, message, maxcharactersize); 
    }
    
int close_client(){
    close(socketfd);
    return 0;
    }