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

    
void* clientSendHello(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_connection_establishment(*data);
    return 0;
    }
    
//if connection is already established! Define receivername for datapack for this one
void* clientSendUpdate(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_update_send(*data);
    return 0;
    }
    
//if connection is already established and update got sent
void* clientSentMessage(void* arg){
    struct datapack *data = (struct datapack*) arg;
    cr_sent_msg(*data);
    return 0;
    }

static int testing(){
    
    struct ccp_contact con = contactlist[0];
    con.contactIPv4[0] = '5';
    struct ccp_contact* newlist = (struct ccp_contact*) malloc(sizeof(contactlist)) ;
    bzero(newlist,maxcontacts);
    newlist[2] = contactlist[0];
    newlist[1] = con;
    con.contactIPv4[0] = '3';
    newlist[0] = con;
    update_contact_list(newlist);
    
    update_contact_list(newlist);
    
    update_contact_list(newlist);
    return 0;
    }

int main(int argc, char **argv)
{
    int exitbool = 1;//abbruch der while schleife
    bzero(contactlist,maxcontacts);
    printf("enter a username for your chat client, the first 16 chars will be accepted\n");
    bzero(usernamebuffer,16);
    fgets(usernamebuffer,16+1,stdin);
    setusername(usernamebuffer);
    
    printf("enter your socket server ip address\n");
    bzero(ipbuffer,16);
    fgets(ipbuffer,16+1,stdin);
    set_serverip_address(ipbuffer);
    bzero(ipbuffer,16);
    create_our_contact();
    
    testing();
    char* helpmsg = "s for create server, cm for chat mode, \ncc for close client, h for help, q for exit, p for print contacts\nch for hello\n";
    printf("%s",helpmsg);
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
            //close_client();
            }
            
        if(strcmp(buffer,"q\n") == 0){
            exitbool = 0;
            }
        
        if(strcmp(buffer,"h\n") == 0){
            printf("%s",helpmsg);
            }
        
        }
        pthread_cancel(&serverthread);
        pthread_cancel(&clientthread);
        close_server();
        
        
}



/*
 * if(strcmp(buffer,"cm\n") == 0){
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
            
            
            }*/ 
            
