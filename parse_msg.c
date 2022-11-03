#include "irc.h"
#include "parse_msg.h"

void parse_msg( connexion_t* server, 
		char* prefix, char* channel, char* params ) {

	if ( strlen(params) == 0 )
		return ;

	// La chaine commence par !  => C'est une commande
	if ( params[0]='!' ) {
		char *p ;
		char *keyword ;
		keyword = params+1 ;
		p = keyword ;
		while (*p && *p != ' ' )
			p++ ;
		*p ='\0' ;
		
		if (strcmp(keyword, "help")==0) {
			irc_say( server, channel, "Bonjour !" ) ;
		}	


	}

	// La chaine contient le mot Igor ( attention CaseSen. )
	

}
