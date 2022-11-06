#ifndef BOT_H
#define BOT_H

#include "connexion.h"

void bot_load_commands( connexion_t* server, char* filename ) ;
void bot_load_people( connexion_t* server, char* filename ) ;

void bot_load_replies( connexion_t* server, char* filename ) ;
void bot_load_actions( connexion_t* server, char* filename ) ;

void bot_action( connexion_t* server, 
        char* prefix, char* channel, char* params ); 
void bot_reply( connexion_t* server, 
        char* prefix, char* channel, char* params ); 
void bot_command( connexion_t* server, 
        char* prefix, char* channel, char* keyword, char* params ) ;

#endif

