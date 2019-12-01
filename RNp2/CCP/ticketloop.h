#ifndef server_h
#define server_h
#include <pthread.h>
typedef struct ticket_lock {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    unsigned long queue_head, queue_tail;
} ticket_lock_t;

#define TICKET_LOCK_INITIALIZER { PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER }

void ticket_lock(ticket_lock_t *ticket);

void ticket_unlock(ticket_lock_t *ticket);

#endif