#include "client.h"
#include "generic.h"
#include "ccp.h"

int socketfd;
int connectionfd;
struct sockaddr_in serveraddr; 


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
    //fill the struct serveraddr with 0's
    bzero(&serveraddr, sizeof(serveraddr)); 
    return 0;
    }
    
static int assign_IP_PORT(char* inetAddress){
    //AF_INET = Address familiy
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = inet_addr(inetAddress);
    //SET MY PORT
    serveraddr.sin_port = htons(PORT); 
    return 0;
    }

static int build_connection(){
    /* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
    For connectionless socket types, just set the default address to send to
    and the only address from which to accept transmissions.
    Return 0 on success, -1 for errors. */
    if (connect(socketfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
    return 0;
    }


int init_client(){
    createSocket();
    return 0;
    }
    
int connect_to_server(char* inetAddress){
    assign_IP_PORT(inetAddress);
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