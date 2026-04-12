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
            return NULL;
        }

        int broadcast = 1;
        setsockopt(client->socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    }
    return client;
}

void osc_client_send_progress( osc_client client, int unit_id, int position, const char *ip, int port )
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    // declare a buffer for writing the OSC packet into
    int len = 1024;
    char buffer[len];
    len = tosc_writeMessage(buffer, len, "/melted/progress", "ii" , unit_id, position);
    if (len <= 0) return;
    sendto(client->socket, buffer, len, 0, 
        (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
}

void osc_client_close(osc_client client)
{
    close(client->socket);
    free(client);
}