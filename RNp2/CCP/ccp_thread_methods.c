#include "ccp.h"
#include <stdlib.h>

//--------TRHEADMETHODS-------------------------------------------------------
void* clientSendHello(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_send_hello(*data);
    free(data);
    return 0;
    }

//if connection is already established! Define receivername for datapack for this one
void* clientSendUpdate(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_send_update(*data);
    free(data);
    return 0;
    }
    
//if connection is already established and update got sent
void* clientSentMessage(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_sent_msg(*data);
    free(data);
    return 0;
    }    
    
 void* clientSentBye(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_bye(data->ccppackage);
    free(data);
    return 0;
    }   
    

void* clientSendHelloReply(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_send_hello_reply(*data);
    free(data);
    return 0;
    }

void* clientSendUpdateReply(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_send_update_reply(*data);
    free(data);
    return 0;
    }

void* clientSentMessageReply(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_sent_msg_reply(*data);
    free(data);
    return 0;
    }
    
    
    

    