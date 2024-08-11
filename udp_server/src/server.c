#include "server.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "pool.h"

int create_server_socket(Server *server, const int port)
{
    server->port = port;

    if ((server->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) 
    {
        perror("Error creating socket");
        return -1;
    }
   	memset(&server->address, 0, sizeof(server->address)); 
    
	server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = htonl(INADDR_ANY);
    server->address.sin_port = htons(server->port);
    
    if (bind(server->socket, (struct sockaddr *) &server->address, sizeof(server->address)) < 0)
    {
        perror("Error binding socket");
        close(server->socket);
        return -1;
    }
    return 0;
}

void handle_client(Server *server)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    
    char buffer[BUFFER_SIZE];
	char hello_message[] = "Hello there!\n";
    
    while (1)
    {
        int datagram_length = recvfrom(
            server->socket, 
            buffer, 
            sizeof(buffer) - 1, 
            0,
            (struct sockaddr *) &client_addr, 
            &client_addr_size
        );
        
        if (datagram_length < 0)
        {
            perror("Error receiving data");
            continue;
        } 
        buffer[datagram_length] = '\0';
        
        printf("message received: %s", buffer);     
    
        sendto(
            server->socket, 
            hello_message, 
            strlen(hello_message), 
            0,  
            (struct sockaddr *) &client_addr, 
            client_addr_size
        );
    }
}

void run_server(Server *server, const int num_threads)
{
    ThreadPool pool;

    if (thread_pool_init(&pool, num_threads, server->socket) != 0)
    {
        exit(EXIT_FAILURE);
    }

    if (thread_pool_destroy(&pool) != 0)
    {
        exit(EXIT_FAILURE);
    }
    close(server->socket);
}
