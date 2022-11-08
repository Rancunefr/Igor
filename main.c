#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <openssl/err.h>

#include "bot.h"
#include "config.h"
#include "connexion.h"
#include "utils.h"
#include "irc.h"
#include "pretty_print.h"
#include "parse_msg.h"

int main(int count, char *strings[])
{
    connexion_t *server;
    char read_buf[1024];
    int bytes;

    ssl_init();

    server = connexion_open(SERVER, PORT);
    connexion_show_cert(server);

	bot_load_commands( server, "./commands.ini" ) ;
	bot_load_people( server, "./people.ini" ) ;

    irc_auth(server, NICKNAME, ID_TOKEN);
    irc_join(server, CHANNEL);
    irc_say(server, CHANNEL, "Hello !");
    irc_action(server, CHANNEL, "salue la noble assemblée");

    while (!server->done) {
		char *prefix = NULL;
		char *command = NULL;
		char *channel;
		char *params;
		bytes = irc_recv_buffer(1, server, read_buf, sizeof(read_buf));
		if (bytes > 0) {
			char * msg = read_buf ;
			if (read_buf[0] == ':') {
				// il y a un prefix, on le recupere
				prefix = strtok(read_buf + 1, " ");
				msg = NULL ;
			}

			command = strtok(msg, " ");

			if (strcmp(command, "PING") == 0) {
				params = strtok(NULL, " ");
				if ( params == NULL ) 
					error( "Error while parsing PING : no string found" ) ;
				irc_pong( server, params ) ;
			} 

			if (strcmp(command, "PRIVMSG") == 0) {
				channel = strtok(NULL, " ");
				if ( channel == NULL ) {
					error( "Error while parsing PRIVMSG : no channel found" );
				}
				params = channel + strlen(channel) + 2;
				if ( params == NULL ) {
					error( "Error while parsing PRIVMSG : no string found" );
				}
				parse_msg( server, prefix, channel, params ) ;
			}	
		}
    }  

    connexion_close(server);
    return 0;
}
