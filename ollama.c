#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#include "ollama.h"

#define MIN(X,Y) (((X)<(Y))?(X):(Y))
#define BUFFER_SIZE (4000000)

struct string_s {
	size_t size ;
	char*  array ;
} ;

struct buffer_s {
	size_t size ;
	size_t position ;
	char* array ;
} ;

struct ollama_cx_s {
	CURL *curl;
	struct buffer_s buffer ;
	struct string_s reply ;
	struct json_object *context ;
} ;

// struct string_s context = { .size = 0 , .array = NULL };


int ollama_parse_msg( struct ollama_cx_s* llama) {
	json_object* root ;
	json_object* json_response ;
	json_object* json_context ;

	root = json_tokener_parse(llama->buffer.array) ;
	json_response = json_object_object_get(root, "response");
	json_context = json_object_object_get(root, "context");

	int cpy_len = MIN(llama->reply.size-1, json_object_get_string_len(json_response)) ;
	memcpy(llama->reply.array, \
			json_object_get_string(json_response), \
			cpy_len ) ;
	llama->reply.array[cpy_len]='\0' ;

	json_object_put( llama->context ) ;
	llama->context = json_object_new_object() ;
	json_context = json_object_get(json_context) ;
	json_object_object_add( llama->context, "context", json_context)  ;
	json_object_put(root) ;

	return 0 ;

}

size_t cb_receive(void *ptr, size_t size, size_t nmemb, void *user_ptr){
	struct buffer_s* buffer = (struct buffer_s*) user_ptr;
	if ( (buffer->position + size*nmemb) > buffer->size ) {
		buffer->array = realloc( buffer->array, buffer->size+(size*nmemb) ) ;
		buffer->size += size*nmemb ;
	}
	memcpy(buffer->array+buffer->position,ptr,size*nmemb);
	buffer->position += size*nmemb ;
	return size*nmemb ;
}

int ollama_send( ollama_cx_t *llama, char* str_prompt ) {

	CURLcode res;
	json_object* ctx = NULL ;

	if (str_prompt == NULL )
		return 1 ;

	json_object* ollama_request = NULL ;
	ollama_request = json_object_new_object() ;

	json_object_object_add( ollama_request, "model", json_object_new_string("igor"));
	json_object_object_add( ollama_request, "prompt", json_object_new_string(str_prompt));
	json_object_object_add( ollama_request, "stream", json_object_new_boolean(0));

	if ( json_object_array_length( json_object_object_get(llama->context,"context") ) != 0 ) {
		ctx = json_object_object_get(llama->context,"context");
		ctx = json_object_get(ctx) ;
		json_object_object_add( ollama_request, "context", ctx);
	}

	curl_easy_setopt(llama->curl, CURLOPT_URL, "http://localhost:11434/api/generate");
	const char* tmp = json_object_to_json_string(ollama_request) ;
	curl_easy_setopt(llama->curl, CURLOPT_POSTFIELDS, tmp) ;
	curl_easy_setopt(llama->curl, CURLOPT_WRITEFUNCTION, cb_receive);
	curl_easy_setopt(llama->curl, CURLOPT_WRITEDATA, (void*) &(llama->buffer) );

	llama->buffer.position = 0 ;
	res = curl_easy_perform(llama->curl);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return 1 ;
	}

	ollama_parse_msg(llama) ;

	json_object_put(ollama_request) ;
	return 0 ;
}


struct ollama_cx_s* ollama_new() {
	struct ollama_cx_s* tmp ;
	tmp = malloc ( sizeof(struct ollama_cx_s) ) ;
	if ( tmp == NULL ) {
		return NULL ;
	}
	tmp->buffer.size = 0 ;
	tmp->buffer.position = 0 ;
	tmp->buffer.array = NULL ;
	tmp->reply.size = 1000 ;
	tmp->reply.array = calloc(1000,1) ;
	tmp->context = json_object_new_object() ;
	json_object_object_add(tmp->context,"context", json_object_new_array()) ;
	curl_global_init(CURL_GLOBAL_ALL);
	tmp->curl = curl_easy_init();
	if (!tmp->curl) {
		return NULL ;
	}
	return tmp ;
}

void ollama_free(struct ollama_cx_s* tmp) {
	curl_easy_cleanup(tmp->curl);
	curl_global_cleanup();
	free(tmp->reply.array) ;
	json_object_put(tmp->context) ; 	// FIXME a verifier
}

char* ollama_get_reply( struct ollama_cx_s* tmp ) {
	return tmp->reply.array ;
}


void ollama_reset_context( struct ollama_cx_s* tmp ) {
	json_object_put( tmp->context ) ;
	tmp->context = json_object_new_object() ;
	json_object_object_add(tmp->context,"context", json_object_new_array()) ;
}

int ollama_get_context_size( struct ollama_cx_s* tmp ) {
	return (int) json_object_array_length(json_object_object_get(tmp->context,"context")) ;
}
