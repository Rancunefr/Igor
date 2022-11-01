#ifndef IRC_h
#define IRC_h

#include "connexion.h"

int irc_send_buffer( connexion_t* server, const char* buffer, size_t buffer_len ) ;
int irc_recv_buffer( connexion_t* server, const char* buffer, size_t buffer_len ) ;

int irc_auth( connexion_t* server, const char* login, const char* password ) ;
int irc_disconnect( connexion_t* server ) ;
int irc_join( connexion_t* server, const char* channel ) ;
int irc_say( connexion_t* server, const char* channel, const char* msg) ;
int irc_action( connexion_t* server, const char* channel, const char* msg) ;

#endif
