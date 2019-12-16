#include "ccp.h"
#include "generic.h"
#include <pthread.h>
#include "ccp_thread_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msgflags.h"

static int setup_datapackage(struct datapack* tmpdatapaket, char* alias, struct sockaddr_in clientdata){
    char bufip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientdata.sin_addr, bufip, sizeof bufip);
    tmpdatapaket->portnumber = PORT;
    put_string_in_sender_receiver(tmpdatapaket->address,bufip);
    put_string_in_sender_receiver(tmpdatapaket->receivername, alias);
    return 0;
    }

int react_to_hello(struct ccp* ccp_data , struct sockaddr_in clientdata){
    printf("I GOT AN HELLO!\n");   
    struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
    struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (sizeof(contactlist));
    pthread_t helperclient;
    memcpy(ccp_contact_newlist,ccp_data->message,sizeof(contactlist));
    update_contact_list(ccp_contact_newlist);
    setup_datapackage(tmpdatapaket,ccp_data->senderAlias,clientdata);
    put_contact_list_in_message_of_ccp(&tmpdatapaket->ccppackage); //send our new contactlist back
    pthread_create(&helperclient,NULL,clientSendHelloReply,(struct datapack*)tmpdatapaket);
    free(ccp_contact_newlist);
    return 0;
}

int react_to_hello_reply(struct ccp* ccp_data){
    printf("I GOT AN HELLO REPLY!\n");
    struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (sizeof(contactlist));
    memcpy(ccp_contact_newlist,ccp_data->message,sizeof(contactlist));
    update_contact_list(ccp_contact_newlist); //JUST UPDATE OUR CONTACT LIST. 
    //UPDATE CONTACT LIST HANDLES NEW HELLOs
    return 0;
    }
    
int react_to_update(struct ccp* ccp_data , struct sockaddr_in clientdata){
    printf("I GOT A ALIVE REQUEST!\n");
    struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
    pthread_t helperclient;
    setup_datapackage(tmpdatapaket,ccp_data->senderAlias,clientdata);
    pthread_create(&helperclient,NULL,clientSendUpdateReply,(struct datapack*)tmpdatapaket);
    return 0;
    }
    
int react_to_update_reply(){
    printf("I GOT A ALIVE ACK!\n");
    set_up_flag();
    return 0;
    }
    
int react_to_msg(struct ccp* ccp_data , struct sockaddr_in clientdata){
    printf("I GOT A MESSAGE\n");
    struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
    pthread_t helperclient;
    char *tmpmsg = (char *) malloc (MAXCHARACTERS);
    memcpy(tmpmsg,ccp_data->message,MAXCHARACTERS);
    setup_datapackage(tmpdatapaket,ccp_data->senderAlias,clientdata);
    printf("MSG FROM CLIENT:\n%s",tmpmsg); 
    pthread_create(&helperclient,NULL,clientSentMessageReply,(struct datapack*)tmpdatapaket);
    return 0;
    }
    
int react_to_msg_reply(){
    set_msg_flag();
    return 0;
    }
    
int react_to_bye(struct ccp* ccp_data , struct sockaddr_in clientdata){
    printf("I GOT A DISCONNECT!\n");
    char bufip[INET_ADDRSTRLEN];//get ip
    inet_ntop(AF_INET, &clientdata.sin_addr, bufip, sizeof bufip);
    struct in_addr addrtmp;
    struct ccp_contact* tempcon = (struct ccp_contact*) malloc (sizeof(struct ccp_contact) );
    inet_pton(AF_INET, bufip, &addrtmp);
    tempcon->contactIPv4 = addrtmp.s_addr;
    tempcon->contactPort = PORT;
    put_string_in_sender_receiver(tempcon->contactalias,"DELETE ME");
    print_contact(tempcon);
    remove_contact(*tempcon);
    free(tempcon);
    return 0;
    }