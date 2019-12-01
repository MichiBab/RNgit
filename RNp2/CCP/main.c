#include <stdio.h>
#include "server.h"
#include "client.h"
#include "generic.h"
#include <pthread.h>
#include "ccp.h"

#define buffersize 1024
char buffer[buffersize];
char portbuffer[buffersize];
char ipbuffer[buffersize];
char msgbuffer[buffersize];
pthread_t serverthread;
pthread_t clientthread;




/**
 * Hier wird der Client erstellt der einfach nur eine msg schickt.
 */
void* clientArgInit(void* arg){
    struct datapack *data = (struct datapack*) arg;
    client_routine(data->address,data->ccppackage.message, data->portnumber);
    return 0;
    }

void* clientArgInitChatMode(void* arg){
    struct datapack *data = (struct datapack*) arg;
    client_routine_chatmode(*data);
    return 0;
    }


int main(int argc, char **argv)
{
    int exitbool = 1;//abbruch der while schleife
    
    printf("s for create server, c for init a clien, cm for chat mode, cc for close client, h for help, q for exit\n");

    while(exitbool){
        bzero(buffer,buffersize);
        fgets(buffer,buffersize+1,stdin);
        
        if(strcmp(buffer,"s\n") == 0){
            pthread_create(&serverthread,NULL,init_server,NULL);
            }
        
        if(strcmp(buffer,"cc\n") == 0){
            close_client();
            }
        
        if(strcmp(buffer,"c\n") == 0){
            struct datapack paket;
 
            bzero(ipbuffer,buffersize);
            printf("geben sie eine ip addresse fuer den clienten ein\n");
            fgets(ipbuffer,buffersize+1,stdin);
            paket.address = ipbuffer;
            
            printf("geben sie den port ein\n");
            int x;
            scanf("%d", &x);
            paket.portnumber = x;
            while ((getchar()) != '\n'); //clear space
            
            bzero(paket.ccppackage.message,buffersize);
            printf("geben sie eine nachricht an die versendet werden soll\n");
            fgets(paket.ccppackage.message,buffersize+1,stdin);
            pthread_create(&clientthread,NULL,clientArgInit,(struct datapack*)&paket);

            }
            
        if(strcmp(buffer,"cm\n") == 0){
            struct datapack paket;
 
            bzero(ipbuffer,buffersize);
            printf("geben sie eine ip addresse fuer den clienten ein\n");
            fgets(ipbuffer,buffersize+1,stdin);
            paket.address = ipbuffer;
            
            printf("geben sie den port ein\n");
            int x;
            scanf("%d", &x);
            paket.portnumber = x;
            while ((getchar()) != '\n'); //clear space
            
            int firstlock = 1;
            lock();
            pthread_create(&clientthread,NULL,clientArgInitChatMode,(struct datapack*)&paket);
            printf("Write message for the client to send, exit to exit\n");
            while(strcmp(msgbuffer,"exit\n") != 0){
                if(firstlock == 1){
                    firstlock = 0;
                    }
                else{
                    lock();
                    }
                    
                fgets(msgbuffer,buffersize+1,stdin);
                changeMessage(msgbuffer);
                unlock();
                }
            close_client();
            
            
            }
            
        if(strcmp(buffer,"q\n") == 0){
            exitbool = 0;
            }
        
        if(strcmp(buffer,"h\n") == 0){
            printf("s for create server, c for init a client, cm for init client with chat mode,\n cc for close client, h for help, q for exit\n");
            }
        
        }
        pthread_cancel(&serverthread);
        pthread_cancel(&clientthread);
        close_server();
        
        
}


