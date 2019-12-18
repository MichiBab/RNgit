#include "server.h"
#include "generic.h"
#include <errno.h> 
#include <unistd.h> 
#include <sys/select.h>
#include <pthread.h>

int pipe_fd[2];

int parentfd; 
pthread_mutex_t socket_lock = PTHREAD_MUTEX_INITIALIZER;
struct sockaddr_in serveraddr; 
struct sockaddr_in client_addr[MAXCLIENTS];
static int getMessages();
//--select 
int client_socket[MAXCLIENTS];
int max_sd, activity, sd, new_socket;
fd_set readfds; 
int addrlen = sizeof(serveraddr);

static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&socket_lock);
    }

int close_server(){
    close(parentfd);
    return 0;
    }

static int create_socket(){
    parentfd = socket(AF_INET, SOCKET_TYPE, SOCKET_ARG);
    if (parentfd < 0) {
        printf("ERROR opening socket");
       // exit(1);
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    return 0;
    }
    

static int setIPandPort(){
    /* this is an Internet address */
    serveraddr.sin_family = AF_INET;

    /* let the system figure out our IP address */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* this is the port we will listen on */
    serveraddr.sin_port = htons(PORT);
    
    return 0;
    }
    
static int bindSocket(){
    if (bind(parentfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        printf("ERROR binding socket");
       // exit(1);
        }
    
    return 0;
    }

static int listenSocket(){
    if (listen(parentfd, MAXCLIENTS) < 0) {
        printf("ERROR listening");
       // exit(1);
    }
    return 0;
    }



//initialise all client_socket[] to 0 so not checked  
static int init_clientfds(){
    for (int i = 0; i < MAXCLIENTS; i++){   
        client_socket[i] = 0;   
        }   
    return 0;
    }


static int addchilds(){
    //add child sockets to set  
    pthread_mutex_lock(&socket_lock); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for (int i = 0 ; i < MAXCLIENTS ; i++){
        //socket descriptor
        sd = client_socket[i];
        //if valid socket descriptor then add to read list
        if(sd > 0){
            FD_SET( sd , &readfds);
            }
        //get greatest
        if(sd > max_sd){
            max_sd = sd;
            }
        }
    pthread_cleanup_pop(1);
    return 0;
    }

int add_socket_to_server_array(int socket, struct sockaddr_in cli){
    for (int i = 0; i < MAXCLIENTS; i++){   
        //if position is empty  
        if( client_socket[i] == 0 ) {   
            pthread_mutex_lock(&socket_lock); 
            pthread_cleanup_push(cleanUpMutex,NULL);

            client_addr[i] = cli;
            client_socket[i] = socket;  
            FD_SET(socket,&readfds);
            
            pthread_cleanup_pop(1);
            printf("Adding to list of sockets as index %d ---socket: %d \n" , i, socket);  

            break;   
            }
    }

}

int test_read(){
    getMessages();
    return 0;
}

static int acceptConnections(){
    //If something happened on the master socket ,  
    //then its an incoming connection     
    if (FD_ISSET(parentfd, &readfds)){
        struct sockaddr_in temp;
        int templen = sizeof(struct sockaddr_in);
        printf("trying to accept a socket\n");
        new_socket = accept(parentfd, (struct sockaddr *)&temp, (socklen_t*)&templen);
        if (new_socket < 0){
            printf("error accept\n");
            //exit(1);
            }
        //add new socket to array of sockets  
        for (int i = 0; i < MAXCLIENTS; i++){   
            //if position is empty  
            if( client_socket[i] == 0 ) {   
                pthread_mutex_lock(&socket_lock); 
                pthread_cleanup_push(cleanUpMutex,NULL);

                client_addr[i] = temp;
                client_socket[i] = new_socket;  

                pthread_cleanup_pop(1);
                //printf("Adding to list of sockets as %d\n" , i);   
                break;   
                }
            }
        }
    return 0;
    }
    
static int getMessages(){
    //GET MESSAGES
    //printf("server tries to read messages now\n");
    for (int i = 0; i < MAXCLIENTS; i++){
        sd = client_socket[i];    
        //if there is a valid socket
        if (FD_ISSET( sd , &readfds)){
           //printf("server got a valid socket\n");
                if(readFromSocket(client_socket[i], client_addr[i])){
                   // printf("client number %d is disconnected (read gave 0 bytes back)\n",i);
                    close( sd );
                    pthread_mutex_lock(&socket_lock); 
                    pthread_cleanup_push(cleanUpMutex,NULL);

                    client_socket[i] = 0;

                    pthread_cleanup_pop(1);
                }
            }
        }
    return 0;
    
    }

int init_server() {

    init_clientfds();
    
    create_socket();

    setIPandPort();
  
    bindSocket();

    listenSocket();

    /* 
    * main loop: wait for a connection request, read input line, 
    * then close connection.
    */
    printf("server now in main loop\n");

    //selfpiping to wake up select
    if (pipe(pipe_fd) == -1){
        printf("error creating pipe, shuting down server\n");
        return -1;
    }

    

    while (1) {
      
        //clear the socket set
        FD_ZERO(&readfds);
      
        //add master socket to set  
        FD_SET(parentfd, &readfds);

        //add pipe
        FD_SET(pipe_fd[0], &readfds);
        if(pipe_fd[0] > parentfd){
            max_sd = pipe_fd[0];
        }
        else{
            max_sd = parentfd;
        }

        //add clients
        addchilds();
        
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely
        //printf("im in select\n");
        struct timeval* t = {3,1};
        activity = select( max_sd + 1 , &readfds , NULL , NULL , t);
        //printf("i finished select\n");
        if ((activity < 0) && (errno!=EINTR)){
            //printf("select error \n");
            }
        //printf("accepted something\n");
        if(activity > 0){
            acceptConnections();
            
            }
        getMessages();
    }
}


