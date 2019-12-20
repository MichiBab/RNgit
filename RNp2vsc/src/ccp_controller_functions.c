#include "ccp.h"
#include "generic.h"
#include <stdio.h>
#include "ccp_thread_methods.h"
#include "ccp_socket_list.h"
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

//checks if real contact and not a pending one with no connected socket
static int update_contact_with_index(int index){
    
    if(check_if_not_null_contact(contactlist[index])  == 0 || contactlist_sockets[index][SOCKETFIELD] == -1){
        DEBUG_MSG("ERROR IN UPDATE CONTACT WITH INDEX; FAULTY CONTACT") return -1;
    }
    //clear flag first
    rm_up_flag_indexed(index);
    struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
    put_string_in_sender_receiver(dpaket->receivername,contactlist[index].contactalias);
    dpaket->socketfd = contactlist_sockets[index][SOCKETFIELD];
    pthread_t updateclient;
    pthread_create(&updateclient,NULL,clientSendUpdate,(struct datapack*)dpaket);
    //now we send the update. if we dont receive a reply in WAITTIME, delete contact
    int received_flag = 0;
    for(int i = 0; i < WAITTIME;i++){
        if(contactlist_sockets[index][UPDATEFLAG] == 1){
            received_flag = 1;
            break;
        }
        else{
            sleep(1);
        }
    }
    //finished waiting or we received it
    if(received_flag == 0){
        remove_contact(contactlist[index]);
        remove_contact_in_socket_array_with_index(index);
        return -1;
    }
    else{
        return 0;
    }

}

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
    init_socket_list();
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
    DEBUG_MSG("creating server");
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
    for(int i = 0; i < maxcontacts; i++){
        //checks in method if viable contact
        update_contact_with_index(i);
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
    if(x>=maxcontacts || x<0){
        printf("wrong index\n");
        }
    else{
        remove_contact(contactlist[x]);
        remove_contact_in_socket_array_with_index(x);
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
    dpaket->portnumber = PORT;
    pthread_create(&halloclient,NULL,clientSendHello,(struct datapack*)dpaket);
    return 0;
    }
    
int ccp_c_quit(){
    printf("ending program, sending bye to every contact first\n");
    pthread_t byeclient;
    struct datapack* dpaket = (struct datapack*) malloc (sizeof(struct datapack));
    pthread_create(&byeclient,NULL,clientSentBye,(struct datapack*)dpaket);
    pthread_join(byeclient,0);
    pthread_cancel(serverthread);
    close_server();
    pthread_join(serverthread,0);
    destroy_list_mutex();
    destroy_socket_mutex();
    pthread_mutex_destroy(&updatemutex);
    return 0;
    }
    
int ccp_c_messaging(){

    
    /*
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
        strcpy(dpaket->receivername,con.contactalias);
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
        strcpy(dpaket->receivername,con.contactalias);
        get_ipstring_from_contact(con,dpaket->address);
        printf("MESSAGE MODE, TERMINATE WITH exit\n");
        do{
                //Setup again, because threads clear their memory when finished
            dpaket = (struct datapack*) malloc (sizeof(struct datapack));
            strcpy(dpaket->receivername,con.contactalias); 
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
    return 0;*/
    }
