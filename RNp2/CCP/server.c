#include "server.h"
#include "generic.h"
#include "ccp.h"
#include <errno.h> 

int parentfd; 
int clientfd; 

char* serverip_address;

struct sockaddr_in serveraddr; 


char buffer[maxcharactersize]; /* message buffer */
int msgSize_in_bytes; /* message byte size */

//--select test
int client_socket[MAXCLIENTS];
int max_sd, activity, sd, new_socket;
fd_set readfds; 
int valread;
int addrlen = sizeof(serveraddr);
//--

int close_server(){
    close(parentfd);
    return 0;
    }

int set_serverip_address(char* msg){
    serverip_address = (char*) malloc(sizeof(char)*16);
    strncpy(serverip_address, msg, 16);
    return 0;
    }

static int create_socket(){
    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0) {
        printf("ERROR opening socket");
        exit(1);
    }

    /*
    * build the server's Internet address
    */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    int opt = 1;
    if( setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {   
        printf("setsockopt");   
        exit(1);   
    }
    
    
    return 0;
    }
    
int get_my_ip(char* input){
    inet_ntop(AF_INET, &(serveraddr.sin_addr), input, INET_ADDRSTRLEN);
    return 0;
    }
    
static int setIPandPort(){
    /* this is an Internet address */
    serveraddr.sin_family = AF_INET;

    /* let the system figure out our IP address */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* this is the port we will listen on */
    serveraddr.sin_port = htons(PORT);
    
    return 0;
    }
    
static int bindSocket(){
    if (bind(parentfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        printf("ERROR binding socket");
        exit(1);
        }
    return 0;
    }

static int listenSocket(){
    if (listen(parentfd, MAXCLIENTS) < 0) {
        printf("ERROR listening");
        exit(1);
    }
    return 0;
    }

static int readFromSocket(int socket){
    bzero(buffer, maxcharactersize);
    msgSize_in_bytes = read(socket, buffer, maxcharactersize);
    printf("msginbytes: %d\n",msgSize_in_bytes);
    if (msgSize_in_bytes < 0) {
        printf("ERROR reading from socket");
        exit(1);
    }
    else if(msgSize_in_bytes == 0){
        return 1;//for closing
        }
    
    //casting 
    struct ccp ccp_tmp;
    *buffer = (char*) &ccp_tmp;
    struct ccp *ccp_data;
    ccp_data = (struct ccp*) buffer;
    
    printf("received data. now printing it\n");
    printf("sender alias: %s\n",ccp_data->senderAlias);
    printf("receiver alias: %s\n",ccp_data->receiverAlias);
    return 0;
    }

//initialise all client_socket[] to 0 so not checked  
static int init_clientfds(){
    for (int i = 0; i < MAXCLIENTS; i++){   
        client_socket[i] = 0;   
        }   
    return 0;
    }


static int addchilds(){
    //add child sockets to set  
    for (int i = 0 ; i < MAXCLIENTS ; i++){
        //socket descriptor
        sd = client_socket[i];
          
        //if valid socket descriptor then add to read list
        if(sd > 0){
            FD_SET( sd , &readfds);
            }
          
          
        //if valid socket descriptor then add to read list
        if(sd > 0){
            max_sd = sd;
            }
        }
    return 0;
    }
    
static int acceptConnections(){
    //If something happened on the master socket ,  
    //then its an incoming connection     
    if (FD_ISSET(parentfd, &readfds)){
        new_socket = accept(parentfd, (struct sockaddr *)&serveraddr, (socklen_t*)&addrlen);
        if (new_socket < 0){
            printf("error accept\n");
            exit(1);
            }
        //add new socket to array of sockets  
        for (int i = 0; i < MAXCLIENTS; i++){   
            //if position is empty  
            if( client_socket[i] == 0 ) {   
                client_socket[i] = new_socket;   
              //  printf("Adding to list of sockets as %d\n" , i);   
                break;   
                }
            }
        }
    return 0;
    }
    
static int getMessages(){
    //GET MESSAGES
    //printf("server tries to read messages now\n");
    for (int i = 0; i < MAXCLIENTS; i++){
        sd = client_socket[i];    
        
        //if there is a valid socket
        if (FD_ISSET( sd , &readfds)){
           //printf("server got a valid socket\n");
                if(readFromSocket(client_socket[i])){
                   // printf("client number %d is disconnected (read gave 0 bytes back)\n",i);
                    close( sd );
                    client_socket[i] = 0;
                }
            }
        }
    return 0;
    
    }

int init_server() {
    
    init_clientfds();
    
    create_socket();

    setIPandPort();
  
    bindSocket();

    listenSocket();

    /* 
    * main loop: wait for a connection request, read input line, 
    * then close connection.
    */
    
    while (1) {
      
        //clear the socket set
        FD_ZERO(&readfds);
      
        //add master socket to set  
        FD_SET(parentfd, &readfds);
        max_sd = parentfd;
      
        addchilds();
          
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno!=EINTR)){
            printf("select error \n");
            }
            
        acceptConnections();
        
        getMessages();
    }
}
