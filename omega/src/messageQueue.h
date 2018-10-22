#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_


int     initMessageQueue        (char* host, int port);
void    destroyMessageQueue     ();

void    waitMessageQueueConnected ();

int     sendMessage             (char* topic, char* payload);
// int     messageQueueSubscribe   (char* topic);


#endif  // _MESSAGE_QUEUE_H_