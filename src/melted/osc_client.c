#include "osc_client.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include "melted_log.h"
#include "tinyosc.h"

static osc_client _client;
osc_client osc_client_init()
{
    if  (_client != NULL)
    {
        return _client;
    }

    _client = malloc( sizeof( osc_client_t ) );
    if ( _client != NULL )
    {
        memset( _client, 0, sizeof( osc_client_t ) );
        
        struct sockaddr_in addr;
        _client->socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (_client->socket < 0) {
            melted_log(LOG_ERR, "Failed to create OSC socket");
            free(_client);
            _client = NULL;
            return NULL;
        }

        // Setup address structure
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
        addr.sin_port = htons(DEFAULT_UDP_PORT);

        // Connect socket (makes send() easier to use)
        if (connect(_client->socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            melted_log(LOG_ERR, "Failed to connect OSC socket");
            close(_client->socket);
            free(_client);
            _client = NULL;
            return NULL;
        }

    }
    return _client;
}

void osc_client_send( osc_client client, char *address, char *format, ... )
{
    // declare a buffer for writing the OSC packet into
    int len = 1024;
    char buffer[len];
    
    va_list ap;
    va_start(ap, format);
    const uint32_t i = tosc_vwrite(buffer, len, address, format, ap);
    va_end(ap);

    send(client->socket, buffer, len, 0);

    return i; // return the total number of bytes written
    // send the data out of the socket
}

void osc_client_close(osc_client client)
{
    if (client && client == _client) {
        if (_client->socket > 0) {
            close(_client->socket);
        }
        free(_client);
        _client = NULL;
    }
}