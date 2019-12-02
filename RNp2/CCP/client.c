#include "client.h"
#include "generic.h"
#include "ccp.h"


//int socketfd;
//struct sockaddr_in serveraddress; 



static int send_routine(struct datapack package){
    
    struct ccp ccp_data = package.ccppackage;
    init_client( &package.serveraddress, &package.socketfd);
    connect_to_server(package.address, package.portnumber, &package.serveraddress, &package.socketfd);
    write(package.socketfd, &ccp_data, sizeof(ccp_data));
    close_client(&package.socketfd);
    return 0;
    }

int cr_send_hello(struct datapack package){
    set_ccp_hello(&package.ccppackage,package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_hello_reply(struct datapack package){
    set_ccp_hello_reply(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_update(struct datapack package){
    set_ccp_update(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_update_reply(struct datapack package){
    set_ccp_update_reply(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_sent_msg(struct datapack package){
    set_ccp_message(&package.ccppackage, package.msg, package.receivername);
    send_routine(package);
    return 0;
    }

int cr_sent_msg_reply(struct datapack package){
    set_ccp_message_reply(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }


//sending a bye package to everyone in the contact list except urself.
//checks if client is not null, if so: set a bye package, init a socket
//to communicate on, connect to the server through contact informations
//and send the bye package. close the client socket and free malloc data
int cr_bye(struct ccp ccp_pack){
    struct sockaddr_in serveraddress;
    int socketfd; 
    char* ip;
    uint16_t* por;
    ip = (char*) malloc(sizeof(char)*4);
    por = (uint16_t*) malloc(sizeof(uint16_t));
    for(int i = 1; i<contactlist;i++){//we start with 1, cause we are index 0
        if( check_if_nullcontact(contactlist[i]) ){ // if true, there is a contact
            set_ccp_bye(&ccp_pack, contactlist[i].contactalias);
            init_client( &serveraddress, &socketfd);
            connect_to_server( get_ipstring_from_contact(contactlist[i],ip), 
                               get_port_int_from_contact(contactlist[i],por), 
                               &serveraddress, &socketfd);
            write(socketfd, &ccp_pack, sizeof(ccp_pack));
            close_client(&socketfd);
            }      
        }
    free(ip);
    free(por);
    }

static int createSocket(struct sockaddr_in* serveraddress, int* socketfd ){
    //AF_INET = Address familiy, here IPv4; SOCK STREAM Means TCP connection
    //TODO last input means Protocoll type, 0 means automatically choosen?
    *socketfd = socket(AF_INET, SOCK_STREAM, 0); //returns a socket file descriptor
    if (*socketfd == -1) { 
        printf("socket creation failed...\n"); 
        //exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    //fill the struct serveraddress with 0's
    bzero(serveraddress, sizeof(*serveraddress)); 
    return 0;
    }
    
static int assign_IP_PORT(char* inetAddress, int portnumber, struct sockaddr_in* serveraddress, int* socketfd ){
    //AF_INET = Address familiy
    printf("my assingend ip for connecting : %s",inetAddress);
    //if(strcmp(inetAddress, 0) == 0){
    //    return -1;
    //    }
    serveraddress->sin_family = AF_INET; 
    serveraddress->sin_addr.s_addr = inet_addr(inetAddress);
    //SET MY PORT
    serveraddress->sin_port = htons(PORT); 
    return 0;
    }

static int build_connection(struct sockaddr_in* serveraddress, int* socketfd ){
    /* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
    For connectionless socket types, just set the default address to send to
    and the only address from which to accept transmissions.
    Return 0 on success, -1 for errors. */
    if (connect(*socketfd, (struct sockaddr*)serveraddress, sizeof(*serveraddress)) != 0) { 
        printf("connection with the server failed...\n"); 
        return -1;
    } 
    else
        printf("connected to the server..\n"); 
    return 0;
    }


int init_client(struct sockaddr_in* serveraddress, int* socketfd ){
    createSocket(serveraddress, socketfd );
    return 0;
    }
    
int connect_to_server(char* inetAddress, int portnumber, struct sockaddr_in* serveraddress, int* socketfd ){
    
    assign_IP_PORT(inetAddress,portnumber,serveraddress,socketfd);
    printf("ip port assigned\n");
    build_connection(serveraddress,socketfd);
    return 0;
    }
    
    
int close_client(int* socketfd){
    close(*socketfd);
    return 0;
    }
    