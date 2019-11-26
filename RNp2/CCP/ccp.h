#ifndef ccp_h
#define ccp_h
#define maxcontacts 46 //what can fit in message block
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

#pragma pack(push, 1) // exact fit - no padding
struct ccp_contact{
    char contactalias[16];
    char contactIPv4[4];
    char contactPort[2];
    };
#pragma pack(pop)

extern struct ccp_contact contactlist[46];

int put_string_in_sender_receiver(char* array, char* input);

#endif