#include "ccp.h"
#include "server.h"
#include "generic.h"
#include <pthread.h>
#include <unistd.h>
#include "ccp_thread_methods.h"

char buffer[maxcharactersize]; /* message buffer */
int msgSize_in_bytes; /* message byte size */

/*struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};*/

int react_to_package(struct ccp* ccp_data, int socket , struct sockaddr_in clientdata){
    //printf("IM IN REACT TO PACKAGE\n");
    if(ccp_data->versionnum != '1'){
       // printf("Wrong version from paket detected!\n");
      //  return 1;
        }
    
    char bufip[INET_ADDRSTRLEN] = ""; //GET IP FROM EXTERN CLIENT
    
    inet_ntop(AF_INET, &clientdata.sin_addr, bufip, sizeof bufip);
   
            
        
    printf("CLIENT IP: %s\n",bufip);
    printf("CLIENT PORT: %d\n", ntohs(clientdata.sin_port));
    if(ccp_data->typeFlags == REQUEST_TO_OPEN_CONNECTION){
            
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (MAXCHARACTERS);
        pthread_t helperclient;
        memcpy(ccp_contact_newlist,ccp_data->message,MAXCHARACTERS);
        update_contact_list(ccp_contact_newlist);
        tmpdatapaket->portnumber = PORT;
        //inet_ntop(AF_INET, &clientdata.sin_addr, tmpdatapaket->address, sizeof bufip);
        put_string_in_sender_receiver(tmpdatapaket->address,bufip);
        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias);
        put_contact_list_in_message_of_ccp(&tmpdatapaket->ccppackage); //send our new contactlist back
        pthread_create(&helperclient,NULL,clientSendHelloReply,(struct datapack*)tmpdatapaket);
        printf("I GOT A HELLO!\n");
        //free(ccp_contact_newlist);
        //free(tmpdatapaket);
        }
    
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_OPENING_CONNECTION){ // i sent a hello and got a hello reply back
        struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (MAXCHARACTERS);
        memcpy(ccp_contact_newlist,ccp_data->message,MAXCHARACTERS);
        printf("I GOT A HELLO REPLY!\n");
        update_contact_list(ccp_contact_newlist); //JUST UPDATE OUR CONTACT LIST. UPDATE CONTACT LIST HANDLES NEW HELLOs
        //TODO MESSAGE
        }
        
    if(ccp_data->typeFlags == PEER_DISCONNECTED){ //removing this contact from our list
        printf("I GOT A DISCONNECT!\n");
        struct in_addr addrtmp;
        struct ccp_contact* tempcon = (struct ccp_contact*) malloc (sizeof(struct ccp_contact) );
        
        inet_pton(AF_INET, bufip, &addrtmp);
        printf("%d\n", addrtmp.s_addr);
        memcpy(tempcon->contactIPv4,&addrtmp.s_addr,sizeof(tempcon->contactIPv4));
        int16_t porttmp;
        porttmp = clientdata.sin_port;
        char maskOne;
        char maskZero;
        maskZero = (0b0000000011111111 & PORT);
        maskOne = ((0b1111111100000000 & PORT)>>8);
        tempcon->contactPort[0] = maskZero;
        tempcon->contactPort[1] = maskOne;
        put_string_in_sender_receiver(tempcon->contactalias,"DELETE ME");
        print_contact(tempcon);
        remove_contact(*tempcon);
        /*uint16_t porttmp;
        porttmp = htons(clientdata.sin_port);
        printf("%d\n", porttmp);
        char maskOne;
        char maskZero;
        maskZero = (0b0000000011111111 & porttmp) & 0b11111111;
        maskOne = ((0b1111111100000000 & porttmp<<8)>>8) & 0b11111111;
        tempcon->contactPort[0] = maskZero;
        tempcon->contactPort[1] = maskOne;
        put_string_in_sender_receiver(tempcon->contactalias,"DELETE ME");
        print_contact(tempcon);
        */
        free(tempcon);
        //todo
        
        }
    
    if(ccp_data->typeFlags == SEND_A_MESSAGE){ // we reseaved a msg. print it and send a reply
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        pthread_t helperclient;
        char *tmpmsg = (char *) malloc (MAXCHARACTERS);
        memcpy(tmpmsg,ccp_data->message,MAXCHARACTERS);
        printf("MSG FROM CLIENT:\n%s",tmpmsg); 
        
        tmpdatapaket->portnumber = PORT;

        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias) ;
        pthread_create(&helperclient,NULL,clientSentMessageReply,(struct datapack*)tmpdatapaket);
        //free(tmpdatapaket);
        //free(tmpmsg);
        }
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_RECEIVING_MESSAGE){
        printf("I GOT A MSG ACK!\n");
        //todo???
        }
    
    if(ccp_data->typeFlags == REQUEST_IF_PEER_IS_ALIVE){
        struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
        pthread_t helperclient;
        tmpdatapaket->portnumber = PORT;
        inet_ntop(AF_INET, &clientdata.sin_addr, tmpdatapaket->address, sizeof bufip);
        put_string_in_sender_receiver(tmpdatapaket->receivername,ccp_data->senderAlias) ;
        pthread_create(&helperclient,NULL,clientSendUpdateReply,(struct datapack*)tmpdatapaket);
        printf("I GOT A ALIVE REQUEST!\n");
        //free(tmpdatapaket);
        }
    
    if(ccp_data->typeFlags == ACKNOWLEDGE_PEER_IS_STILL_ALIVE){
        printf("I GOT A ALIVE ACK!\n");
        
        //todo???
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
    
    printf("received data. now printing it\n");
    printf("sender alias: %s\n",ccp_data->senderAlias);
    printf("receiver alias: %s\n",ccp_data->receiverAlias);
    
    react_to_package(ccp_data, socket , clientdata);
    free(ccp_data);
    free(buf);
    return 0;
    }