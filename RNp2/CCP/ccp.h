#ifndef ccp_h
#define ccp_h
#define maxcontacts 46 //what can fit in message block
#include <arpa/inet.h>

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


#pragma pack(push, 1)
struct datapack{
    char* address;
    struct ccp ccppackage;
    int portnumber;
    char* receivername; //for send update
    char* msg; //for send msg
    struct sockaddr_in serveraddress;
    int socketfd; 
    };
#pragma pack(pop)

#pragma pack(push, 1) // exact fit - no padding
struct ccp_contact{
    char contactalias[16];
    char contactIPv4[4];
    char contactPort[2];
    };
#pragma pack(pop)


#define REQUEST_TO_OPEN_CONNECTION 0b00010000
#define ACKNOWLEDGE_OPENING_CONNECTION 0b00010001
#define PEER_DISCONNECTED 0b00001000
#define SEND_A_MESSAGE 0b00000100
#define ACKNOWLEDGE_RECEIVING_MESSAGE 0b00000101
#define REQUEST_IF_PEER_IS_ALIVE 0b00000010
#define ACKNOWLEDGE_PEER_IS_DEAD 0b00000011



extern struct ccp_contact contactlist[maxcontacts];
extern char our_username[16];

int printf_port(char arr[2]);
int printf_ipv4(char arr[4]);

int cr_connection_establishment(struct datapack package); //defined in client
int cr_update_send(struct datapack package); //defined in client
int cr_sent_msg(struct datapack package); //defined in client
int cr_bye(struct ccp ccp_pack); //defined in client

int set_ccp_hello(struct ccp* hellopack, char* receivername);
int set_ccp_update(struct ccp* updatepack, char* receivername);
int set_ccp_message(struct ccp* msgpack, char* message, char* receivername);
int set_ccp_bye(struct ccp* byepack, char* receivername);

int put_contact_list_in_message_of_ccp(struct ccp* pack);
int put_string_in_sender_receiver(char* array, char* input);

int create_our_contact();
int print_contact(struct ccp_contact* con);
int print_my_contactlist();

int add_contact(struct ccp_contact con);
int setusername(char* username);
int put_message_in_cpp(struct ccp* pack, char* message);
int update_contact_list(struct ccp_contact* clientlist);
int check_if_nullcontact(struct ccp_contact con);

int get_ipstring_from_contact(struct ccp_contact con, char* erg);
int get_port_int_from_contact(struct ccp_contact con, uint16_t* erg);
#endif