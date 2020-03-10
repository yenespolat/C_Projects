//Yasin Enes Polat 150117015
#include <stdio.h>
#include <stdlib.h>

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ) //exit( 1 )
