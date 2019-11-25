#include "server.h"
#include "generic.h"
#include "ccp.h"


int socketfd; //SOCKET FILE DESCRIPTOR
int len; //saving size of a client
struct sockaddr_in servaddr;
struct sockaddr_in clients[MAXCLIENTS];
int clientConnectionfd[MAXCLIENTS];//CONNECTION FILE DESCRIPTORS

static int createSocket(){
    //AF_INET = Address familiy, here IPF4; SOCK STREAM Means TCP connection
    //TODO last input means Protocoll type, 0 means automatically choosen?
    socketfd = socket(AF_INET, SOCK_STREAM, 0); //returns a socket file descriptor
    if (socketfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    //fill the struct serveraddr with 0's
    bzero(&servaddr, sizeof(servaddr)); 
    return 0;
    }
    
static int assign_IP_PORT(){
    //AF_INET = Address familiy
    servaddr.sin_family = AF_INET; 
    //SET MY SERVER IP ADDRESS. INADDR_ANY means it will listen at any interface it can.
    //maybe //inet_addr(SERVERADDRESS); instead of any later. to specify the address
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //SET MY PORT
    servaddr.sin_port = htons(PORT); 
    return 0;
    }

static int bindServersocket(){
    /* Give the socket FD the local address ADDR (which is LEN bytes long).  */
    if ((bind(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
    return 0;
    }
    
static int listenForConnections(){
    /*  Prepare to accept connections on socket FD.
        N connection requests will be queued before further requests are refused.
        Returns 0 on success, -1 for errors.  */
    if ((listen(socketfd, MAXCLIENTS)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(clients[0]); //size of a client
    return 0;
    }
    
    
/* Await a connection on socket FD.
   When a connection arrives, open a new socket to communicate with it,
   set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
   peer and *ADDR_LEN to the address's actual length, and return the
   new socket's descriptor, or -1 for errors. 
   TODO: the client counter is not usable for the end implementation. Just for testing*/
int clientCounter = 0;
static int accept_Incomming_Connection(){ //
    clientConnectionfd[clientCounter] = accept(socketfd, (struct sockaddr*)&clients[clientCounter], &len); 
    if (clientConnectionfd[clientCounter] < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        if(clientCounter>=MAXCLIENTS){
            printf("ERROR: I have too many clients");
            exit(0);
            }
        clientCounter++;
        printf("server acccept the client...\n"); 
    return 0;
    }



static int testingMessageReceive(){
    printf("server: im now in \"testing receive a message\"\n");
    char buffer[MAXCHARACTERS];
    for(int y = 0; y<MAXCLIENTS;y++){//go through the client list maxclient times every 0.1 second
        for(int i = 0; i<MAXCLIENTS; i++){//reading data from every client 
            bzero(buffer,sizeof(buffer));
            //read from socket
            if(clientConnectionfd[i] != 0){//If there is a connection to read from
            //if i client is gone, his index should be put to 0 again to avoid error in read()
                if(read(clientConnectionfd[i], buffer, maxcharactersize) != -1 ){
                    if(buffer!="\0"){
                        printf("From client %d: %s \n", i,buffer);
                        }
                    }
                }
            }
        }
        usleep(100000); //0.1 second
    }
    
        


int init_server(){

    createSocket();
    
    assign_IP_PORT();
    
    bindServersocket();
    
    listenForConnections();
    
    accept_Incomming_Connection();
    
    //FOR TESTING
    testingMessageReceive();
    
    close(socketfd);
    
    return 0;
    }