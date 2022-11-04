#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define error( msg ) \
	{ fprintf( stderr, "Error :\n"); \
	fprintf( stderr, "%s : %d\n", __FILE__, __LINE__ ) ; \
	fprintf( stderr, "%s\n", msg ); \
	fprintf( stderr, "errno : %d (%s)\n", errno, strerror(errno) ) ; \
	exit( EXIT_FAILURE ); }	

#endif

