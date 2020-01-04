#include "client.h"
#include "generic.h"
#include <unistd.h> 
#include <sys/time.h>
//#include <signal.h>

/*void interrupt_handler(int sig){
    printf("Interrupt recieved from client!\n");
    return;
}*/

#define CLIENTSOCKWAIT 3
static int createSocket(struct sockaddr_in* serveraddress, int* socketfd ){
    //AF_INET = Address familiy, here IPv4; SOCK STREAM Means TCP connection
    //TODO last input means Protocoll type, 0 means automatically choosen?
    struct timeval tv;
    tv.tv_sec = CLIENTSOCKWAIT;
    tv.tv_usec = 0;
    
    *socketfd = socket(AF_INET, SOCKET_TYPE , SOCKET_ARG); //returns a socket file descriptor
    if (*socketfd == -1) { 
        printf("socket creation failed...\n"); 
        //exit(0); 
    } 
    else
     //   printf("Socket successfully created..\n"); 
    //fill the struct serveraddress with 0's
    bzero(serveraddress, sizeof(*serveraddress)); 
    setsockopt(*socketfd , SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    if(sctp_mode && heartbeat_param > 0){
        set_heartbeat(socketfd);
    }
    return 0;
    }
    
static int assign_IP_PORT(char* inetAddress, int portnumber, struct sockaddr_in* serveraddress, int* socketfd ){
    //AF_INET = Address familiy
    DEBUG_MSG_STRING("my assingend ip for connecting : %s\n",inetAddress);
    //printf("my assingend port for connecting : %d\n",portnumber);
    serveraddress->sin_family = AF_INET; 
    serveraddress->sin_addr.s_addr = inet_addr(inetAddress);
    //SET MY PORT
    serveraddress->sin_port = htons(portnumber); 
    return 0;
    }

static int build_connection(struct sockaddr_in* serveraddress, int* socketfd ){
    /* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
    For connectionless socket types, just set the default address to send to
    and the only address from which to accept transmissions.
    Return 0 on success, -1 for errors. */
    //signal(SIGINT, interrupt_handler);
    DEBUG_MSG("im in connect \n");
    if (connect(*socketfd, (struct sockaddr*)serveraddress, sizeof(*serveraddress)) != 0) { 
        printf("connection with the server failed...\n"); 
        return -1;
    } 
    else
        DEBUG_MSG("connected to the server..\n"); 
    return 0;
    
    return 0;
    }


int init_client(struct sockaddr_in* serveraddress, int* socketfd ){
    createSocket(serveraddress, socketfd );
    return 0;
    }
    
int connect_to_server(char* inetAddress, int portnumber, struct sockaddr_in* serveraddress, int* socketfd ){

    assign_IP_PORT(inetAddress,portnumber,serveraddress,socketfd);

    if(build_connection(serveraddress,socketfd) == -1){
        DEBUG_MSG("error in connecting \n");
        return -1;
        }
    return 0;
    }
    
    
int close_client(int* socketfd){
    close(*socketfd);
    return 0;
    }
    