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
	FILE* handle;
	char buffer[255] ;
	char key[128];
	int count ;
	handle = fopen( filename, "r" ) ;
	if ( handle == NULL ) {
		print_error( "Could not load replies file\n" ) ;
		return ;
	}

	count = 0 ;
	int c ;
	while( (c=fgetc(handle)) != EOF ) {
		if ( c == '\n' )
			count++;
	}


	server->replies = malloc( count*sizeof(char*) ) ;
	server->nb_replies = count ;

	fseek(handle, 0, SEEK_SET) ;

	count = 0 ;
	while ( fgets( buffer, 255, handle) != NULL ) {
		size_t longueur = 0 ;
		longueur = strlen( buffer ) ;
		server->replies[count] = malloc( longueur+1 ) ;
		snprintf( server->replies[count], longueur+1, "%s", buffer ) ;
		count++ ;
	}
#ifdef DEBUG
	int i ;
	for (i=0; i<server->nb_replies; i++ )
		printf("- %s\n", server->replies[i] ) ;
#endif

	sprintf(key, "%d replies loaded \n", server->nb_replies ) ;
	print_notice( key ) ;

}


void bot_load_actions( connexion_t* server, char* filename ) {
	FILE* handle;
	char buffer[255] ;
	char key[128];
	int count ;
	handle = fopen( filename, "r" ) ;
	if ( handle == NULL ) {
		print_error( "Could not load actions file\n" ) ;
		return ;
	}

	count = 0 ;
	int c ;
	while( (c=fgetc(handle)) != EOF ) {
		if ( c == '\n' )
			count++;
	}


	server->actions = malloc( count*sizeof(char*) ) ;
	server->nb_actions = count ;

	fseek(handle, 0, SEEK_SET) ;

	count = 0 ;
	while ( fgets( buffer, 255, handle) != NULL ) {
		size_t longueur = 0 ;
		longueur = strlen( buffer ) ;
		server->actions[count] = malloc( longueur+1 ) ;
		snprintf( server->actions[count], longueur+1, "%s", buffer ) ;
		count++ ;
	}
#ifdef DEBUG
	int i ;
	for (i=0; i<server->nb_actions; i++ )
		printf("- %s\n", server->actions[i] ) ;
#endif

	sprintf(key, "%d actions loaded \n", server->nb_actions ) ;
	print_notice( key ) ;

}

void bot_action( connexion_t* server, 
        char* prefix, char* channel, char* params ){

	char reponse[255] ;
	int nb_reponse ;
	char* nickname ;
	nickname = strtok(prefix, "!");
	if (nickname == NULL ) {
		return ;
	}
	
	nb_reponse = rand() % ( server->nb_actions ) ;

	snprintf( reponse,255, server->actions[nb_reponse], nickname ) ; 
	irc_action( server, channel, reponse ) ;
}

void bot_reply( connexion_t* server, 
        char* prefix, char* channel, char* params ){

	char reponse[255] ;
	char* nickname ;
	int nb_reponse ;
	nickname = strtok(prefix, "!");
	if (nickname == NULL ) {
		return ;
	}

	nb_reponse = rand() % ( server->nb_replies ) ;

	snprintf( reponse,255, "%s: %s", nickname, server->replies[nb_reponse] ) ; 
	irc_say( server, channel, reponse ) ;
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
	printf("keyword: <%s> \n", keyword ) ;
	printf("channel: <%s> \n", channel ) ;
	printf("user level: <%d> \n", level ) ;
#endif

	// Commandes de niveau 5
	
	if ( level >=5 ) { 
		if ( strcmp( keyword, "die" ) == 0 )  {
			server->done = 1 ;
			irc_say( server, channel, "AAAAAaaaaaarg" ) ;
			irc_action( server, channel, "agonise" ) ;
			return ;
		}
	}

	// Commande de niveau 0
	
		if ( strcmp( keyword, "help" ) == 0 ) {
			char buffer[600] ;
			for ( i=0; i<server->nb_commands; i++ ) {
				sprintf( buffer, "[%s] %s \n", server->commands[i].keyword, server->commands[i].description ) ;
				irc_say( server, channel, buffer ) ;    // FIXME
			}
		}

		if ( strcmp( keyword, "friends" ) == 0 ) {
			irc_say( server, channel, "Tix : https://www.twitch.tv/tixlegeek" ) ;
			irc_say( server, channel, "Fixou : https://www.twitch.tv/fixoulab" ) ;
			irc_say( server, channel, "Imil : https://www.twitch.tv/imilnb" ) ;
			irc_say( server, channel, "Nils : https://www.twitch.tv/ahp_nils" ) ;
		}

		if ( strcmp( keyword, "tagada" ) == 0 ) {
			irc_say( server, channel, "tsoin tsoin !" ) ;
		}

		if ( strcmp( keyword, "ciseau" ) == 0 ) {
			irc_say( server, channel, "Pierre. Je gagne !" ) ;
		}

		if ( strcmp( keyword, "pierre" ) == 0 ) {
			irc_say( server, channel, "Enchanté Pierre, moi c'est Igor." ) ;
		}

		if ( strcmp( keyword, "spock" ) == 0 ) {
			irc_say( server, channel, "🖖" ) ;
		}

	// Commandes normales

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



