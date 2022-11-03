#ifndef IRC_h
#define IRC_h

#include "connexion.h"

int irc_send_buffer( int display, connexion_t* server, const char* buffer ) ;
int irc_recv_buffer( int display, connexion_t* server, char* buffer, size_t buffer_len ) ;

int irc_auth( connexion_t* server, const char* login, const char* password ) ;
int irc_disconnect( connexion_t* server ) ;  // TODO
int irc_join( connexion_t* server, const char* channel ) ;
int irc_say( connexion_t* server, const char* channel, const char* msg) ;
int irc_action( connexion_t* server, const char* channel, const char* msg) ; // TOD
int irc_pong( connexion_t* server, char* msg ) ;
#endif
