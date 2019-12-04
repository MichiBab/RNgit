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

pthread_mutex_t listmutex = PTHREAD_MUTEX_INITIALIZER;

static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&listmutex);
    }

//if all data is 0, there is no contact
//returns 0 if nullcontact, else 1
int check_if_nullcontact(struct ccp_contact con){
    for(int i = 0; i < contactaliassize; i++){
        if(con.contactalias[i] != 0){
            return 1;
            }
        if(i<4){
            if(con.contactIPv4[i] != 0){
                return 1;
                }
            }
        if(i<2){
            if(con.contactPort[i] != 0){
                return 1;
                }
            }
        }
    return 0;
    }

//compares ip address and port. ignores alias
//returns 0 on equality
static int compare_contact(struct ccp_contact one, struct ccp_contact two){
     if( (memcmp(one.contactIPv4, two.contactIPv4, sizeof (one.contactIPv4)) == 0) && 
            (memcmp(one.contactPort, two.contactPort, sizeof (one.contactPort)) == 0) ){
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
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for(int i = 0; i<(MAXCHARACTERS/maxcontacts);i++){
        for(int y = 0; y<22;y++){
            if(y<contactaliassize){
                pack->message[y+22*i] = contactlist[i].contactalias[y];
                }
            else if(y<20){
                pack->message[y+22*i] = contactlist[i].contactIPv4[y-contactaliassize];
                }
            else{
                pack->message[y+22*i] = contactlist[i].contactPort[y-20];
                }
            }
        }
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


int update_contact_list(struct ccp_contact* clientlist){
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    int marker[maxcontacts];//marking new contacts
    for(int i = 0; i<maxcontacts;i++){
        marker[i] = 0;
        }
    struct ccp_contact our_tmp;
    struct ccp_contact client_tmp;
    for(int i = 0; i<maxcontacts;i++){
        client_tmp = clientlist[i];
        if(check_if_nullcontact(client_tmp) != 0){//if its valid, compare
            for(int y = 0; y<maxcontacts;y++){
                our_tmp = contactlist[y];
                if(check_if_nullcontact(our_tmp) != 0){
                    if( compare_contact(client_tmp,our_tmp) == 0 ){
                            //if we got it in our list
                            y=maxcontacts; //break out
                            }
                }
                if(y==(maxcontacts-1)){//means its not in our list
                        add_contact(client_tmp);
                        marker[i] = 1;//Mark new connection info
                        }
                }
            
        }
    }

    //send a hello pack to every new entry.
    for(int i = 0; i<maxcontacts;i++){
        if(marker[i] == 1){
            pthread_t helperthread;
            struct datapack* dpaket = (struct datapack*) malloc(sizeof (struct datapack));
            char tmpIP[16];
            uint16_t tmpP;
            printf("new entry from clientlist\n");
            get_ipstring_from_contact(clientlist[i],tmpIP);
            put_string_in_sender_receiver(dpaket->address,tmpIP);
            get_port_int_from_contact(clientlist[i],&tmpP);
            //dpaket->portnumber = tmpP;
            dpaket->portnumber = PORT;
            strcpy(dpaket->receivername,clientlist[i].contactalias);
            pthread_create(&helperthread,NULL,clientSendHello,(struct datapack*)dpaket);
            sleep(1);
            }
        }
    pthread_cleanup_pop(1);
    return 0;
    }

//method adds new contact to the first open spot
int add_contact(struct ccp_contact con){

    for(int i = 0; i < maxcontacts; i++){
        if(check_if_nullcontact(contactlist[i])==0){
            contactlist[i] = con;
            break;
            }
        }

    return 0;
    }



int remove_contact(struct ccp_contact con){
    pthread_mutex_lock(&listmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for(int i = 0; i<maxcontacts;i++){
            if(check_if_nullcontact(contactlist[i]) != 0){
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
    myip = addr.s_addr;
    
    me->contactIPv4[3] = (myip & 0b11111111<<24) >> 24;
    me->contactIPv4[2] = (myip & 0b11111111<<16) >> 16;
    me->contactIPv4[1] = (myip & 0b11111111<<8) >> 8;
    me->contactIPv4[0] = (myip & 0b11111111);
    
    int port = PORT;
    me->contactPort[1] = (port & 0b11111111<<8) >> 8;
    me->contactPort[0] = (port & 0b11111111);
    
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
        if(check_if_nullcontact(list[i])==1){
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
    printf_ipv4(con->contactIPv4);
    printf_port(con->contactPort);
    return 0;
    }
    
int printf_ipv4(char arr[4]){
    char zero = arr[0];
    char one = arr[1];
    char two = arr[2];
    char three = arr[3];
    uint32_t ip_as_integer = 0;
    uint32_t mask1 = 
            0b00000000000000000000000011111111 & zero;
    uint32_t mask2 = 
            0b00000000000000001111111100000000 & one<<8;
    uint32_t mask3 = 
            0b00000000111111110000000000000000 & two<<16;
    uint32_t mask4 = 
            0b11111111000000000000000000000000 & three<<24;
    ip_as_integer = mask1 | mask2 | mask3 | mask4;
    char buf[16];
    inet_ntop(AF_INET, &ip_as_integer, buf, 16);
    printf("IPv4 Address: %s\n", buf);
    return 0;
    }

int printf_port(char arr[2]){
    uint16_t port_as_integer = 0;
    char maskOne = arr[1];
    char maskZero = arr[0];
    uint16_t firstmask = 0b1111111100000000 & (maskOne<<8);
    uint16_t secondmask = 0b0000000011111111 & maskZero;
    port_as_integer = firstmask | secondmask;
    printf("Port: %d\n",port_as_integer);
    return 0;
    }
    
int get_ipstring_from_contact(struct ccp_contact con, char* erg){
    char zero = con.contactIPv4[0];
    char one = con.contactIPv4[1];
    char two = con.contactIPv4[2];
    char three = con.contactIPv4[3];
    uint32_t ip_as_integer = 0;
    uint32_t mask1 = 
            0b00000000000000000000000011111111 & zero;
    uint32_t mask2 = 
            0b00000000000000001111111100000000 & one<<8;
    uint32_t mask3 = 
            0b00000000111111110000000000000000 & two<<16;
    uint32_t mask4 = 
            0b11111111000000000000000000000000 & three<<24;
    ip_as_integer = mask1 | mask2 | mask3 | mask4;
    inet_ntop(AF_INET, &ip_as_integer, erg, 16);
    return 0;
    }
    
int get_port_int_from_contact(struct ccp_contact con, uint16_t* erg){
    uint16_t port_as_integer = 0;
    char maskOne = con.contactPort[1];
    char maskZero = con.contactPort[0];
    uint16_t firstmask = 0b1111111100000000 & (maskOne<<8);
    uint16_t secondmask = 0b0000000011111111 & maskZero;
    port_as_integer = firstmask | secondmask;
    *erg = port_as_integer;
    return 0;
    }