#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL    -1

#include "config.h"
#include "connexion.h"

int main(int count, char *strings[])
{
	connexion_t* server ;
    char read_buf[1024];
    char write_buf[1024] = { 0 };
    int bytes;
    SSL_library_init();

    server = connexion_open(SERVER, PORT);

	sprintf(write_buf, "CAP REQ :twitch.tv/commands\n");
	printf(">>> %s", write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	/* get reply & decrypt */
	read_buf[bytes] = 0;
	printf("<<< %s", read_buf);

	sprintf(write_buf, "PASS %s\n", ID_TOKEN);
	printf(">>> %s", write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	sprintf(write_buf, "NICK %s\n", NICKNAME);
	printf(">>> %s", write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	/* get reply & decrypt */
	read_buf[bytes] = 0;
	printf("<<< %s", read_buf);

	sprintf(write_buf, "JOIN %s\n", CHANNEL);
	printf(">>> %s", write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));
	bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	/* get reply & decrypt */
	read_buf[bytes] = 0;
	printf("<<< %s", read_buf);

	sprintf(write_buf, "PRIVMSG  %s :Hello, I'm the bot !\n",
		CHANNEL);
	printf(">>> %s", write_buf);
	SSL_write(server->ssl, write_buf, strlen(write_buf));

	while (1) {
	    bytes = SSL_read(server->ssl, read_buf, sizeof(read_buf));	/* get reply & decrypt */
	    if (bytes != 0) {
		read_buf[bytes] = 0;
		printf("<<< %s", read_buf);
	    }
	}

	connexion_close( server ) ;

    return 0;
}
