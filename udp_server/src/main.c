#include <stdio.h>
#include <stdlib.h>

#include "../include/server.h"

#define PORT 5000
#define NUM_THREADS 4

int main(void)
{
    Server server;

    if (create_server_socket(&server, PORT) != 0)
    {
        exit(EXIT_FAILURE);
    }

    run_server(&server, NUM_THREADS);

    return 0;
}
