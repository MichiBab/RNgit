#include "ccp.h"
#include "client.h"
#include "server.h"
#include "generic.h"
#include <unistd.h>

static int send_routine(struct datapack package){
    struct ccp ccp_data = package.ccppackage;
    DEBUG_MSG_NUM("Sending data to socket:",package.socketfd);
    send(package.socketfd, &ccp_data, sizeof(ccp_data), MSG_NOSIGNAL); 
    send_pipe_signal();//for loopback race condition
    return 0;
    }

int cr_send_hello(struct datapack package){ 
    DEBUG_MSG("IM SENDING A HELLO"); 
    set_ccp_hello(&package.ccppackage,package.receivername);
    //creats socket
    init_client( &package.serveraddress, &package.socketfd);
    if(connect_to_server(package.address, package.portnumber, &package.serveraddress, &package.socketfd) == 0){
        //if connected
        DEBUG_MSG("package send");
        add_socket_to_server_array(package.socketfd,package.serveraddress);
        send_routine(package);
        
        return 0;
    }
    DEBUG_MSG("package not send");

    return 0;
    }
    
int cr_send_hello_reply(struct datapack package){
    DEBUG_MSG("I SEND A HELLO REPLY"); 
    set_ccp_hello_reply(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_update(struct datapack package){
    DEBUG_MSG("I SEND AN UPDATE"); 
    set_ccp_update(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }
    
int cr_send_update_reply(struct datapack package){
    set_ccp_update_reply(&package.ccppackage, package.receivername);
    DEBUG_MSG("I SEND AN UPDATE REPLY"); 
    send_routine(package);
    return 0;
    }
    
int cr_sent_msg(struct datapack package){
    DEBUG_MSG("I SEND A MSG"); 
    set_ccp_message(&package.ccppackage, package.msg, package.receivername);
    send_routine(package);
    return 0;
    }

int cr_sent_msg_reply(struct datapack package){
    DEBUG_MSG("I SEND A MSG REPLY"); 
    set_ccp_message_reply(&package.ccppackage, package.receivername);
    send_routine(package);
    return 0;
    }


//sending a bye package to everyone in the contact list except urself.
//checks if socket is not null, if so: set a bye package
int cr_bye(struct datapack package){
    
    for(int i = 0; i<maxcontacts;i++){
        package.socketfd = contactlist_sockets[i][SOCKETFIELD];
        if(package.socketfd != -1){
            set_ccp_bye(&package.ccppackage, contactlist[i].contactalias);
            send_routine(package);
        }
    
    }

    return 0;
    }


    