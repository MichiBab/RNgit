#include "ccp.h"
#include <stdio.h>
#include <string.h>

struct ccp_contact contactlist[46];

//17 to fill the 16th byte with snprintf
int put_string_in_sender_receiver(char* array, char* input){
    bzero(array,16);
    int buffersize;
    strlen(input)<=17 ? buffersize = strlen(input) : 17;
    snprintf(array,17,input);
    }


