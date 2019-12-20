#include "ccp.h"
#include "generic.h"
#include <pthread.h>
#include "ccp_thread_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ccp_socket_list.h"
#include <unistd.h> 

static int setup_datapackage(struct datapack* tmpdatapaket, char* alias, struct sockaddr_in clientdata, int socket){
    // not needed, cause we work on running sockets in the react routine
    //char bufip[INET_ADDRSTRLEN];
    //inet_ntop(AF_INET, &clientdata.sin_addr, bufip, sizeof bufip);
    tmpdatapaket->portnumber = PORT;
    tmpdatapaket->socketfd = socket;
    //put_string_in_sender_receiver(tmpdatapaket->address,bufip); 
    put_string_in_sender_receiver(tmpdatapaket->receivername, alias);
    return 0;
    }

static int react_to_listdata(struct sockaddr_in clientdata, int socket, struct ccp_contact* ccp_contact_newlist){

    //first we need to find his contact in his own list
    int his_con_index = 
        return_client_contact_index_through_ip4(clientdata.sin_addr.s_addr, ccp_contact_newlist);
    if(his_con_index == -1){
        DEBUG_MSG("we did not find him in his own contact list");
        return -1;
    }
    //now we add him to our list, so we dont send a hello to him
    int his_con_in_our_list_index = 
        add_contact_mutex_locked(ccp_contact_newlist[his_con_index]);
    if(his_con_in_our_list_index == -1){
        //IF HES IN OUR LIST, WE WILL HAVE TO RETRIEVE HIS INDEX AND PUT HIM IN ADD ENTRYS
        his_con_in_our_list_index = 
            return_client_contact_index_through_ip4(clientdata.sin_addr.s_addr, contactlist);
        if(his_con_in_our_list_index == -1){DEBUG_MSG("error in react to listdata") return -1;}
    }

    //now we mark him and his socket in our global socket_array
    add_entrys_to_socket_array(his_con_in_our_list_index, SOCKETFIELD, socket);
    
    printf("We got a new Client on socket %d\n",his_con_in_our_list_index);
    //JUST UPDATE OUR CONTACT LIST. 
    merge_lists(ccp_contact_newlist);//we will ignore him here now, cause he got already added before.
    update_contact_list(ccp_contact_newlist); 

    return 0;

}

int react_to_hello(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket){
    printf("I GOT A HELLO from socket %d\n",socket);   
    struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
    struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (sizeof(contactlist));
    pthread_t helperclient;
    memcpy(ccp_contact_newlist,ccp_data->message,sizeof(contactlist));

    react_to_listdata(clientdata, socket, ccp_contact_newlist);

    setup_datapackage(tmpdatapaket,ccp_data->senderAlias, clientdata, socket);

    put_contact_list_in_message_of_ccp(&tmpdatapaket->ccppackage); //send our new contactlist back

    pthread_create(&helperclient,NULL,clientSendHelloReply,(struct datapack*)tmpdatapaket);
    free(ccp_contact_newlist);
    return 0;
}

int react_to_hello_reply(struct ccp* ccp_data, struct sockaddr_in clientdata, int socket){
    DEBUG_MSG("I GOT A HELLO REPLY!");
    struct ccp_contact *ccp_contact_newlist = (struct ccp_contact *) malloc (sizeof(contactlist));
    memcpy(ccp_contact_newlist,ccp_data->message,sizeof(contactlist));
    react_to_listdata(clientdata, socket, ccp_contact_newlist);
    free(ccp_contact_newlist);
    
    return 0;
    }
    
int react_to_update(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket){
    DEBUG_MSG("I GOT AN ALIVE REQUEST!");
    struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
    pthread_t helperclient;
    setup_datapackage(tmpdatapaket, ccp_data->senderAlias, clientdata, socket);
    pthread_create(&helperclient,NULL,clientSendUpdateReply,(struct datapack*)tmpdatapaket);
    return 0;
    }
    
int react_to_update_reply(int socket){
    DEBUG_MSG("I GOT AN ALIVE ACK!");
    int index = 
        retrieve_index_through_socket(socket);
    if(index == -1){DEBUG_MSG("error in react to update reply getting the index of the socket")return -1;}
    set_up_flag(index);
    return 0;
    }
    
int react_to_msg(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket){
    DEBUG_MSG("I GOT A MESSAGE");
    struct datapack* tmpdatapaket  = (struct datapack *) malloc (sizeof(struct datapack));
    pthread_t helperclient;
    char *tmpmsg = (char *) malloc (MAXCHARACTERS);
    memcpy(tmpmsg,ccp_data->message,MAXCHARACTERS);
    setup_datapackage(tmpdatapaket,ccp_data->senderAlias,clientdata, socket);
    printf("MSG FROM CLIENT ON SOCKET %d:--------------------------\n%s\n--------------------------\n"
                                        ,socket,tmpmsg); 
    pthread_create(&helperclient,NULL,clientSentMessageReply,(struct datapack*)tmpdatapaket);
    return 0;
    }
    
int react_to_msg_reply( int socket){
    DEBUG_MSG("I GOT A ALIVE ACK");
    int index = 
        retrieve_index_through_socket(socket);
    if(index == -1){DEBUG_MSG("error in react to msg reply getting the index of the socket")return -1;}
    set_msg_flag(index);

    return 0;
    }
    
int react_to_bye(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket){
    DEBUG_MSG("I GOT A DISCONNECT!\n");
    int index = 
        retrieve_index_through_socket(socket);
    if(index == -1){DEBUG_MSG("error in react to bye getting the index of the socket")return -1;}
    //remove from normal list
    remove_contact(contactlist[index]);
    //remove from socket list
    remove_contact_in_socket_array_with_index(index);
    //free socket
    close(socket);

    }