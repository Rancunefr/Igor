#ifndef OLLAMA_H
#define OLLAMA_H

struct ollama_cx_s ;
typedef struct ollama_cx_s ollama_cx_t ;

struct ollama_cx_s* ollama_new() ;
void ollama_free(struct ollama_cx_s* tmp) ;
int ollama_send( struct ollama_cx_s *llama, char* str_prompt ) ;
char* ollama_get_reply( struct ollama_cx_s* tmp ) ;
#endif
