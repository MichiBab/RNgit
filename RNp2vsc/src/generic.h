#ifndef gen_h
#define gen_h
#define MAXCHARACTERS 1024 
#define PORT 12345 
#define MAXCLIENTS 40
#define maxcharactersize 1060 // ccp paket size
#define WAITTIME 5
#define UPDATE_RESET_TIMER 60
extern int sctp_mode;
#define DEBUG_MSG(x) if(debugmode==1){ printf("---DBG: %s\n",x);}
#define DEBUG_MSG_NUM(x,d) if(debugmode==1){ printf("---DBG: %s %d\n",x,d);}
#define DEBUG_MSG_STRING(x,s) if(debugmode==1){ printf("---DBG: %s %s\n",x,s);}
extern int debugmode;
#define SOCKET_TYPE (sctp_mode ? SOCK_STREAM : SOCK_STREAM ) 
#define SOCKET_ARG (sctp_mode ? IPPROTO_SCTP : 0)
#endif