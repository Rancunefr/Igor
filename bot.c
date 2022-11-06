#include <iniparser.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "bot.h"
#include "config.h"
#include "irc.h"
#include "pretty_print.h"
#include "utils.h"

#define DEBUG

void bot_load_commands( connexion_t* server, char* filename ) {
	int i ;	
	dictionary* dico ;
	char key[128] ;
	dico = iniparser_load( filename ) ;
	if ( dico == NULL ) {
		print_error( "Could not load commands file\n" ) ;
		return ;
	}
	server->nb_commands = iniparser_getnsec( dico ) ;
	if ( server->nb_commands == 0 ){
		print_notice("No command was found\n") ;
		return ;
	}

	server->commands = malloc( server->nb_commands * sizeof(shell_command_t) ) ;	
	if ( server->commands == NULL ) {
			error("Malloc failed") ;
	}

	for ( i=0; i<server->nb_commands; i++ ) {
		const char* sec_name = iniparser_getsecname( dico, i ) ;
		memcpy( server->commands[i].keyword, sec_name, strlen(sec_name)+1 ) ;	

		sprintf( key, "%s:description", sec_name ) ;
		const char* desc = iniparser_getstring(	dico, key, "?" ) ;
		memcpy( server->commands[i].description, desc, strlen(desc)+1 ) ;

		sprintf( key, "%s:command", sec_name ) ;
		const char* cmd = iniparser_getstring( dico, key, "/usr/bin/true" ) ;
		memcpy( server->commands[i].command, cmd, strlen(cmd)+1 ) ;

		sprintf( key, "%s:required_level", sec_name ) ;
		server->commands[i].required_level = iniparser_getint( dico, key, 5 ) ;
		
	}
	sprintf( key, "%d commands loaded\n", server->nb_commands ) ;
	print_notice( key ) ;
	iniparser_freedict( dico) ;
}

void bot_load_people( connexion_t* server, char* filename ) {
	int i ;	
	dictionary* dico ;
	char key[128] ;
	dico = iniparser_load( filename ) ;
	if ( dico == NULL ) {
		print_error( "Could not load people file\n" ) ;
		return ;
	}
	server->nb_people = iniparser_getnsec( dico ) ;
	if ( server->nb_people == 0 ){
		print_notice("No people was found\n") ;
		return ;
	}

	server->people = malloc( server->nb_people * sizeof(people_t) ) ;	
	if ( server->people == NULL ) {
			error("Malloc failed") ;
	}

	for ( i=0; i<server->nb_people; i++ ) {
		const char* sec_name = iniparser_getsecname( dico, i ) ;
		memcpy( server->people[i].nickname, sec_name, strlen(sec_name)+1 ) ;	

		sprintf( key, "%s:level", sec_name ) ;
		server->people[i].level = iniparser_getint( dico, key, 0 ) ;
	}

	sprintf( key, "%d people loaded\n", server->nb_people ) ;
	print_notice( key ) ;
	iniparser_freedict( dico) ;
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

	int i ;
	int level ;
	char* nickname ;

	nickname = strtok(prefix, "!");
	if (nickname == NULL ) {
		return ;
	}
	
	level = 0 ;
	for ( i=0; i< server->nb_people; i++ )
		if ( strcmp( nickname, server->people[i].nickname ) == 0 )
			level = server->people[i].level ;


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

	for ( i=0; i< server->nb_commands; i++ ) {
		if ( strcmp( keyword, server->commands[i].keyword ) == 0 ) {
			if ( level >= server->commands[i].required_level ) {
				pid_t pid ;
				pid = fork() ;
				if ( pid == -1 ) {
					error( "Could not fork" ) ;
				}
				if ( pid == 0 ) {
					char* arg[255] ;
					char* chemin ;
					chemin = strtok( server->commands[i].command, " ") ;
					arg[0] = chemin ;
					int c = 1 ;
					do {
						arg[c] =strtok( NULL, " " ) ;
						c++ ;
					} while  ( arg[c-1] != NULL ) ;
					execvp( chemin, arg );
				} else {
					int stat_loc ;
					wait( &stat_loc) ;
				}
			}
		}
	}

}



