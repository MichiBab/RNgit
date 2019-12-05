#include "ccp.h"
#include "server.h"
#include "generic.h"
#include <pthread.h>
#include <unistd.h>
#include "ccp_thread_methods.h"
#include "msgflags.h"

int readFromSocket(int socket, struct sockaddr_in clientdata){
    char* buf = (char*) malloc(maxcharactersize);
    bzero(buf, maxcharactersize);
    int msgSize_in_bytes = read(socket, buf, maxcharactersize);
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

    react_to_package(ccp_data , clientdata);
    free(ccp_data);
    free(buf);
    return 0;
    }
    

int react_to_package(struct ccp* ccp_data , struct sockaddr_in clientdata){
    if(ccp_data->versionnum != '1'){
       // printf("Wrong version from paket detected!\n");
       // return 1;
        }
    char bufip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientdata.sin_addr, bufip, sizeof bufip);

    printf("CLIENT IP: %s\n",bufip);
    //printf("CLIENT PORT: %d\n", ntohs(clientdata.sin_port)); ???
    
    switch(ccp_data->typeFlags){
        case REQUEST_TO_OPEN_CONNECTION:
            react_to_hello(ccp_data,clientdata);
            break;
        
        case ACKNOWLEDGE_OPENING_CONNECTION:
            react_to_hello_reply(ccp_data);
            break;
        
        case PEER_DISCONNECTED:
            react_to_bye(ccp_data,clientdata);
            break;
        
        case SEND_A_MESSAGE:
            react_to_msg(ccp_data,clientdata);
            break;
        
        case REQUEST_IF_PEER_IS_ALIVE:
            react_to_update(ccp_data,clientdata);
            break;
            
        case ACKNOWLEDGE_RECEIVING_MESSAGE:
            react_to_msg_reply();
            break;
        
        case ACKNOWLEDGE_PEER_IS_STILL_ALIVE:
            react_to_update_reply();
            break;
        }

    return 0;
    }
    



//-------------------------------------------------------------------------
