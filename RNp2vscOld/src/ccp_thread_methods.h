#ifndef threadmethods_h
#define threadmethods_h
void* clientSendHello(void* arg);
void* clientSendUpdate(void* arg);
void* clientSentMessage(void* arg);
void* clientSentBye(void* arg);
void* clientSendHelloReply(void* arg);
void* clientSendUpdateReply(void* arg);
void* clientSentMessageReply(void* arg);
#endif