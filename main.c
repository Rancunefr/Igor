#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <openssl/err.h>

#include "config.h"
#include "connexion.h"
#include "irc.h"
#include "pretty_print.h"

int main(int count, char *strings[])
{
	connexion_t* server ;
    char read_buf[1024];
    char write_buf[1024];
    int bytes;
	int done = 0 ;

	ssl_init() ;

    server = connexion_open(SERVER, PORT);
	connexion_show_cert(server) ;

	irc_auth( server, NICKNAME , ID_TOKEN ) ;

	irc_join( server, CHANNEL ) ;

	irc_say( server, CHANNEL, "Hello !" ) ;

	while (!done) {
		char* prefix = NULL ;
		char* command = NULL ;
		char* argument_1 ;
		char* argument_2 ;
		bytes = irc_recv_buffer(1, server, read_buf, sizeof(read_buf) ) ;
		if ( bytes > 0 ) {
			if ( read_buf[0] == ':' ) {
				// il y a un prefix, on le recupere
				prefix = strtok( read_buf+1, " ") ;
				command = strtok( NULL, " ") ;
			} else {	
				command = strtok( read_buf, " ") ;
			}
		}

		if ( command )
			printf("Commande : %s\n", command ) ;
		if ( prefix )
			printf("Prefix : %s \n", prefix ) ;

		if ( strcmp( command, "PING") == 0 ) {
			argument_1 = strtok( NULL, " " ) ;
			sprintf(write_buf, "PONG %s", argument_1 ) ;
			bytes = irc_send_buffer(1, server, write_buf ) ;
		}
 
		if ( strcmp( command, "PRIVMSG") == 0 ) {
			argument_1 = strtok( NULL, " ") ; // channel
			argument_2 = argument_1 + strlen(argument_1) + 2 ; // message
			printf("Canal : %s\n", argument_1) ;
			printf("Message: %s\n", argument_2) ;
		}
	}

	connexion_close( server ) ;

    return 0;
}
