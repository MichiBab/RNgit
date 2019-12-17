#include "ccp.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "server.h"
#include "generic.h"
#include "client.h"
#include <pthread.h>

struct ccp_contact contactlist[maxcontacts];//GLOBAL
char our_username[contactaliassize];//GLOBAL
int marker[maxcontacts];//marking new contacts
int contactlist_sockets[maxcontacts][2];

pthread_mutex_t listmutex = PTHREAD_MUTEX_INITIALIZER;

static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&listmutex);
    }
    
int destroy_list_mutex(){
    pthread_mutex_destroy(&listmutex);
    return 0;
    }

//if all data is 0, there is no contact
//returns 0 if nullcontact, else 1
int check_if_not_null_contact(struct ccp_contact con){
    if(con.contactIPv4 != 0){
        return 1;
       }
    if(con.contactPort != 0){
        return 1;
        }
    for(int i = 0; i < contactaliassize; i++){
        if(con.contactalias[i] != 0){
            return 1;
            }
        }
    return 0;
    }

//compares ip address and port. ignores alias
//returns 0 on equality
static int compare_contact(struct ccp_contact one, struct ccp_contact two){
     if( ( one.contactIPv4 == two.contactIPv4) && 
            (one.contactPort == two.contactPort) ){
                return 0;
             }
    return 1;
    }

//17 to fill the 16th byte with snprintf
int put_string_in_sender_receiver(char* array, char* input){
    bzero(array,contactaliassize);
    int buffersize;
    (strlen(input)<=16) ? buffersize = strlen(input) : 16;
    snprintf(array,buffersize+1,"%s",input);
    return 0;
    }
    

int put_contact_list_in_message_of_ccp(struct ccp* pack){
    bzero(pack->message, sizeof (pack->message));
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    memcpy(pack->message,contactlist,sizeof(contactlist));
    pthread_cleanup_pop(1);//remove cleanup handler
    return 0;
    }
    

int put_message_in_ccp(struct ccp* pack, char* message){
    bzero(pack->message,MAXCHARACTERS);
    int max;
    max = (strlen(message) < MAXCHARACTERS) ? strlen(message) : MAXCHARACTERS;
    strncpy(pack->message, message, max);
    return 0;
    }

//sends an update to every new entry
int update_contact_list(){
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for(int i = 0; i<maxcontacts;i++){
        if(marker[i] == 1){
            printf("im in marker with index %d\n",i);
            pthread_t helperthread;

            struct datapack* dpaket = (struct datapack*) malloc(sizeof (struct datapack));
            char tmpIP[16];
            uint16_t tmpP;
            printf("new entry from clientlist, sending a hello\n");
            inet_ntop(AF_INET, &contactlist[i].contactIPv4, tmpIP, 16);
            put_string_in_sender_receiver(dpaket->address,tmpIP);
            dpaket->portnumber = PORT;
            put_string_in_sender_receiver(dpaket->receivername,contactlist[i].contactalias);

            pthread_create(&helperthread,NULL,clientSendHello,(struct datapack*)dpaket);
            }
    }
    for(int i = 0; i < maxcontacts;i++){
        marker[i] = 0;
    }
    pthread_cleanup_pop(1);

    return 0;
    }

int merge_lists(struct ccp_contact clientlist[maxcontacts]){
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for(int i = 0; i<maxcontacts;i++){
        marker[i] = 0;
        }
    struct ccp_contact our_tmp;
    struct ccp_contact client_tmp;
    for(int i = 0; i<maxcontacts;i++){ //hier war -1
        client_tmp = clientlist[i];
        if(check_if_not_null_contact(client_tmp) != 0){//if its valid, compare
            for(int y = 0; y<maxcontacts;y++){ //hier war noch ein -1 bei maxcontact 46
                our_tmp = contactlist[y];
                if(check_if_not_null_contact(our_tmp) != 0){
                    if( compare_contact(client_tmp,our_tmp) == 0 ){
                            //if we got it in our list
                            y=maxcontacts; //break out
                            }
                }
                if(y==(maxcontacts-1)){//means its not in our list //hier war auch -1
                        int spot = add_contact(client_tmp);
                        printf("NEW CONTACT: \n");
                        print_contact(&client_tmp);
                        marker[spot] = 1;//Mark new connection info
                        }
                }
            
        }
    }
    pthread_cleanup_pop(1);
    return 0;
}

//method adds new contact to the first open spot and returns the index. -1 if its already in
int add_contact(struct ccp_contact con){
    for(int i = 0; i < maxcontacts; i++){
        if((check_if_not_null_contact(contactlist[i])==0)){
            contactlist[i] = con;
            return i;
            }
        }

    return -1;
    }

//method adds new contact to the first open spot and returns the index. -1 if its already in
int add_contact_mutex_locked(struct ccp_contact con){

    int index = -1;
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for(int i = 0; i < maxcontacts; i++){
        if((check_if_not_null_contact(contactlist[i])==0)){
            contactlist[i] = con;
            index = i;
            i = maxcontacts; //break
            }
        }
    pthread_cleanup_pop(1);
    return index;
    }

int remove_contact(struct ccp_contact con){
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for(int i = 0; i<maxcontacts;i++){
            if(check_if_not_null_contact(contactlist[i]) != 0){
                if( compare_contact(con,contactlist[i]) == 0 ){
                        bzero( &contactlist[i],sizeof (struct ccp_contact));
                        printf("deleted succsessfully\n");
                        i=maxcontacts;//breakout
                    }
                }
        }
    pthread_cleanup_pop(1);
    return 0;
    }
    

int create_our_contact(char* ipstring){
    struct ccp_contact *me = (struct ccp_contact*)malloc(sizeof(struct ccp_contact));
    put_string_in_sender_receiver(me->contactalias,our_username);
    struct in_addr addr;
    inet_pton(AF_INET, ipstring, &addr);
    uint32_t myip;
    me->contactIPv4 = addr.s_addr;
    me->contactPort = PORT;    
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    add_contact(*me);
    free(me);
    pthread_cleanup_pop(1);
    return 0;
    }

int print_my_contactlist(){
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    print_a_contactlist(contactlist);
    pthread_cleanup_pop(1);
    return 0;
    }
    
int print_a_contactlist(struct ccp_contact list[maxcontacts]){
    for(int i = 0; i<maxcontacts;i++){
        if(check_if_not_null_contact(list[i])==1){
            printf("----------------------\n");
            printf("index: %d\n",i);
            print_contact(&list[i]);
            printf("\n");
            }
        }
    return 0;
    }

int setusername(char* username){
    strncpy(our_username, username, 16);
    for(int i = 0; i<16 ; i++){//clear \n from fgets input
        if(our_username[i] == '\n'){
            our_username[i] = 0;
            }
        }
    printf("our username is : %s\n",our_username);
    return 0;
    }
    
int print_contact(struct ccp_contact* con){
    printf("Alias: %s\n",con->contactalias);
    char buf[16];
    inet_ntop(AF_INET, &con->contactIPv4, buf, 16);
    printf("IPv4 Address: %s\n", buf);
    printf("Port: %d\n", con->contactPort);
    return 0;
    }

int get_ipstring_from_contact(struct ccp_contact con, char* erg){
    inet_ntop(AF_INET, &con.contactIPv4, erg, 16);
    return 0;
}

//-1 if not in client contact list. else return idnex
int return_client_contact_index_through_ip4(uint32_t client_ipv4, struct ccp_contact* clientlist){
    for(int i = 0; i<maxcontacts;i++){
        if(clientlist[i].contactIPv4 == client_ipv4){
            return i;
        }
    }
    return -1;
}

int add_entrys_to_socket_array(int listindex, int MODE, int data){
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);

    contactlist_sockets[listindex][MODE] = data;
    
    pthread_cleanup_pop(1);
}