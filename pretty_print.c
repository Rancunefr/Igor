#include <stdio.h>
#include <string.h>
#include "pretty_print.h"

const char str_out[] = "\x1b[1;34m>>>\x1b[0m ";
const char str_in[] = "\x1b[1;33m<<<\x1b[0m ";
const char str_notice[] = "\x1b[1;32m***\x1b[0m ";
const char str_error[] = "\x1b[1;31m!!!\x1b[0m ";


void print_in( const char* msg ) {
	const char* line ;
	unsigned int line_end ;
	line = msg ;
	do {
			line_end = 0 ;
			while ( *(line+line_end) != '\n' )
				line_end++ ;
			fwrite( str_in, 1, strlen(str_out), stdout) ;
			fwrite( line, 1, line_end+1, stdout) ;
			line = line + line_end+1 ;	
	} while ( *line != '\0' ) ;
}

void print_out( const char* msg ) {
	const char* line ;
	unsigned int line_end ;
	line = msg ;
	do {
			line_end = 0 ;
			while ( *(line+line_end) != '\n' )
				line_end++ ;
			fwrite( str_out, 1, strlen(str_out), stdout) ;
			fwrite( line, 1, line_end+1, stdout) ;
			line = line + line_end+1 ;	
	} while ( *line != '\0' ) ;
}

void print_error( const char* msg ) {
	const char* line ;
	unsigned int line_end ;
	line = msg ;
	do {
			line_end = 0 ;
			while ( *(line+line_end) != '\n' )
				line_end++ ;
			fwrite( str_error, 1, strlen(str_out), stdout) ;
			fwrite( line, 1, line_end+1, stdout) ;
			line = line + line_end+1 ;	
	} while ( *line != '\0' ) ;
}

void print_notice( const char* msg ) {
	const char* line ;
	unsigned int line_end ;
	line = msg ;
	do {
			line_end = 0 ;
			while ( *(line+line_end) != '\n' )
				line_end++ ;
			fwrite( str_notice, 1, strlen(str_out), stdout) ;
			fwrite( line, 1, line_end+1, stdout) ;
			line = line + line_end+1 ;	
	} while ( *line != '\0' ) ;
}

