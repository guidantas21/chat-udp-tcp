#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <netinet/in.h>

#define BUFFER_SIZE 2024

typedef struct 
{
    int socket;
    struct sockaddr_in address;
    int port;
} Server;

int create_server_socket(Server *server, const int port);

void handle_client(Server *server);

void run_server(Server *server, const int num_threads);

#endif
