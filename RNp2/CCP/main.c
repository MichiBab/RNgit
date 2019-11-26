#include <stdio.h>
#include "server.h"
#include "client.h"
#include "generic.h"
#include "testingLocal.h"



#pragma pack(push, 1) // exact fit - no padding
struct ccp{
    char versionnum;
    char typeFlags;
    char reserved[2];
    char senderAlias[16];
    char receiverAlias[16];
    char message[1024];
    };
#pragma pack(pop)





int main(int argc, char **argv)
{
    struct ccp test;
    printf("%d\n",sizeof(test));
    
    put_string_in_sender_receiver(test.senderAlias,"00000000000000012");
    printf("%s \n",test.senderAlias);
    //testing_locally();
}


