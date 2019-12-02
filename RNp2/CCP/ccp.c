#include "ccp.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "server.h"
#include "generic.h"
#define contactaliassize 16
/*struct ccp{
    char versionnum;
    char typeFlags;
    char reserved[2];
    char senderAlias[16];
    char receiverAlias[16];
    char message[1024];
    };

struct ccp_contact{
    char contactalias[16];
    char contactIPv4[4];
    char contactPort[2];
    };*/

struct ccp_contact contactlist[maxcontacts];//GLOBAL
char our_username[contactaliassize];//GLOBAL


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

//17 to fill the 16th byte with snprintf
int put_string_in_sender_receiver(char* array, char* input){
    bzero(array,contactaliassize);
    int buffersize;
    strlen(input)<=17 ? buffersize = strlen(input) : 17;
    snprintf(array,17,input);
    return 0;
    }
    
int set_ccp_hello(struct ccp* hellopack, char* receivername){
    hellopack->versionnum = 1;
    hellopack->typeFlags = REQUEST_TO_OPEN_CONNECTION;
    for(int i = 0; i<contactaliassize;i++){
        hellopack->senderAlias[i] = our_username[i];
        }
    put_string_in_sender_receiver(hellopack->receiverAlias,receivername);
    put_string_in_sender_receiver(hellopack->senderAlias,our_username);
    put_contact_list_in_message_of_ccp(hellopack);
    
    return 0;
    }
    
//msg is unimportant
int set_ccp_update(struct ccp* updatepack, char* receivername){
    updatepack->versionnum = 1;
    updatepack->typeFlags = REQUEST_IF_PEER_IS_ALIVE;
    put_string_in_sender_receiver(updatepack->receiverAlias,receivername);
    put_string_in_sender_receiver(updatepack->senderAlias,our_username);
    return 0;
    }

int set_ccp_message(struct ccp* msgpack, char* message, char* receivername){
    
    msgpack->versionnum = 1;
    msgpack->typeFlags = SEND_A_MESSAGE;
    put_message_in_cpp(msgpack,message);
    
    put_string_in_sender_receiver(msgpack->receiverAlias,receivername);
    put_string_in_sender_receiver(msgpack->senderAlias,our_username);
    
    return 0;
    }

int set_ccp_bye(struct ccp* byepack, char* receivername){
    byepack->versionnum = 1;
    byepack->typeFlags = PEER_DISCONNECTED;
    put_string_in_sender_receiver(byepack->receiverAlias,receivername);
    put_string_in_sender_receiver(byepack->senderAlias,our_username);
    return 0;
    }

int put_contact_list_in_message_of_ccp(struct ccp* pack){
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
    return 0;
    }

int put_message_in_cpp(struct ccp* pack, char* message){
    bzero(pack->message,MAXCHARACTERS);
    int max;
    max = (strlen(message) < MAXCHARACTERS) ? strlen(message) : MAXCHARACTERS;
    strncpy(pack->message, message, max);
    return 0;
    }


int update_contact_list(struct ccp_contact* clientlist){
    int* marker = (int*) malloc(sizeof(int)*maxcontacts);//marking new contacts
    struct ccp_contact our_tmp;
    struct ccp_contact client_tmp;
    for(int i = 0; i<maxcontacts;i++){
        client_tmp = clientlist[i];
        if(check_if_nullcontact(client_tmp) != 0){//if its valid, compare
            for(int y = 0; y<maxcontacts;y++){
                our_tmp = contactlist[y];
                //if(check_if_nullcontact(our_tmp) != 0){
                    if( (strcmp(our_tmp.contactIPv4, client_tmp.contactIPv4) == 0) &&
                        (strcmp(our_tmp.contactPort, client_tmp.contactPort) == 0) ){
                            //if we got it in our list
                            y=maxcontacts; //break out
                            }
                  //  }
                if(y==(maxcontacts-1)){//means its not in our list
                        add_contact(client_tmp);
                        marker[i] = 1;//Mark new connection info
                        }
                }
            }
        }
    //TODO: send a hello paket to every marked entry/ == new contacts    
    for(int i = 0; i<maxcontacts;i++){
        if(marker[i]){
            printf("new entry from clientlist on index: %d\n",i);
            }
        }
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

int create_our_contact(){

    struct ccp_contact *me = (struct ccp_contact*)malloc(sizeof(struct ccp_contact));
    
    put_string_in_sender_receiver(me->contactalias,our_username);

    //char* ip_address_fromserver;
    //ip_address_fromserver = malloc( sizeof(char)*4);
    //get_my_ip(ip_address_fromserver);
    
    struct in_addr addr;
    //hier muss ip_address_fromserver hin
    
    //inet_pton(AF_INET, ip_address_fromserver, &addr);
    inet_pton(AF_INET, serversocket_ip_address, &addr);
    int myip;
    myip = addr.s_addr;
    
    me->contactIPv4[3] = (myip & 0b11111111<<24) >> 24;
    me->contactIPv4[2] = (myip & 0b11111111<<16) >> 16;
    me->contactIPv4[1] = (myip & 0b11111111<<8) >> 8;
    me->contactIPv4[0] = (myip & 0b11111111);
    
    int port = PORT;
    me->contactPort[1] = (port & 0b11111111<<8) >> 8;
    me->contactPort[0] = (port & 0b11111111);
    
    
    add_contact(*me);
    
    return 0;
    }

int print_my_contactlist(){
    
    for(int i = 0; i<maxcontacts;i++){
        if(check_if_nullcontact(contactlist[i])==1){
            print_contact(&contactlist[i]);
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
    printf("----------------------\n");
    printf("Alias: %s\n",con->contactalias);
    printf_ipv4(con->contactIPv4);
    printf_port(con->contactPort);
    return 0;
    }
    
int printf_ipv4(char arr[4]){
    int ip_as_integer = ( (arr[3] << 24 ) | (arr[2] << 16 ) | ( arr[1] << 8 ) | arr[0] );
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
    int ip_as_integer = ( (con.contactIPv4[3] << 24 ) | (con.contactIPv4[2] << 16 ) | ( con.contactIPv4[1] << 8 ) | con.contactIPv4[0] );
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