#ifndef CONNEXION_H
#define CONNEXION_H

#include <openssl/ssl.h>

typedef struct {
	int			sd ;
	SSL_CTX*	ssl_context ;
	SSL*		ssl ;
} connexion_t ;

void ssl_init();

connexion_t* connexion_open(const char *hostname, int port) ;
void connexion_close( connexion_t * ) ;

void connexion_show_cert(connexion_t* server);

#endif
