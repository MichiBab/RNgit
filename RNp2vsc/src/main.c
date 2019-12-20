#include <stdio.h>
#include "ccp_controller_functions.h"
#include <string.h>
#include "generic.h"


int debugmode = 0;

#define buffersize 1024
#define addrsize 16
char buffer[buffersize];


int main(int argc, char **argv){
    int exitbool = 1;
    ccp_c_init_program();

    char* helpmsg = "c for the hello pack, h for help, q for exit, m for messaging a contact, p for print contacts, d for delete contact, u for update all, dbg for toggling debug mode\n";
    printf("%s",helpmsg);
    
    while(exitbool){
        bzero(buffer,buffersize);
        fgets(buffer,buffersize+1,stdin);
        
        if(strcmp(buffer,"c\n") == 0){ 
           ccp_c_send_hello();
        }
        
        if(strcmp(buffer,"p\n") == 0){
            ccp_c_print_list();
        }
        
        if(strcmp(buffer,"d\n") == 0){
            ccp_c_delete();
        }
        
        if(strcmp(buffer,"m\n") == 0){
            ccp_c_messaging();
        }
            
        if(strcmp(buffer,"q\n") == 0){
            ccp_c_quit();
            exitbool = 0;
        }
            
        if(strcmp(buffer,"u\n") == 0){
            ccp_c_update_all();
        }
            
        if(strcmp(buffer,"h\n") == 0){
            printf("%s",helpmsg);
        }
        
        if(strcmp(buffer,"dbg\n") == 0){
            if(debugmode){
                printf("turning off debug mode\n");
                debugmode=0;
            }
            else{
                printf("turning on debug mode\n");
                debugmode = 1;
            }
        }
            
        
        }
        return 0;
}


