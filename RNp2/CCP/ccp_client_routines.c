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
    printf("I SEND A HELLO\n"); 
    set_ccp_hello(&package.ccppackage,package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_hello_reply(struct datapack package){
    printf("I SEND A HELLO REPLY\n"); 
    set_ccp_hello_reply(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_update(struct datapack package){
    printf("I SEND A UPDATE\n"); 
    set_ccp_update(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_update_reply(struct datapack package){
    
    set_ccp_update_reply(&package.ccppackage, package.receivername);
    printf("I SEND A UPDATE REPLY\n"); 
    send_routine(package);
    return 0;
    }
    
int cr_sent_msg(struct datapack package){
    printf("I SEND A MSG\n"); 
    set_ccp_message(&package.ccppackage, package.msg, package.receivername);
    send_routine(package);
    return 0;
    }

int cr_sent_msg_reply(struct datapack package){
    printf("I SEND A MSG REPLY\n"); 
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
    for(int i = 0; i<maxcontacts;i++){//we start with 1, cause we are index 0. for testing its on 0
        if( check_if_nullcontact(contactlist[i]) ){ 
            printf("SENDING A DELETE TO THIS CONTACT ON INDEX %d\n",i);
            print_contact(&contactlist[i]);
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
