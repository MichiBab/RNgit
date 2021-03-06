#include "ccp.h"
#include "generic.h"
#include <stdio.h>
#include "ccp_thread_methods.h"
#include "msgflags.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "ccp_controller_functions.h"
#define buffersize 1024
#define addrsize 16

char buffer[buffersize];
char usernamebuffer[16];
char receiverbuffer[16];
char portbuffer[buffersize];
char ipbuffer[addrsize];
char msgbuffer[buffersize];
char *pos;
int sctp_mode = 0;

pthread_t serverthread;

//only one update at a time is allowed.
//else the reply flag could get problems.
//for example: the background task sends an update all
//im waiting in message mode for an reply.
//now i got a reply, but its from the wrong one, not from
//the one im messaging but from the one in update all.
//its just a "got reply" flag, not specified from where.
pthread_mutex_t updatemutex = PTHREAD_MUTEX_INITIALIZER;


void* init_server_thread(void* arg){
     init_server();
     return 0;
     }
     
//updating all contacts every 60 seconds
void* init_timer_updater(void* arg){
    while(1){
        for(int i = 0; i < UPDATE_RESET_TIMER;i++){
            sleep(1);
            }
        ccp_c_update_all();
        }
    }
    
int ccp_c_init_program(){
    printf("Type 1 to enter SCTP Mode\n");
    bzero(usernamebuffer,16);
    fgets(usernamebuffer,16+1,stdin);
    if(strcmp(usernamebuffer,"1\n") == 0){
        printf("SCTP Mode enabled\n");
        sctp_mode = 1;
        }
    
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
    create_our_contact(ipbuffer);
    pthread_t updateclient;
    pthread_create(&updateclient,NULL,init_timer_updater,NULL);
    return 0;
    }

int ccp_c_print_list(){
    print_my_contactlist();
    return 0;
    }
    
static void cleanUpMutex(void* arg){
            pthread_mutex_unlock(&updatemutex);
        }
    
int ccp_c_update_all(){
    for(int i = 1; i<maxcontacts-1;i++){ // 0 is our contact
        struct ccp_contact con = contactlist[i];        
        if(check_if_not_null_contact(con)){
            print_contact(&con);
            sleep(2);
            pthread_t halloclient;
            struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
            //now we send a update to con and wait for him to reply to the update
            dpaket->portnumber = PORT;
            strcpy(dpaket->receivername,con.contactIPv4);
            get_ipstring_from_contact(con,dpaket->address);
            pthread_mutex_lock(&updatemutex); 
            pthread_cleanup_push(cleanUpMutex,NULL);
            pthread_create(&halloclient,NULL,clientSendUpdate,(struct datapack*)dpaket);
            printf("waiting for update reply (maximum wait time: %d)\n",WAITTIME);
            for(int i = 0; i < WAITTIME;i++){//waiting for package update reply
                if(msgflagarray[Update_Flag]){
                    i = WAITTIME;
                    }
                else{
                    sleep(1);
                    }
                }
            if(msgflagarray[Update_Flag] != 1){
                printf("failed to get the update reply. Removing this contact now:\n");
                print_contact(&con);
                remove_contact(con);
                }
            pthread_join(halloclient,0);
            pthread_cleanup_pop(1);
            } 
        }  
    return 0;
    }

int ccp_c_delete(){
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
    return 0;
    }
    
int ccp_c_send_hello(){
    pthread_t halloclient;
    struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
    bzero(receiverbuffer,sizeof receiverbuffer);
    printf("Geben sie den namen des Empfaengers ein\n");
    fgets(receiverbuffer,buffersize+1,stdin);
    if ((pos=strchr(receiverbuffer, '\n')) != NULL){//remove newline for ip
        *pos = '\0';
        }   
    put_string_in_sender_receiver(dpaket->receivername,receiverbuffer); 
    bzero(ipbuffer,sizeof ipbuffer);
    printf("geben sie eine ip addresse fuer den clienten ein\n");
    fgets(ipbuffer,buffersize+1,stdin);
    if ((pos=strchr(ipbuffer, '\n')) != NULL){//remove newline for ip
        *pos = '\0';
        }   
    put_string_in_sender_receiver(dpaket->address,ipbuffer);
    printf("geben sie den port ein\n");
    int x;
    scanf("%d", &x);
    dpaket->portnumber = x;
    while ((getchar()) != '\n'); //clear space
    pthread_create(&halloclient,NULL,clientSendHello,(struct datapack*)dpaket);
    return 0;
    }
    
int ccp_c_quit(){
    pthread_t halloclient;
    struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
    pthread_create(&halloclient,NULL,clientSentBye,(struct datapack*)dpaket);
    pthread_join(halloclient,0);
    pthread_cancel(serverthread);
    close_server();
    pthread_join(serverthread,0);
    destroy_list_mutex();
    pthread_mutex_destroy(&updatemutex);
    return 0;
    }
    
int ccp_c_messaging(){
    printf("Schreiben sie einen Index, um dem Kontakt eine Nachricht zu senden\n");
    pthread_t halloclient;
    struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
    print_my_contactlist();
    msgflagarray[Update_Flag] = 0;
    msgflagarray[MSG_Flag] = 0;
    int x;
    int exitflag = 0;
    scanf("%d", &x);
    while ((getchar()) != '\n'); //clear space
    if(x>=maxcontacts){
        printf("wrong index\n");
        }
    else{
        struct ccp_contact con = contactlist[x];
        if(check_if_not_null_contact(con) == 0){
            printf("wrong index\n");
            return 0;
            }
        //now we send a update to con and wait for him to reply to the update
        dpaket->portnumber = PORT;
        strcpy(dpaket->receivername,con.contactIPv4);
        get_ipstring_from_contact(con,dpaket->address);
        pthread_mutex_lock(&updatemutex); 
        pthread_cleanup_push(cleanUpMutex,NULL);
        pthread_create(&halloclient,NULL,clientSendUpdate,(struct datapack*)dpaket);
        printf("waiting for update reply (maximum wait time: %d)\n",WAITTIME);
        for(int i = 0; i < WAITTIME;i++){//waiting for package update reply
            if(msgflagarray[Update_Flag]){
                i = WAITTIME;
                }
            else{
                sleep(1);
                }
            }
        if(msgflagarray[Update_Flag] != 1){ 
            exitflag = 1;
        }
        pthread_cleanup_pop(1);
        if(exitflag){
            printf("failed to get the update reply. Removing this contact now:\n");
            print_contact(&con);
            remove_contact(con);
            return 0;
            }
        printf("i got an update reply and thread is stopped, trying to msg now\n");
        rm_up_flag();
        pthread_t msgclient;
            //NOW U CAN COMMUNICATE
        struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
            //now we send a update to con and wait for him to reply to the update
        dpaket->portnumber = PORT;
        strcpy(dpaket->receivername,con.contactIPv4);
        get_ipstring_from_contact(con,dpaket->address);
        printf("MESSAGE MODE, TERMINATE WITH exit\n");
        do{
                //Setup again, because threads clear their memory when finished
            dpaket = (struct datapack*) malloc (sizeof(struct datapack));
            strcpy(dpaket->receivername,con.contactIPv4); 
            get_ipstring_from_contact(con,dpaket->address);
            bzero(msgbuffer,sizeof msgbuffer);
            printf("geben sie eine nachricht zum versenden ein:\n");
            fgets(msgbuffer,buffersize+1,stdin);
            if(strcmp(msgbuffer,"exit\n") != 0){
                strcpy(dpaket->msg, msgbuffer);
                dpaket->portnumber = PORT;
                pthread_create(&msgclient,NULL,clientSentMessage,(struct datapack*)dpaket);
                pthread_join(halloclient,0);
                printf("waiting for msg reply (maximum wait time: %d)\n",WAITTIME);
                for(int i = 0; i < WAITTIME;i++){//waiting for package update reply
                    if(msgflagarray[MSG_Flag]){
                        i = WAITTIME;
                        }
                    else{
                        sleep(1);
                        printf(".");
                        }
                    }
                if(msgflagarray[MSG_Flag] != 1){
                    //end of communication, he left.
                    strcpy(msgbuffer,"exit\n");
                    return 0;
                    }
                else{
                    rm_msg_flag();
                    //we can continue to talk
                    }
                }
            }while(strcmp(msgbuffer,"exit\n") != 0);   
        }
    return 0;
    }
