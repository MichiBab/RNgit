#include "ccp.h"

static int put_generic_name_data(struct ccp* pack, char* receivername){
    pack->versionnum = 1;
    put_string_in_sender_receiver(pack->receiverAlias,receivername);
    put_string_in_sender_receiver(pack->senderAlias,our_username);
    return 0;
    }

int set_ccp_hello(struct ccp* hellopack, char* receivername){
    put_generic_name_data(hellopack,receivername);
    hellopack->typeFlags = REQUEST_TO_OPEN_CONNECTION;
    put_contact_list_in_message_of_ccp(hellopack);
    return 0;
    }
    
int set_ccp_hello_reply(struct ccp* hellopack, char* receivername){
    put_generic_name_data(hellopack,receivername);
    hellopack->typeFlags = ACKNOWLEDGE_OPENING_CONNECTION;
    put_contact_list_in_message_of_ccp(hellopack);
    return 0;
    }
    
//msg is unimportant 
int set_ccp_update(struct ccp* updatepack, char* receivername){
    put_generic_name_data(updatepack,receivername);
    updatepack->typeFlags = REQUEST_IF_PEER_IS_ALIVE;
    return 0;
    }  

int set_ccp_update_reply(struct ccp* updatepack, char* receivername){
    put_generic_name_data(updatepack,receivername);
    updatepack->typeFlags = ACKNOWLEDGE_PEER_IS_STILL_ALIVE;
    return 0;
    }

int set_ccp_message(struct ccp* msgpack, char* message, char* receivername){
    put_generic_name_data(msgpack,receivername);
    msgpack->typeFlags = SEND_A_MESSAGE;
    put_message_in_ccp(msgpack,message);
    return 0;
    }
    
int set_ccp_message_reply(struct ccp* msgpack, char* receivername){
    put_generic_name_data(msgpack,receivername);
    msgpack->typeFlags = ACKNOWLEDGE_RECEIVING_MESSAGE;
    return 0;
    
    }

int set_ccp_bye(struct ccp* msgpack, char* receivername){
    put_generic_name_data(msgpack,receivername);
    msgpack->typeFlags = PEER_DISCONNECTED;
    return 0;
    
    }

