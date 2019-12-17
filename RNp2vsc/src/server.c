#include "server.h"
#include "generic.h"
#include <errno.h> 
#include <unistd.h> 
#include <sys/select.h>
#include <pthread.h>

int parentfd; 

struct sockaddr_in serveraddr; 
struct sockaddr_in client_addr[MAXCLIENTS];

//--select 
int client_socket[MAXCLIENTS];
int max_sd, activity, sd, new_socket;
fd_set readfds; 
int addrlen = sizeof(serveraddr);
int  temp_accept_socket, temp_read_socket ;
int i, ready, sock_max, max=-1;
fd_set gesamt_sock, lese_sock;
pthread_mutex_t socket_lock = PTHREAD_MUTEX_INITIALIZER;
//--

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
        printf("ERROR binding socket\n");
       // exit(1);
        }
    
    return 0;
    }

static int listenSocket(){
    if (listen(parentfd, MAXCLIENTS) < 0) {
        printf("ERROR listening\n");
       // exit(1);
    }
    return 0;
    }

//initialise all client_socket[] to 0 so not checked  
static int init_clientfds(){
    for (int i = 0; i < MAXCLIENTS; i++){   
        client_socket[i] = -1;   
        }   
    return 0;
    }


void accept_connection(int *socket, int *new_socket, struct sockaddr_in* client){
   int len;
   len = sizeof(client);
   *new_socket = accept( *socket,(struct sockaddr *)client, &len );
   if (*new_socket  == -1) 
      printf("Fehler bei accept");
      //exit(1);
}

int add_socket_to_server_array(int socket, struct sockaddr_in cli){
    pthread_mutex_lock(&socket_lock); 
    pthread_cleanup_push(cleanUpMutex,NULL);
    for( i=0; i< FD_SETSIZE; i++){
        if(client_socket[i] < 0) {
            printf("server: added socket\n");
            client_socket[i] = temp_accept_socket;
            client_addr[i] = cli;
            break;
            }
         }

    /* Mehr als FD_SETSIZE Clients sind nicht möglich */ 
    if( i == FD_SETSIZE ){
        printf("zu viele verbindungen\n");
        //exit(1);
        }

    /* Den neuen (Socket-)Deskriptor zur
     * (Gesamt-)Menge hinzufügen */
    FD_SET(temp_accept_socket, &gesamt_sock);
    pthread_cleanup_pop(1);
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

    
    sock_max = parentfd;
    FD_ZERO(&gesamt_sock);  
    FD_SET(parentfd, &gesamt_sock);
    struct sockaddr_in client_temp;
    while (1) {
        /* Immer aktualisieren */
        lese_sock = gesamt_sock;

        /* Hier wird auf die Ankunft von Daten oder
         * neuer Verbindungen von Clients gewartet */
        ready = select(sock_max+1, &lese_sock, NULL, NULL, NULL);

        /* Eine neue Clientverbindung ... ? */
        if( FD_ISSET(parentfd, &lese_sock)) {
            accept_connection( &parentfd, &temp_accept_socket, &client_temp );
            
            /* Freien Platz für (Socket-)Deskriptor 
             * in client_sock suchen und vergeben */
            add_socket_to_server_array(temp_accept_socket, client_temp);
            

            /* select() benötigt die höchste 
             * (Socket-)Deskriptor-Nummer */
            if( temp_accept_socket > sock_max ){
                sock_max = temp_accept_socket;
            }

            /* höchster Index für client_sock
            * für die anschließende Schleife benötigt */
            if( i > max ){
                max = i;
            }

            /* ... weitere (Lese-)Deskriptoren bereit? */
            if( --ready <= 0 ){
                continue; //Nein ...
            }
            
        }

        /* Ab hier werden alle Verbindungen von Clients auf
        * die Ankunft von neuen Daten überprüft */
        for(i=0; i<=max; i++) {

            temp_read_socket = client_socket[i];

            if(sd >= 0){
                /* (Socket-)Deskriptor gesetzt ... */   
                if(FD_ISSET(temp_read_socket, &lese_sock)){
                    /* ... dann die Daten lesen */
                    if(readFromSocket(client_socket[i], client_addr[i])){
                        printf("client number %d is disconnected (read gave 0 bytes back)\n",i);
                        close( temp_read_socket );
                        client_socket[i] = -1;
                    }

                }

            }

        }
    }
}



