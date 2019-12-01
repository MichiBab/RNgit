#include "ccp.h"
#include <stdio.h>
#include <string.h>


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

//17 to fill the 16th byte with snprintf
int put_string_in_sender_receiver(char* array, char* input){
    bzero(array,16);
    int buffersize;
    strlen(input)<=17 ? buffersize = strlen(input) : 17;
    snprintf(array,17,input);
    }

int print_my_contactlist(){

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


int setusername(char* username){
    strncpy(our_username, username, 16);
    printf("our username is : %s\n",our_username);
    return 0;
    }
    
