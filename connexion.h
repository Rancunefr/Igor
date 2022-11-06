#ifndef CONNEXION_H
#define CONNEXION_H

#include <openssl/ssl.h>

#define STRING_SIZE 500
#define KEYWORD_SIZE 24

typedef struct {
	char keyword[KEYWORD_SIZE] ;		// mot clé
	char description[STRING_SIZE] ;		// description de la commande
	char command[STRING_SIZE] ;			// commande shell executée
	int  required_level ;				// niveau de privilege nécessaire
} shell_command_t ;


typedef struct {
	char nickname[STRING_SIZE] ;
	int level ;
} people_t ;


typedef struct {
	int			sd ;
	SSL_CTX*	ssl_context ;
	SSL*		ssl ;

	int done ;

	char actions[100][STRING_SIZE] ;	
	int nb_actions ;

	char replies[100][STRING_SIZE] ;
	int nb_replies ;

	shell_command_t commands[100] ;
	int nb_commands ;

	people_t people[100] ;
	int nb_people ;

} connexion_t ;

void ssl_init();

connexion_t* connexion_open(const char *hostname, int port) ;
void connexion_close( connexion_t * ) ;

void connexion_show_cert(connexion_t* server);

#endif
