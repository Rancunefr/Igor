#include <string.h>
#include <openssl/ssl.h>

#include "irc.h"
#include "pretty_print.h"

int irc_send_buffer( connexion_t* server, const char* buffer ) {
	print_out(buffer);
	SSL_write(server->ssl, buffer, strlen(buffer));
	return 0 ;
}

int irc_recv_buffer( connexion_t* server, char* buff, size_t buff_len ) {
	int bytes ;
	bytes = SSL_read( server->ssl, buff, buff_len ) ;
	buff[bytes] = '\0' ;
	print_in(buff) ;
	return bytes ;
}

