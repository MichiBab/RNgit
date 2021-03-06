#include "ccp.h"
#include "server.h"
#include "generic.h"
#include <pthread.h>
#include <unistd.h>
#include "ccp_thread_methods.h"
#include "ccp_socket_list.h"

int readFromSocket(int socket, struct sockaddr_in clientdata){
    
    char* buf = (char*) malloc(maxcharactersize);
    bzero(buf, maxcharactersize);
    DEBUG_MSG("IN RECEIVE NOW");
    int msgSize_in_bytes = recv(socket, buf, maxcharactersize, MSG_NOSIGNAL);
    DEBUG_MSG_NUM("msginbytes: %d\n",msgSize_in_bytes);
    if (msgSize_in_bytes < 0) {
        DEBUG_MSG("ERROR reading from socket");
        return 0;
        //exit(1);
    }
    else if(msgSize_in_bytes == 0){
        return 1;//for closing
        }
        
    struct ccp *ccp_data = (struct ccp *) malloc (sizeof(struct ccp));
    memcpy(ccp_data,buf,sizeof(struct ccp));

    react_to_package(ccp_data , clientdata, socket);
    free(ccp_data);
    free(buf);
    return 0;
    }
    

int react_to_package(struct ccp* ccp_data , struct sockaddr_in clientdata , int socket){
    if(ccp_data->versionnum != '1' && ccp_data->versionnum != 0b1){
        printf("Wrong version from paket detected!\n");
        printf("we received: %d\n",ccp_data->versionnum);
        return 1;
        }
    char bufip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientdata.sin_addr, bufip, sizeof bufip);

    DEBUG_MSG_STRING("CLIENT IP: %s\n",bufip);
    
    switch(ccp_data->typeFlags){
        case REQUEST_TO_OPEN_CONNECTION:
            react_to_hello(ccp_data,clientdata,socket);
            break;
        
        case ACKNOWLEDGE_OPENING_CONNECTION:
            react_to_hello_reply(ccp_data,clientdata,socket);
            break;
        
        case PEER_DISCONNECTED:
            react_to_bye(ccp_data,clientdata,socket);
            break;
        
        case SEND_A_MESSAGE:
            react_to_msg(ccp_data,clientdata,socket);
            break;
        
        case REQUEST_IF_PEER_IS_ALIVE:
            react_to_update(ccp_data,clientdata,socket);
            break;
            
        case ACKNOWLEDGE_RECEIVING_MESSAGE:
            react_to_msg_reply(socket);
            break;
        
        case ACKNOWLEDGE_PEER_IS_STILL_ALIVE:
            react_to_update_reply(socket);
            break;
        default:
            printf("Unknown type Flags\n");
        }
    send_pipe_signal();//for loopback race condition
    return 0;
    }
    



//-------------------------------------------------------------------------
