#ifndef CONNEXION_H
#define CONNEXION_H

#include <openssl/ssl.h>

int OpenConnection(const char *hostname, int port) ;
SSL_CTX* InitCTX(void) ;
void ShowCerts(SSL* ssl) ;  

#endif
