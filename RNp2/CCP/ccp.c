#include "ccp.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "server.h"
#include "generic.h"

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
char our_username[16];//GLOBAL


//if all data is 0, there is no contact
//returns 0 if nullcontact, else 1
static int check_if_nullcontact(struct ccp_contact con){
    for(int i = 0; i < 16; i++){
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
    bzero(array,16);
    int buffersize;
    strlen(input)<=17 ? buffersize = strlen(input) : 17;
    snprintf(array,17,input);
    return 0;
    }



int set_ccp_hello(struct ccp* hellopack, char* receivername){
    hellopack->versionnum = 1;
    hellopack->typeFlags = REQUEST_TO_OPEN_CONNECTION;
    for(int i = 0; i<16;i++){
        hellopack->senderAlias[i] = our_username[i];
        }
    put_string_in_sender_receiver(hellopack->receiverAlias,receivername);
    
    put_contact_list_in_message_of_ccp(hellopack);
    
    return 0;
    }

int put_contact_list_in_message_of_ccp(struct ccp* pack){
    for(int i = 0; i<(1024/maxcontacts);i++){
        for(int y = 0; y<22;y++){
            if(y<16){
                pack->message[y+22*i] = contactlist[i].contactalias[y];
                }
            else if(y<20){
                pack->message[y+22*i] = contactlist[i].contactIPv4[y-16];
                }
            else{
                pack->message[y+22*i] = contactlist[i].contactPort[y-20];
                }
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
    }

int create_our_contact(){

    struct ccp_contact *me = (struct ccp_contact*)malloc(sizeof(struct ccp_contact));
    
    put_string_in_sender_receiver(me->contactalias,our_username);

    
    //char* ip_address_fromserver;
    //ip_address_fromserver = malloc( sizeof(char)*4);
   // get_my_ip(ip_address_fromserver);
    
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
    print_contact(&contactlist[0]);
    
    return 0;
    }

int print_my_contactlist(){
    
    return 0;
    }

int setusername(char* username){
    strncpy(our_username, username, 16);
    printf("our username is : %s\n",our_username);
    return 0;
    }
    
int print_contact(struct ccp_contact* con){
    printf("Alias: %s",con->contactalias);
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