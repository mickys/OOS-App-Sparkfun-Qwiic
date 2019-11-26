#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#define MQ_DEFAULT_HOST 	"localhost"
#define MQ_DEFAULT_PORT 	1883

int     initMessageQueue        (const char* host, int port, const char* certificate);
void    destroyMessageQueue     ();

void    waitMessageQueueConnected ();

int     sendMessage             (char* topic, char* payload);
// int     messageQueueSubscribe   (char* topic);


#endif  // _MESSAGE_QUEUE_H_