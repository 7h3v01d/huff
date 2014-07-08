#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "huff.h"


#define BUFF_SIZE 512


void help( void );

void encode( FILE *in, FILE *out );
void encode_writer( const void *data, unsigned int size, void *arg );

void decode( FILE *in, FILE *out );
void decode_writer( const void *data, unsigned int size, void *arg );


int main( int argc, char **argv )
{
	FILE *in = NULL;
	FILE *out = NULL;
	int dec = 0;


	if( 3 > argc ||
	    4 < argc ||
	    (4 == argc &&
	     (0 != strcmp("-c", argv[1]) &&
	      0 != strcmp("-d", argv[1]))) ) {
		help();
		return EXIT_FAILURE;
	}

	if( 4 == argc &&
	    0 == strcmp("-d", argv[1]) ) {
		dec = !0;
	}

	in = fopen( 3 == argc ? argv[1] : argv[2], "rb" );
	if( NULL == in ) {
		printf( "Unable to open input file: %m\n" );
		return EXIT_FAILURE;
	}

	out = fopen( 3 == argc ? argv[2] : argv[3], "wb" );
	if( NULL == out ) {
		printf( "Unable to open output file: %m\n" );
		fclose( in );
		return EXIT_FAILURE;
	}

	if( 0 == dec ) {
		encode( in, out );
	}
	else {
		decode( in, out );
	}

	fclose( out );
	fclose( in );

	return EXIT_SUCCESS;
}


void help( void )
{
	printf( "Huffman coder/encoder\n" );
	printf( "Usage: huff [-c | -d] input.file output.file\n" );
	printf( "Options: -c  Encode file\n" );
	printf( "         -d  Decode file\n" );
}

void encode( FILE *in, FILE *out )
{
	huff_enc_t huff;


	huff_init_enc( &huff, (const huff_proc_t *)encode_writer, out );

	do {
		char buff[BUFF_SIZE];
		size_t size = fread( buff, 1, BUFF_SIZE, in );
		if( 0 != size ) {
			huff_update_tree_enc( &huff, buff, size );
		}
		else {
			if( 0 != ferror(in) ) {
				printf( "Unable to read input file: %m\n" );
				return;
			}
			break;
		}
	} while( 1 );

	rewind( in );

	huff_final_tree_enc( &huff );

	do {
		char buff[BUFF_SIZE];
		size_t size = fread( buff, 1, BUFF_SIZE, in );
		if( 0 != size ) {
			huff_update_enc( &huff, buff, size );
		}
		else {
			if( 0 != ferror(in) ) {
				printf( "Unable to read input file: %m\n" );
				return;
			}
			break;
		}
	} while( 1 );

	huff_final_enc( &huff );
}

void encode_writer( const void *data, unsigned int size, void *arg )
{
	size_t siz = fwrite( data, 1, size, (FILE *)arg );
	if( siz != size ) {
		printf( "Unable to write output file: %m\n" );
	}
}

void decode( FILE *in, FILE *out )
{
	(void)in;
	(void)out;
}

void decode_writer( const void *data, unsigned int size, void *arg )
{
	size_t siz = fwrite( data, 1, size, (FILE *)arg );
	if( siz != size ) {
		printf( "Unable to write output file: %m\n" );
	}
}
