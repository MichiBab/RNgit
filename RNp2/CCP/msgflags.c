#include "generic.h"
#include "msgflags.h"
#include <pthread.h>

pthread_mutex_t flagmutex = PTHREAD_MUTEX_INITIALIZER;

char msgflagarray[2];

static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&flagmutex);
    }   

int set_msg_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[MSG_Flag] = 1;
    pthread_cleanup_pop(1);
    return 0;
    }
    
int rm_msg_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[MSG_Flag] = 0;
    pthread_cleanup_pop(1);
    return 0;
    }
    
int set_up_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[Update_Flag] = 1;
    pthread_cleanup_pop(1);
    return 0;
    }
    
int rm_up_flag(){
    pthread_mutex_lock(&flagmutex); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    msgflagarray[Update_Flag] = 0;
    pthread_cleanup_pop(1);
    return 0;
    }