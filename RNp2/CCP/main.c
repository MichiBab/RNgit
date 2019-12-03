#include <stdio.h>
#include "server.h"
#include "client.h"
#include "generic.h"
#include <pthread.h>
#include "ccp.h"

#define buffersize 1024
#define addrsize 16
char buffer[buffersize];
char usernamebuffer[16];
char receiverbuffer[16];
char portbuffer[buffersize];
char ipbuffer[addrsize];
char msgbuffer[buffersize];
pthread_t serverthread;


    
void* init_server_thread(void* arg){
     init_server();
     return 0;
     }


//-------------------------------------------------------------------------


int main(int argc, char **argv)
{
    int exitbool = 1;//abbruch der while schleife
    bzero(contactlist,maxcontacts);
    printf("enter a username for your chat client, the first 16 chars will be accepted\n");
    bzero(usernamebuffer,16);
    fgets(usernamebuffer,16+1,stdin);
    setusername(usernamebuffer);
    bzero(ipbuffer,16);
    
    printf("creating server\n");
    pthread_create(&serverthread,NULL,init_server_thread,NULL);

    bzero(ipbuffer,sizeof ipbuffer);
    printf("geben sie eine ip addresse fuer ihren eigenen contact ein\n");
    fgets(ipbuffer,buffersize+1,stdin);
    char *pos;
    if ((pos=strchr(ipbuffer, '\n')) != NULL){//remove newline for ip
        *pos = '\0';
    }
    printf("angegebene ip ----%s----\n",ipbuffer);
    create_our_contact(ipbuffer);

    char* helpmsg = "c for sending messages to a client, ch just for the hello pack, h for help, q for exit, p for print contacts, d for delete contact\n";
    printf("%s",helpmsg);
    while(exitbool){
        bzero(buffer,buffersize);
        fgets(buffer,buffersize+1,stdin);
        
        if(strcmp(buffer,"p\n") == 0){
            print_my_contactlist();
            }
        
        if(strcmp(buffer,"d\n") == 0){
            printf("this is the contact list:\n");
            print_my_contactlist();
            printf("delete one with typing an index now. from 0 to %d\n",maxcontacts-1);
            int x;
            scanf("%d", &x);
            while ((getchar()) != '\n'); //clear space
            if(x>=maxcontacts){
                printf("wrong index\n");
                }
            else{
                remove_contact(contactlist[x]);
                }
            
            }
        
        if(strcmp(buffer,"c\n") == 0){
            pthread_t halloclient;
            struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
    
            bzero(receiverbuffer,sizeof receiverbuffer);
            printf("Geben sie den namen des Empfaengers ein\n");
            fgets(receiverbuffer,buffersize+1,stdin);
            put_string_in_sender_receiver(dpaket->receivername,receiverbuffer); 
            
            bzero(ipbuffer,sizeof ipbuffer);
            printf("geben sie eine ip addresse fuer den clienten ein\n");
            fgets(ipbuffer,buffersize+1,stdin);
            put_string_in_sender_receiver(dpaket->address,ipbuffer);
            
            printf("geben sie den port ein\n");
            int x;
            scanf("%d", &x);
            dpaket->portnumber = x;
            while ((getchar()) != '\n'); //clear space
            
            pthread_create(&halloclient,NULL,clientSendHello,(struct datapack*)dpaket);
            
            //hier muss nun auf das hello reply gewartet werden. ???
            pthread_join(halloclient,0);
        
            //Setup again, because threads clear their memory when finished
            dpaket = (struct datapack*) malloc (sizeof(struct datapack));
            put_string_in_sender_receiver(dpaket->receivername,receiverbuffer); 
            put_string_in_sender_receiver(dpaket->address,ipbuffer);
            dpaket->portnumber = x;
            
            pthread_create(&halloclient,NULL,clientSendUpdate,(struct datapack*)dpaket);
            //hier muss auf das update reply gewartet werden. ???
            pthread_join(halloclient,0);
            
            printf("MESSAGE MODE, TERMINATE WITH exit\n");
  
            
            while(strcmp(msgbuffer,"exit\n") != 0){
                //Setup again, because threads clear their memory when finished
                dpaket = (struct datapack*) malloc (sizeof(struct datapack));
                put_string_in_sender_receiver(dpaket->receivername,receiverbuffer); 
                put_string_in_sender_receiver(dpaket->address,ipbuffer);
                bzero(msgbuffer,sizeof msgbuffer);
                printf("geben sie eine nachricht zum versenden ein:\n");
                fgets(msgbuffer,buffersize+1,stdin);
                if(strcmp(msgbuffer,"exit\n") != 0){
                    strcpy(dpaket->msg, msgbuffer);
                    pthread_create(&halloclient,NULL,clientSentMessage,(struct datapack*)dpaket);
                    pthread_join(halloclient,0);
                    }
                }
            
            }
        
        if(strcmp(buffer,"ch\n") == 0){
            pthread_t halloclient;
            struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
    
            bzero(receiverbuffer,sizeof receiverbuffer);
            printf("Geben sie den namen des Empfaengers ein\n");
            fgets(receiverbuffer,buffersize+1,stdin);
            put_string_in_sender_receiver(dpaket->receivername,receiverbuffer);
            
            bzero(ipbuffer,sizeof ipbuffer);
            printf("geben sie eine ip addresse fuer den clienten ein\n");
            fgets(ipbuffer,buffersize+1,stdin);
            put_string_in_sender_receiver(dpaket->address,ipbuffer);
            
            printf("geben sie den port ein\n");
            int x;
            scanf("%d", &x);
            dpaket->portnumber = x;
            while ((getchar()) != '\n'); //clear space
            
            pthread_create(&halloclient,NULL,clientSendHello,(struct datapack*)dpaket);
            
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
        pthread_cancel(serverthread);
        close_server();
        
        
}


