#include <string.h>
#include <openssl/ssl.h>

#include "irc.h"
#include "pretty_print.h"
#include "utils.h"

int irc_send_buffer( int display, connexion_t* server, const char* buffer ) {
	int ret ;
	if (display)
		print_out(buffer);
	ret = SSL_write(server->ssl, buffer, strlen(buffer));
	if ( ret <=0 ) {
		fprintf(stderr, "SSL Error %d\n", SSL_get_error(server->ssl, ret ));
		error( "SSL_write()" ) ;
	}
	return 0 ;
}

int irc_recv_buffer( int display, connexion_t* server, char* buff, size_t buff_len ) {
	int bytes ;
	bytes = SSL_read( server->ssl, buff, buff_len ) ;
	if ( bytes <=0 ) {
		fprintf(stderr, "SSL Error %d\n", SSL_get_error(server->ssl, bytes ));
		error( "SSL_write()" ) ;
	}
	buff[bytes] = '\0' ;
	if (display)
		print_in(buff) ;
	return bytes ;
}

int irc_auth( connexion_t* server, const char* login, const char* password ) {
    char read_buf[1024];
    char write_buf[1024];
	irc_send_buffer(1, server, "CAP REQ :twitch.tv/commands\n");
	irc_recv_buffer(1, server, read_buf, sizeof(read_buf) ) ;
	sprintf(write_buf, "PASS %s\n", password);
#ifdef DISPLAY_OAUTH
	irc_send_buffer(1, server, write_buf );
#else
	irc_send_buffer(0, server, write_buf );
#endif
	sprintf(write_buf, "NICK %s\n", login);
	irc_send_buffer(1, server, write_buf );
	irc_recv_buffer(1, server, read_buf, sizeof(read_buf) ) ;
	// FIXME verifier ici la reponse a l'authentification
	return 0; 
}

int irc_join( connexion_t* server, const char* channel ) {
    char read_buf[1024];
    char write_buf[1024] = { 0 };
	sprintf(write_buf, "JOIN %s\n", channel);
	irc_send_buffer(1, server, write_buf );
	irc_recv_buffer(1, server, read_buf, sizeof(read_buf) ) ;
	return 0 ;
}

void irc_say( connexion_t* server, const char* channel, const char* msg) {
    char read_buf[1024];
    char write_buf[1024] = { 0 };
	sprintf(write_buf, "PRIVMSG  %s :%s\n", channel, msg);
	irc_send_buffer(1, server, write_buf );
	irc_recv_buffer(1, server, read_buf, sizeof(read_buf) ) ;
}

void irc_action( connexion_t* server, const char* channel, const char* msg) {
    char write_buf[1024] = { 0 };
	sprintf(write_buf, "PRIVMSG %s :/me %s\n", channel, msg);
	irc_send_buffer(1, server, write_buf );
}

void irc_pong( connexion_t* server, char* msg ) {
    char write_buf[1024] = { 0 };
	sprintf(write_buf, "PONG %s", msg);
	irc_send_buffer(1, server, write_buf);
}
