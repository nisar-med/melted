#ifndef _OSC_CLIENT_H_
#define _OSC_CLIENT_H_

#define DEFAULT_UDP_PORT 9000
#define DEFAULT_IP "127.0.0.1"

typedef struct
{
    int socket;
}
*osc_client, osc_client_t;

extern osc_client osc_client_init();
extern void osc_client_send_progress( osc_client client, int position );
#endif