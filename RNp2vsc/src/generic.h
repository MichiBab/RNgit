#ifndef gen_h
#define gen_h
#define MAXCHARACTERS 1024 
#define PORT 12345 
#define MAXCLIENTS 40
#define maxcharactersize 1060 // ccp paket size
#define WAITTIME 5
#define UPDATE_RESET_TIMER 60
extern int sctp_mode;
#define SOCKET_TYPE (sctp_mode ? SOCK_STREAM : SOCK_STREAM ) // hier hatte ich SOCK_SEQPACKET. Ging dann aber nicht
#define SOCKET_ARG (sctp_mode ? IPPROTO_SCTP : 0)
#endif