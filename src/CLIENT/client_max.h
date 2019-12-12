#ifndef CLIENT_MAX
#define CLIENT_MAX

// on ne déclare ici que les deux fonctions appelables par le client
void client_max_sendData(int fifofd, int argc, char * argv[]);
void client_max_receiveResult(int fifofd, int argc, char * argv[]);

#endif
