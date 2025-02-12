#include "osc_client.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <tinyosc/tinyosc.h>
#include "melted_log.h"

osc_client osc_client_init()
{
    osc_client client;

    client = malloc( sizeof( osc_client_t ) );
    if ( client != NULL )
    {
        memset( client, 0, sizeof( osc_client_t ) );
        
        client->socket = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
        if (client->socket < 0) {
            melted_log(LOG_ERR, "Failed to create OSC socket");
            free(client);
            client = NULL;
            return NULL;
        }

        // Setup address structure
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
        addr.sin_port = htons(DEFAULT_UDP_PORT);

        // Set socket options to non-blocking and ignore ICMP errors
        int broadcast = 1;
        setsockopt(client->socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
        
        // Store address for later use
        memcpy(&client->addr, &addr, sizeof(addr));
    }
    return client;
}

void osc_client_send_progress( osc_client client, int position )
{
    // declare a buffer for writing the OSC packet into
    int len = 1024;
    char buffer[len];
    len = tosc_writeMessage(buffer, len, "/ping", "fsi" , 1.0f, "position", position);
    if (len <= 0) return;
    sendto(client->socket, buffer, len, 0, 
        (struct sockaddr*)&client->addr, sizeof(struct sockaddr_in));
}

void osc_client_close(osc_client client)
{
    close(client->socket);
    free(client);
}