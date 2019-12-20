#include "generic.h"
#include "ccp_socket_list.h"
#include <pthread.h>
#include "ccp.h"
#include <stdio.h>
#include <string.h>

pthread_mutex_t flagmutex = PTHREAD_MUTEX_INITIALIZER;

char msgflagarray[2];
// 0: socket 1: upd reply 2: msg reply
int contactlist_sockets[maxcontacts][3];

int init_socket_list(){
    for(int i = 0; i<maxcontacts;i++){
        for(int y = 0; y<3;y++){
            contactlist_sockets[i][y] = -1;
        }
    }
}

//return -1, if not in our list. else his index
int retrieve_index_through_socket(int socket){
    for(int i = 0; i < maxcontacts; i++){
        if(contactlist_sockets[i][SOCKETFIELD] = socket){
            return i;
        }    
    }
    return -1;
}

int destroy_socket_mutex(){
    pthread_mutex_destroy(&flagmutex);
}

int remove_contact_in_socket_array_with_index(int index){
    if(index >= maxcontacts || index < 0){
        DEBUG_MSG("ERROR IN REMOVE CONTACT IN SOCKET ARRAY; WRONG IDNEX")return-1;}
    for(int i = 0; i<3; i++){
        contactlist_sockets[index][i] = -1;
    }
    return 0;
}

static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&flagmutex);
    }   

int set_msg_flag(int socket){
    int ret = -1;
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    int index = retrieve_index_through_socket(socket);
    if(index == -1){
        DEBUG_MSG("ccp msg flags: socket not found");
    }
    else{
        contactlist_sockets[index][MSGFLAG] = 1;
        ret = index;
    }
    pthread_cleanup_pop(1);
    return ret;
    }
    
int rm_msg_flag(int socket){
    int ret = -1;
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    int index = retrieve_index_through_socket(socket);
    if(index == -1){
        DEBUG_MSG("ccp msg flags: socket not found");
    }
    else{
        contactlist_sockets[index][MSGFLAG] = 0;
        ret = index;
    }
    pthread_cleanup_pop(1);
    return ret;
    }
    
int set_up_flag(int socket){
    int ret = -1;
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    int index = retrieve_index_through_socket(socket);
    if(index == -1){
        DEBUG_MSG("ccp msg flags: socket not found");
    }
    else{
        contactlist_sockets[index][UPDATEFLAG] = 1;
        ret = index;
    }
    pthread_cleanup_pop(1);
    return ret;
    }
    
int rm_up_flag(int socket){
    int ret = -1;
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    int index = retrieve_index_through_socket(socket);
    if(index == -1){
        DEBUG_MSG("ccp msg flags: socket not found");
    }
    else{
        contactlist_sockets[index][UPDATEFLAG] = 0;
        ret = index;
    }
    pthread_cleanup_pop(1);
    return ret;
    }

int rm_up_flag_indexed(int index){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    contactlist_sockets[index][UPDATEFLAG] = 0;
    pthread_cleanup_pop(1);
    return 0;
}
int rm_msg_flag_indexed(int index){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    contactlist_sockets[index][MSGFLAG] = 0;
    pthread_cleanup_pop(1);
    return 0;
}


    