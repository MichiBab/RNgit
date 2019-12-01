#include "ticketloop.h"

void ticket_lock(ticket_lock_t *ticket)
{
    unsigned long queue_me;

    pthread_mutex_lock(&ticket->mutex);
    queue_me = ticket->queue_tail++;
    while (queue_me != ticket->queue_head)
    {
        pthread_cond_wait(&ticket->cond, &ticket->mutex);
    }
    pthread_mutex_unlock(&ticket->mutex);
}

void ticket_unlock(ticket_lock_t *ticket)
{
    pthread_mutex_lock(&ticket->mutex);
    ticket->queue_head++;
    pthread_cond_broadcast(&ticket->cond);
    pthread_mutex_unlock(&ticket->mutex);
}