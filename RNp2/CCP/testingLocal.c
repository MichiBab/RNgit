#include "testingLocal.h"
#include "server.h"
#include "client.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
pthread_t myclient;
pthread_t myclient2;
pthread_t myserver;

struct data{
    
    char* address;
    char* message;
    
    };

//---------SERVER----------------------
void* createServer(void* unused){
    init_server();
    }
//-------------------------------------




//---------CLIENT----------------------

void* createClient(void* datastruct){
    init_client();
    struct data *mydata = (struct data*)datastruct;
    connect_to_server(mydata->address);
    usleep(100000);
    for(int i = 0; i<5;i++){
        testing_sendMessage(mydata->message);
        usleep(100000); // equals 0.1 seconds);
    }
    close_client();
    return 0;
    }

//-------------------------------------

int testing_locally(){
    //server
    pthread_create(&myserver,NULL,createServer,0);
    sleep(2); // unclean. waiting for server to finish his init properly.
    
    //client
    struct data myData;
    myData.address = "127.0.0.1";
    myData.message = "Hello, i am a Client";
    pthread_create(&myclient,NULL,createClient,(struct data*)&myData);
    pthread_join(myserver,0);
    pthread_join(myclient,0);
    return 0;
    }

    
