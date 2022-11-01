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
    char write_buf[1024] = { 0 };
    int bytes;

	ssl_init() ;
    server = connexion_open(SERVER, PORT);
	connexion_show_cert(server) ;

	irc_send_buffer(server, "CAP REQ :twitch.tv/commands\n");
	irc_recv_buffer(server, read_buf, sizeof(read_buf) ) ;
	sprintf(write_buf, "PASS %s\n", ID_TOKEN);
	irc_send_buffer(server, write_buf );
	sprintf(write_buf, "NICK %s\n", NICKNAME);
	irc_send_buffer(server, write_buf );
	irc_recv_buffer(server, read_buf, sizeof(read_buf) ) ;
	sprintf(write_buf, "JOIN %s\n", CHANNEL);
	irc_send_buffer(server, write_buf );
	irc_recv_buffer(server, read_buf, sizeof(read_buf) ) ;
	sprintf(write_buf, "PRIVMSG  %s :Hello, I'm the bot !\n",
		CHANNEL);
	irc_send_buffer(server, write_buf );

	while (1) {
		bytes = irc_recv_buffer(server, read_buf, sizeof(read_buf) ) ;
	}

	connexion_close( server ) ;

    return 0;
}
