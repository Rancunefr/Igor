
#include <sys/socket.h>
#include <openssl/err.h>
#include <netdb.h>
#include <resolv.h>
#include <strings.h>
#include <unistd.h>

#include "connexion.h"
#include "pretty_print.h"
#include "utils.h"

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
    const SSL_METHOD *method;

	connexion = malloc(sizeof(connexion_t));
	if (connexion==NULL) 
		error( "malloc()" ); 

	connexion->nb_actions = 0 ;
	connexion->nb_replies = 0 ;
	connexion->nb_commands = 0 ;
	connexion->nb_people = 0 ;
	connexion->done = 0 ;

	host = gethostbyname(hostname) ;
    if ( host == NULL )
		error( "gethostbyname()");

    connexion->sd = socket(PF_INET, SOCK_STREAM, 0);
	if ( connexion->sd == -1 )
		error( "socket()") ;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(connexion->sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(connexion->sd);
		error( "connect()" );
    }
		
    method = TLS_client_method();  
    connexion->ssl_context = SSL_CTX_new(method);   
    if ( connexion->ssl_context == NULL )
    {
        ERR_print_errors_fp(stderr);
		error( "SSL_CTX_new()" );
    }	
    
	connexion->ssl = SSL_new(connexion->ssl_context);		
    SSL_set_fd(connexion->ssl, connexion->sd);			
    if (SSL_connect(connexion->ssl) == -1) {			
		ERR_print_errors_fp(stderr)	;
		error( "SSL_connect()" );
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
        print_notice("No client certificates configured.\n");
}

void connexion_close( connexion_t* server ) {
	SSL_free( server->ssl ) ;
	close ( server->sd ) ;
	SSL_CTX_free( server->ssl_context) ;
}
