#include "ccp.h"
#include "server.h"
#include "generic.h"
#include <pthread.h>
#include <unistd.h>
#include "ccp_thread_methods.h"

char buffer[maxcharactersize]; /* message buffer */
int msgSize_in_bytes; /* message byte size */
char msgflagarray[2];
    
pthread_mutex_t flagmutex = PTHREAD_MUTEX_INITIALIZER;

int react_to_package(struct ccp* ccp_data, int socket , struct sockaddr_in clientdata){
    //printf("IM IN REACT TO PACKAGE\n");
    if(ccp_data->versionnum != '1'){
       // printf("Wrong version from paket detected!\n");
       // return 1;
        }
    
    char bufip[INET_ADDRSTRLEN] = ""; //GET IP FROM EXTERN CLIENT
    
    inet_ntop(AF_INET, &clientdata.sin_addr, bufip, sizeof bufip);

    printf("CLIENT IP: %s\n",bufip);
    //printf("CLIENT PORT: %d\n", ntohs(clientdata.sin_port)); ???
    if(ccp_data->typeFlags == REQUEST_TO_OPEN_CONNECTION){
        printf("I GOT A HELLO!\n");   
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (sizeof(contactlist));
        pthread_t helperclient;
        memcpy(ccp_contact_newlist,ccp_data->message,sizeof(contactlist));
        update_contact_list(ccp_contact_newlist);
        tmpdatapaket->portnumber = PORT;

        put_string_in_sender_receiver(tmpdatapaket->address,bufip);
        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias);
        put_contact_list_in_message_of_ccp(&tmpdatapaket->ccppackage); //send our new contactlist back

        pthread_create(&helperclient,NULL,clientSendHelloReply,(struct datapack*)tmpdatapaket);

        }
    
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_OPENING_CONNECTION){ //i sent a hello and got a hello reply back
        printf("I GOT A HELLO REPLY!\n");
        struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (sizeof(contactlist));
        memcpy(ccp_contact_newlist,ccp_data->message,sizeof(contactlist));
        
        update_contact_list(ccp_contact_newlist); //JUST UPDATE OUR CONTACT LIST. UPDATE CONTACT LIST HANDLES NEW HELLOs
        //TODO MESSAGE
        }
        
    if(ccp_data->typeFlags == PEER_DISCONNECTED){ //removing this contact from our list
        printf("I GOT A DISCONNECT!\n");
        struct in_addr addrtmp;
        struct ccp_contact* tempcon = (struct ccp_contact*) malloc (sizeof(struct ccp_contact) );
        
        inet_pton(AF_INET, bufip, &addrtmp);

        memcpy(tempcon->contactIPv4,&addrtmp.s_addr,sizeof(tempcon->contactIPv4));

        char maskOne;
        char maskZero;
        maskZero = (0b0000000011111111 & PORT);
        maskOne = ((0b1111111100000000 & PORT)>>8);
        tempcon->contactPort[0] = maskZero;
        tempcon->contactPort[1] = maskOne;
        put_string_in_sender_receiver(tempcon->contactalias,"DELETE ME");
        print_contact(tempcon);
        remove_contact(*tempcon);
        free(tempcon);
        
        }
    
    if(ccp_data->typeFlags == SEND_A_MESSAGE){ // we reseaved a msg. print it and send a reply
        printf("I GOT A MESSAGE\n");
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        pthread_t helperclient;
        char *tmpmsg = (char *) malloc (MAXCHARACTERS);
        memcpy(tmpmsg,ccp_data->message,MAXCHARACTERS);
        
        tmpdatapaket->portnumber = PORT;

        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias) ;
        
        printf("MSG FROM CLIENT:\n%s",tmpmsg); 
        
        pthread_create(&helperclient,NULL,clientSentMessageReply,(struct datapack*)tmpdatapaket);
        }
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_RECEIVING_MESSAGE){
        set_msg_flag();
        }
    
    if(ccp_data->typeFlags == REQUEST_IF_PEER_IS_ALIVE){
        printf("I GOT A ALIVE REQUEST!\n");
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        
        pthread_t helperclient;
        tmpdatapaket->portnumber = PORT;
        inet_ntop(AF_INET, &clientdata.sin_addr, tmpdatapaket->address, sizeof bufip);
        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias) ;
        pthread_create(&helperclient,NULL,clientSendUpdateReply,(struct datapack*)tmpdatapaket);

        }
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_PEER_IS_STILL_ALIVE){
        printf("I GOT A ALIVE ACK!\n");
        set_up_flag();
        }
    
    return 0;
    }
    
int readFromSocket(int socket, struct sockaddr_in clientdata){
    char* buf = (char*) malloc(maxcharactersize);
    bzero(buf, maxcharactersize);
    msgSize_in_bytes = read(socket, buf, maxcharactersize);
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

    react_to_package(ccp_data, socket , clientdata);
    free(ccp_data);
    free(buf);
    return 0;
    }



//-------------------------------------------------------------------------
static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&flagmutex);
    }   

int set_msg_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[MSG_Flag] = 1;
    pthread_cleanup_pop(1);
    return 0;
    }
int rm_msg_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[MSG_Flag] = 0;
    pthread_cleanup_pop(1);
    return 0;
    }
    
int set_up_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[Update_Flag] = 1;
    pthread_cleanup_pop(1);
    return 0;
    }
    
int rm_up_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[Update_Flag] = 1;
    pthread_cleanup_pop(1);
    return 0;
    }