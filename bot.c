#include <string.h>
#include "bot.h"
#include "irc.h"
#include "config.h"

#define DEBUG

void bot_load_commands( connexion_t* server, char* filename ) {
	
}


void bot_load_replies( connexion_t* server, char* filename ) {

}


void bot_load_actions( connexion_t* server, char* filename ) {

}


void bot_action( connexion_t* server, 
        char* prefix, char* channel, char* params ){

}

void bot_reply( connexion_t* server, 
        char* prefix, char* channel, char* params ){

}


void bot_command( connexion_t* server, 
        char* prefix, char* channel, char* keyword, char* params ) {

	char* nickname ;
	nickname = strtok(prefix, "!");
	if (nickname == NULL ) {
		return ;
	}

#ifdef DEBUG
	printf("nickname : <%s> \n", nickname ) ;
	printf("channel: <%s> \n", channel ) ;
	printf("keyword: <%s> \n", keyword ) ;
#endif
	
	if ( strcmp( nickname, OWNER) == 0 ) { // Ici les commandes reservees au proprio du bot
		if ( strcmp( keyword, "die" ) == 0 )  {
			server->done = 1 ;
			irc_say( server, channel, "AAAAAaaaaaarg" ) ;
			return ;
		}
	}

	if ( strmcp( keyword, "help" ) == 0 ) {
		irc_say( server, channel, "Voici l'aide \n" ) ;
		return ;
	}

}



