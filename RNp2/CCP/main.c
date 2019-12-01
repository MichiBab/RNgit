#include <stdio.h>
#include "server.h"
#include "client.h"
#include "generic.h"
#include <pthread.h>
#include "ccp.h"

#define buffersize 1024
char buffer[buffersize];
char usernamebuffer[16];
char receiverbuffer[16];
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
    
void* clientSendHello(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_connection_establishment(*data);
    return 0;
    }

static int testingstuff(){
    
    struct in_addr addr;
    inet_pton(AF_INET, "127.0.0.1", &addr);
    printf("%d\n", addr.s_addr);
    
    char buf[16];
    inet_ntop(AF_INET, &addr, buf, 16);
    printf("%s\n", buf);
    
    struct ccp_contact con;
    put_string_in_sender_receiver(con.contactalias, "testcontact");
    con.contactIPv4[0] = "1";
    con.contactIPv4[1] = "4";
    con.contactIPv4[2] = "1";
    con.contactIPv4[3] = "5";
    con.contactPort[0] = "8";
    con.contactPort[1] = "8";
    contactlist[0] = con;
    contactlist[1] = con;
    
    
    
    return 0;
    }

int main(int argc, char **argv)
{
    testingstuff();

    int exitbool = 1;//abbruch der while schleife
    
    printf("enter a username for your chat client, the first 16 chars will be accepted\n");
    bzero(usernamebuffer,16);
    fgets(usernamebuffer,16+1,stdin);
    setusername(usernamebuffer);
    
    
    printf("s for create server, c for init a client, cm for chat mode, \ncc for close client, h for help, q for exit, p for print contacts\nch for hello\n");

    while(exitbool){
        bzero(buffer,buffersize);
        fgets(buffer,buffersize+1,stdin);
        
        if(strcmp(buffer,"p\n") == 0){
            print_my_contactlist();
            }
        
        if(strcmp(buffer,"s\n") == 0){
            pthread_create(&serverthread,NULL,init_server,NULL);
            }
        
        if(strcmp(buffer,"ch\n") == 0){
            struct datapack dpaket;
     
           // bzero(paket,sizeof(paket));
            bzero(receiverbuffer,buffersize);
            printf("Geben sie den namen des Empfaengers ein\n");
            fgets(receiverbuffer,buffersize+1,stdin);
            
            bzero(ipbuffer,buffersize);
            printf("geben sie eine ip addresse fuer den clienten ein\n");
            fgets(ipbuffer,buffersize+1,stdin);
            dpaket.address = ipbuffer;
            
            printf("geben sie den port ein\n");
            int x;
            scanf("%d", &x);
            dpaket.portnumber = x;
            while ((getchar()) != '\n'); //clear space
            
            set_ccp_hello(&dpaket.ccppackage, receiverbuffer);
            pthread_create(&clientthread,NULL,clientSendHello,(struct datapack*)&dpaket);
            
            }
        
        if(strcmp(buffer,"cc\n") == 0){
            close_client();
            }
        
        if(strcmp(buffer,"c\n") == 0){
            struct datapack paket;
           // bzero(paket,sizeof(paket));
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
           // bzero(paket,sizeof(paket));
            
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


