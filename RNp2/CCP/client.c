#include "client.h"
#include "generic.h"
#include "ccp.h"
#include "ticketloop.h"

int socketfd;
struct sockaddr_in serveraddress; 

//fair mutex lock. priorities wont matter.
ticket_lock_t chatmutex;

int lock(){
    ticket_lock(&chatmutex);
    }
    
int unlock(){
    ticket_unlock(&chatmutex);
    }

int cr_connection_establishment(struct datapack package){
    struct ccp ccp_data = package.ccppackage;
    printf("uebergeben ip: %s\n", package.address);
    printf("uebergeben port: %d\n",package.portnumber);
    init_client();
    connect_to_server(package.address, package.portnumber);
    write(socketfd, &ccp_data, sizeof(ccp_data));
    
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
    
    
int close_client(){
    close(socketfd);
    return 0;
    }
    
    

//waren nur zum testen da-------------------------------------------------------------------------------------------------
char* clientmessage;

int changeMessage(char* msg){
    clientmessage = msg;
    }

int client_routine(char* ip, char* message, int portnumber){
    init_client();
    //printf("fertig mit client init\n");
    connect_to_server(ip, portnumber);
    //printf("fertig mit connect to server\n");
    //testing send message
    
    write(socketfd, message, maxcharactersize); 
    sleep(1);
    write(socketfd, message, maxcharactersize); 
    printf("client has send the message\n");
    
    return 0;
    }

int client_routine_chatmode(struct datapack mypack){
    init_client();
    connect_to_server(mypack.address, mypack.portnumber);
    while(1){
        lock();
        write(socketfd, clientmessage, maxcharactersize); 
        unlock();
        }
    
    return 0;
    }