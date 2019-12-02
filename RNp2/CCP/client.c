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
    //printf("uebergeben ip: %s\n", package.address);
    //printf("uebergeben port: %d\n",package.portnumber);
    init_client();
    connect_to_server(package.address, package.portnumber);
    write(socketfd, &ccp_data, sizeof(ccp_data));
    close_client();
    return 0;
    }
    
int cr_update_send(struct datapack package){
    set_ccp_update(&package.ccppackage, package.receivername);
    struct ccp ccp_data = package.ccppackage;
    init_client();
    connect_to_server(package.address, package.portnumber);
    write(socketfd, &ccp_data, sizeof(ccp_data));
    close_client();
    return 0;
    }
    
int cr_sent_msg(struct datapack package){
    set_ccp_message(&package.ccppackage, package.msg, package.receivername);
    struct ccp ccp_data = package.ccppackage;
    init_client();
    connect_to_server(package.address, package.portnumber);
    write(socketfd, &ccp_data, sizeof(ccp_data));
    close_client();
    return 0;
    }

//sending a bye package to everyone in the contact list except urself.
//checks if client is not null, if so: set a bye package, init a socket
//to communicate on, connect to the server through contact informations
//and send the bye package. close the client socket and free malloc data
int cr_bye(struct ccp ccp_pack){
    char* ip;
    uint16_t* por;
    ip = (char*) malloc(sizeof(char)*4);
    por = (uint16_t*) malloc(sizeof(uint16_t));
    for(int i = 1; i<contactlist;i++){//we start with 1, cause we are index 0
        if( check_if_nullcontact(contactlist[i]) ){ // if true, there is a contact
            set_ccp_bye(&ccp_pack, contactlist[i].contactalias);
            init_client();
            connect_to_server( get_ipstring_from_contact(contactlist[i],ip), 
                               get_port_int_from_contact(contactlist[i],por));
            write(socketfd, &ccp_pack, sizeof(ccp_pack));
            close_client();
            }      
        }
    free(ip);
    free(por);
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