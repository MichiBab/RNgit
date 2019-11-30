#include <stdio.h>
#include "server.h"
#include "client.h"
#include "generic.h"
#include <pthread.h>

#define buffersize 16
char buffer[buffersize];
char portbuffer[buffersize];
char ipbuffer[buffersize];
char msgbuffer[buffersize];
pthread_t serverthread;
pthread_t clientthread;

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
    
    };
#pragma pack(pop)


/**
 * Hier wird der Client erstellt der einfach nur eine msg schickt.
 */
void* clientArgInit(void* arg){
    struct datapack *data = (struct datapack*) arg;
    client_routine(data->address,data->ccppackage.message, data->portnumber);
    return 0;
    }




int main(int argc, char **argv)
{
    int exitbool = 1;//abbruch der while schleife
    
    printf("s for create server, c for client, h for help, q for exit\n");

    while(exitbool){
        bzero(buffer,buffersize);
        fgets(buffer,buffersize+1,stdin);
        
        if(strcmp(buffer,"s\n") == 0){
            pthread_create(&serverthread,NULL,init_server,NULL);
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

            pthread_create(&serverthread,NULL,clientArgInit,(struct datapack*)&paket);
            }
        if(strcmp(buffer,"q\n") == 0){
            exitbool = 0;
            }
        
        if(strcmp(buffer,"h\n") == 0){
            printf("s for create server, c for client, h for help, q for exit\n");
            }
        
        }
        pthread_cancel(&serverthread);
        pthread_cancel(&clientthread);
        close_server();
        
        
}


