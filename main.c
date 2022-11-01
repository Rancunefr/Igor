#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "config.h"
#include "connexion.h"
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

	sprintf(write_buf, "CAP REQ :twitch.tv/commands\n");
	print_out(write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	
	read_buf[bytes] = 0;
	print_in(read_buf);

	sprintf(write_buf, "PASS %s\n", ID_TOKEN);
	print_out(write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	sprintf(write_buf, "NICK %s\n", NICKNAME);
	print_out(write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	
	read_buf[bytes] = 0;
	print_in(read_buf);

	sprintf(write_buf, "JOIN %s\n", CHANNEL);
	print_out(write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	
	read_buf[bytes] = 0;
	print_in(read_buf);

	sprintf(write_buf, "PRIVMSG  %s :Hello, I'm the bot !\n",
		CHANNEL);
	print_out(write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));

	while (1) {
	    bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	
	    if (bytes != 0) {
		read_buf[bytes] = 0;
		print_in(read_buf);
	    }
	}

	connexion_close( server ) ;

    return 0;
}
