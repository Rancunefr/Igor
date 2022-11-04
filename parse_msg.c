#include <string.h>
#include "config.h"
#include "irc.h"
#include "parse_msg.h"

void parse_msg( connexion_t* server, 
		char* prefix, char* channel, char* params ) {


printf("DEBUG: <%s> \n", params ) ;

	if ( strlen(params) == 0 )
		return ;

	// La chaine commence par !  => C'est une commande
	
	if ( params[0] == '!' ) {
		char *p ;
		char *keyword ;
		keyword = params+1 ;
		p = keyword ;
		while (*p && *p !=' '&& *p!='\n' && *p!= '\r' )
			p++ ;
		*p ='\0' ;
		
		if (strcmp(keyword, "help")==0) {
			irc_say( server, channel, "Bonjour !" ) ;
		}	
	
		// TODO Parsing de fichier ini avec les commandes et leur mot clé

		return ;
	}

	// La chaine transmise contient le nom du bot
	
	if ( strstr( params, NICKNAME ) != 0 ) {
			irc_say( server, channel, "Oui maître, bien maître ..." ) ;
			return ;
	}
	

}
