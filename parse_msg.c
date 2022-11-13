#include <string.h>
#include "config.h"
#include "irc.h"
#include "parse_msg.h"
#include "bot.h"

void parse_msg( connexion_t* server, 
		char* prefix, char* channel, char* params ) {

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
		
		bot_command( server, prefix, channel, keyword, params ) ;

		return ;
	}
	
	// La chaine transmise contient le nom du bot
	
	if ( strstr( params, NICKNAME ) != 0 ) {
		if ( strstr( params, "ACTION" ) != 0 )
			bot_action( server, prefix, channel, params ) ;
		else
			bot_reply( server, prefix, channel, params ) ;
	}

	
}
