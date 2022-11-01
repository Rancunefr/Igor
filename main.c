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
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    char acClientRequest[1024] = { 0 };
    int bytes;
    SSL_library_init();
    ctx = InitCTX();
    server = OpenConnection(SERVER, PORT);
    ssl = SSL_new(ctx);		/* create new SSL connection state */
    SSL_set_fd(ssl, server);	/* attach the socket descriptor */
    if (SSL_connect(ssl) == FAIL)	/* perform the connection */
	ERR_print_errors_fp(stderr);
    else {
	ShowCerts(ssl);		/* get any certs */

	sprintf(acClientRequest, "CAP REQ :twitch.tv/commands\n");
	printf(">>> %s", acClientRequest);
	SSL_write(ssl, acClientRequest, strlen(acClientRequest));
	bytes = SSL_read(ssl, buf, sizeof(buf));	/* get reply & decrypt */
	buf[bytes] = 0;
	printf("<<< %s", buf);

	sprintf(acClientRequest, "PASS %s\n", ID_TOKEN);
	printf(">>> %s", acClientRequest);
	SSL_write(ssl, acClientRequest, strlen(acClientRequest));
	sprintf(acClientRequest, "NICK %s\n", NICKNAME);
	printf(">>> %s", acClientRequest);
	SSL_write(ssl, acClientRequest, strlen(acClientRequest));
	bytes = SSL_read(ssl, buf, sizeof(buf));	/* get reply & decrypt */
	buf[bytes] = 0;
	printf("<<< %s", buf);

	sprintf(acClientRequest, "JOIN %s\n", CHANNEL);
	printf(">>> %s", acClientRequest);
	SSL_write(ssl, acClientRequest, strlen(acClientRequest));
	bytes = SSL_read(ssl, buf, sizeof(buf));	/* get reply & decrypt */
	buf[bytes] = 0;
	printf("<<< %s", buf);

	sprintf(acClientRequest, "PRIVMSG  %s :Hello, I'm the bot !\n",
		CHANNEL);
	printf(">>> %s", acClientRequest);
	SSL_write(ssl, acClientRequest, strlen(acClientRequest));

	while (1) {
	    bytes = SSL_read(ssl, buf, sizeof(buf));	/* get reply & decrypt */
	    if (bytes != 0) {
		buf[bytes] = 0;
		printf("<<< %s", buf);
	    }
	}
    }
    SSL_free(ssl);
    close(server);		/* close socket */
    SSL_CTX_free(ctx);		/* release context */
    return 0;
}
