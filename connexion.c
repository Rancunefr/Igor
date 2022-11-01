
#include <sys/socket.h>
#include <openssl/err.h>
#include <netdb.h>
#include <resolv.h>
#include <strings.h>
#include <unistd.h>

#include "connexion.h"
#include "pretty_print.h"

void ssl_init() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();  
    SSL_load_error_strings();   
}

connexion_t*connexion_open(const char *hostname, int port)
{
	connexion_t* connexion ;
    struct hostent *host;
    struct sockaddr_in addr;
	connexion = malloc(sizeof(connexion_t));
	if (connexion==NULL) {
		fprintf(stderr,"Malloc error\n");
		return NULL ;
	}
    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
     	free(connexion);
		return NULL ;
    }
    connexion->sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(connexion->sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(connexion->sd);
        perror(hostname);
		free(connexion);
		return NULL ;
    }
		
    const SSL_METHOD *method;
		
    method = TLS_client_method();  
    connexion->ssl_context = SSL_CTX_new(method);   
    if ( connexion->ssl_context == NULL )
    {
        ERR_print_errors_fp(stderr);
		free(connexion) ;
		return NULL ;
        abort();
    }
    
	connexion->ssl = SSL_new(connexion->ssl_context);		
    SSL_set_fd(connexion->ssl, connexion->sd);			
    if (SSL_connect(connexion->ssl) == -1) {			
		ERR_print_errors_fp(stderr)	;
		free(connexion) ;
		return NULL ;
	}

    return connexion ;
}

void connexion_show_cert(connexion_t* server)
{
	char buffer[1024] ;
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(server->ssl); 
    if ( cert != NULL )
    {
        print_notice("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        sprintf(buffer,"Subject: %s\n", line);
		print_notice(buffer) ;
        free(line);       
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        sprintf(buffer, "Issuer: %s\n", line);
		print_notice(buffer) ;
        free(line);       
        X509_free(cert);     
    }
    else
        printf("Info: No client certificates configured.\n");
}

void connexion_close( connexion_t* server ) {
	SSL_free( server->ssl ) ;
	close ( server->sd ) ;
	SSL_CTX_free( server->ssl_context) ;
}
