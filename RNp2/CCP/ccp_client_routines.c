#include "ccp.h"
#include "client.h"
#include <unistd.h>

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
    uint16_t* por;
    char* ip = (char*) malloc(sizeof(char)*4);
    por = (uint16_t*) malloc(sizeof(uint16_t));
    for(int i = 1; i<maxcontacts;i++){//we start with 1, cause we are index 0
        if( check_if_nullcontact(contactlist[i]) ){ // if true, there is a contact
            set_ccp_bye(&ccp_pack, contactlist[i].contactalias);
            init_client( &serveraddress, &socketfd);
            get_ipstring_from_contact(contactlist[i],ip);
            get_port_int_from_contact(contactlist[i],por);
            connect_to_server( ip,  *por, &serveraddress, &socketfd);
            write(socketfd, &ccp_pack, sizeof(ccp_pack));
            close_client(&socketfd);
            }      
        }
    free(ip);
    free(por);
    return 0;
    }

//int connect_to_server(char* inetAddress, int portnumber, struct sockaddr_in* serveraddress, int* socketfd );