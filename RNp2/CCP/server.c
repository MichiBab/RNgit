#include "server.h"
#include "generic.h"
#include "ccp.h"


int parentfd; 
int clientfd; 

struct sockaddr_in serveraddr; 
struct sockaddr_in clientaddr;

struct hostent *hostp; /* client host info */

char buffer[maxcharactersize]; /* message buffer */
int msgSize_in_bytes; /* message byte size */



int close_server(){
    close(parentfd);
    }


static int create_socket(){
    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0) {
        printf("ERROR opening socket");
        exit(1);
    }

    /*
    * build the server's Internet address
    */
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
        exit(1);
        }
    return 0;
    }

static int listenSocket(){
    if (listen(parentfd, MAXCLIENTS) < 0) {
        printf("ERROR listening");
        exit(1);
    }
    return 0;
    }

static int readFromSocket(){
    bzero(buffer, maxcharactersize);
    msgSize_in_bytes = read(clientfd, buffer, maxcharactersize);
    if (msgSize_in_bytes < 0) {
        printf("ERROR reading from socket");
        exit(1);
    }
    printf("server received %d bytes: %s\n", msgSize_in_bytes, buffer);
    return 0;
    }

int init_server() {
    
    
    create_socket();

    setIPandPort();
  
    bindSocket();

    listenSocket();

  /* 
   * main loop: wait for a connection request, read input line, 
   * then close connection.
   */
  int clientlen = sizeof(clientaddr);
  while (1) {

    clientfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (clientfd < 0) {
        printf("ERROR accepting a client");
        exit(1);
    }
    
    readFromSocket();

    close(clientfd);
  }
}