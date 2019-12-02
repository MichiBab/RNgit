#include "server.h"
#include "generic.h"
#include "ccp.h"
#include <errno.h> 
#include "client.h"

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

static int react_to_package(struct ccp* pack, int socket);

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
       // exit(1);
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
       // exit(1);   
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
       // exit(1);
        }
    return 0;
    }

static int listenSocket(){
    if (listen(parentfd, MAXCLIENTS) < 0) {
        printf("ERROR listening");
       // exit(1);
    }
    return 0;
    }

static int readFromSocket(int socket){
    char* buf = (char*) malloc(maxcharactersize);
    bzero(buf, maxcharactersize);
    msgSize_in_bytes = read(socket, buf, maxcharactersize);
    //printf("msginbytes: %d\n",msgSize_in_bytes);
    if (msgSize_in_bytes < 0) {
        printf("ERROR reading from socket");
        //exit(1);
    }
    else if(msgSize_in_bytes == 0){
        return 1;//for closing
        }
        
    struct ccp *ccp_data = (struct ccp *) malloc (sizeof(struct ccp));
    memcpy(ccp_data,buf,sizeof(struct ccp));
    
    printf("received data. now printing it\n");
    printf("sender alias: %s\n",ccp_data->senderAlias);
    printf("receiver alias: %s\n",ccp_data->receiverAlias);
    
    react_to_package(ccp_data, socket);
    free(ccp_data);
    free(buf);
    return 0;
    }


/*struct ccp{
    char versionnum;
    char typeFlags;
    char reserved[2];
    char senderAlias[16];
    char receiverAlias[16];
    char message[1024];
    };*/
static int react_to_package(struct ccp* ccp_data, int socket){
    //printf("IM IN REACT TO PACKAGE\n");
    if(ccp_data->versionnum != '1'){
       // printf("Wrong version from paket detected!\n");
      //  return 1;
        }
    
    char bufip[INET_ADDRSTRLEN] = ""; //GET IP FROM SOCKET
        struct sockaddr_in name;
        socklen_t len = sizeof(name);

        if (getpeername(socket, (struct sockaddr *)&name, &len) != 0) {
            printf("getpeername_error");
            return 1;
        } else {
            inet_ntop(AF_INET, &name.sin_addr, bufip, sizeof bufip);
            }
    printf("socket ip: %s\n",bufip);
    if(ccp_data->typeFlags == REQUEST_TO_OPEN_CONNECTION){
            
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (MAXCHARACTERS);
        pthread_t helperclient;
        memcpy(ccp_contact_newlist,ccp_data->message,MAXCHARACTERS);

        update_contact_list(ccp_contact_newlist);

        tmpdatapaket->portnumber = PORT;
        inet_ntop(AF_INET, &name.sin_addr, tmpdatapaket->address, sizeof bufip);
        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias);
        put_contact_list_in_message_of_ccp(&tmpdatapaket->ccppackage); //send our new contactlist back
        pthread_create(&helperclient,NULL,clientSendHelloReply,(struct datapack*)tmpdatapaket);
        //free(ccp_contact_newlist);
        //free(tmpdatapaket);
        }
    
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_OPENING_CONNECTION){ // i sent a hello and got a hello reply back
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        pthread_t helperclient;
        struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (MAXCHARACTERS);
        memcpy(ccp_contact_newlist,ccp_data->message,MAXCHARACTERS);
        update_contact_list(ccp_contact_newlist); //JUST UPDATE OUR CONTACT LIST. UPDATE CONTACT LIST HANDLES NEW HELLOs
        //free(ccp_contact_newlist);
        //free(tmpdatapaket);
        }
        
    if(ccp_data->typeFlags == PEER_DISCONNECTED){
        //todo
        
        }
    
    if(ccp_data->typeFlags == SEND_A_MESSAGE){ // we reseaved a msg. print it and send a reply
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        pthread_t helperclient;
        char *tmpmsg = (char *) malloc (MAXCHARACTERS);
        memcpy(tmpmsg,ccp_data->message,MAXCHARACTERS);
        printf("MSG FROM CLIENT:\n%s",tmpmsg); 
        
        tmpdatapaket->portnumber = PORT;
        inet_ntop(AF_INET, &name.sin_addr, tmpdatapaket->address, sizeof bufip);
        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias) ;
        pthread_create(&helperclient,NULL,clientSentMessageReply,(struct datapack*)tmpdatapaket);
        //free(tmpdatapaket);
        //free(tmpmsg);
        }
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_RECEIVING_MESSAGE){
        //todo???
        }
    
    if(ccp_data->typeFlags == REQUEST_IF_PEER_IS_ALIVE){
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        pthread_t helperclient;
        tmpdatapaket->portnumber = PORT;
        inet_ntop(AF_INET, &name.sin_addr, tmpdatapaket->address, sizeof bufip);
        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias) ;
        pthread_create(&helperclient,NULL,clientSendUpdateReply,(struct datapack*)tmpdatapaket);
        //free(tmpdatapaket);
        }
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_PEER_IS_STILL_ALIVE){
        //todo???
        }
    
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
            //exit(1);
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
