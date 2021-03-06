#ifndef ccp_h
#define ccp_h
#define maxcontacts 45 //what can fit in message block
#include <arpa/inet.h>

#define contactaliassize 16

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

// maybe with c++: change all to <> type, so everyone has a specific one.
//for example: class datapack: ccp serveraddr socket. class msg extends datapack with msg, and hello extens datapack with address
#pragma pack(push, 1)
struct datapack{ 
    char address[16]; //for send hello
    struct ccp ccppackage; //for all
    uint16_t portnumber; //for send hello
    char receivername[16]; //for send update
    char msg[1024]; //for send msg
    struct sockaddr_in serveraddress; //for client data
    int socketfd; //for client data
    };
#pragma pack(pop)

#pragma pack(push, 1) // exact fit - no padding
struct ccp_contact{
    char contactalias[16];
    uint32_t contactIPv4;
    uint16_t contactPort;
    };
#pragma pack(pop)


#define REQUEST_TO_OPEN_CONNECTION 0b00000001
#define ACKNOWLEDGE_OPENING_CONNECTION 0b00000101
#define PEER_DISCONNECTED 0b00000010
#define SEND_A_MESSAGE 0b00001000
#define ACKNOWLEDGE_RECEIVING_MESSAGE 0b00001100
#define REQUEST_IF_PEER_IS_ALIVE 0b00010000
#define ACKNOWLEDGE_PEER_IS_STILL_ALIVE 0b00010100

#define SOCKETFIELD 0
#define UPDATEFLAG 1
#define MSGFLAG 2
extern struct ccp_contact contactlist[maxcontacts];
extern int contactlist_sockets[maxcontacts][3];
extern char our_username[16];



//CLIENT ROUTINES
int cr_send_hello(struct datapack package); 
int cr_send_hello_reply(struct datapack package); 
int cr_send_update(struct datapack package); 
int cr_send_update_reply(struct datapack package); 
int cr_sent_msg(struct datapack package); 
int cr_sent_msg_reply(struct datapack package); 
int cr_bye(struct datapack package); 

//CCP PAKET SET
int set_ccp_hello(struct ccp* hellopack, char* receivername);
int set_ccp_hello_reply(struct ccp* hellopack, char* receivername);
int set_ccp_update(struct ccp* updatepack, char* receivername);
int set_ccp_update_reply(struct ccp* updatepack, char* receivername);
int set_ccp_message(struct ccp* msgpack, char* message, char* receivername);
int set_ccp_message_reply(struct ccp* msgpack, char* receivername);
int set_ccp_bye(struct ccp* msgpack, char* receivername);

//CCP LIST HANDLING
int print_a_contactlist(struct ccp_contact list[maxcontacts]);
int put_contact_list_in_message_of_ccp(struct ccp* pack);
int put_string_in_sender_receiver(char* array, char* input);
int put_message_in_ccp(struct ccp* pack, char* message);
int create_our_contact(char* ipstring);
int print_contact(struct ccp_contact* con);
int print_my_contactlist();
int remove_contact(struct ccp_contact con);
int merge_lists(struct ccp_contact* clientlist);
int add_contact(struct ccp_contact con);
int setusername(char* username);
int update_contact_list();
int check_if_not_null_contact(struct ccp_contact con);
int return_client_contact_index_through_ip4(uint32_t client_ipv4, struct ccp_contact* clientlist);
int get_ipstring_from_contact(struct ccp_contact con, char* erg);
int add_contact_mutex_locked(struct ccp_contact con);
int add_entrys_to_socket_array(int listindex, int MODE, int data);
int destroy_list_mutex();
int print_connected_contacts();

//servermethods
int react_to_package(struct ccp* pack , struct sockaddr_in clientdata, int socket);

//react routine
int react_to_hello(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket);
int react_to_hello_reply(struct ccp* ccp_data, struct sockaddr_in clientdataint, int socket);
int react_to_update(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket);
int react_to_update_reply( int socket);
int react_to_msg(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket);
int react_to_msg_reply( int socket);
int react_to_bye(struct ccp* ccp_data , struct sockaddr_in clientdata, int socket);

#endif