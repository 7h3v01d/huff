#ifndef NULL
#define NULL ( (void)0 )
#endif /* NULL */


typedef void (huff_proc_t)( const void *data, unsigned int size, void *arg );


typedef struct {
	unsigned char symb;
	unsigned int freq;
	unsigned char code;
} huff_node_t;

typedef struct {
	huff_proc_t *proc;
	void *arg;
	unsigned char size;
	huff_node_t root[256];
} huff_enc_t;


void huff_init_enc( huff_enc_t *ctx, const huff_proc_t *proc, const void *arg )
{
	ctx->proc = proc;
	ctx->arg = (void *)arg;
	ctx->size = 0;

	for( unsigned int i = 0; 256 > i; ++i ) {
		ctx->root[i].symb = (unsigned char)i;
		ctx->root[i].freq = 0;
		ctx->root[i].code = 0;
	}
}

void huff_update_tree_enc( huff_enc_t *ctx, const void *data, unsigned int size )
{
	for( unsigned int i = 0; i < size; ++i ) {
		++ctx->root[((unsigned char *)data)[i]].freq;
	}
}
#include <stdio.h>//
void huff_final_tree_enc( huff_enc_t *ctx )
{
	ctx->proc( ctx->root, 256*9, ctx->arg );

	for( int i = 0; 256 > i; ++i ) {
		printf( "i: %d, symb: %d, freq: %u, code: %d\n",
			i,
			ctx->root[i].symb,
			ctx->root[i].freq,
			ctx->root[i].code );
	}//

	/* TODO: this is bubblesort O(n^2) -> use quicksort O(n log n) instead. */
	for( unsigned int i = 0; 255 > i; ++i ) {
		for( unsigned int j = i+1; 256 > j; ++j ) {
			if( ctx->root[i].freq < ctx->root[j].freq ) {
				huff_node_t t = ctx->root[i];
				ctx->root[i] = ctx->root[j];
				ctx->root[j] = t;
			}
		}
	}

	for( int i = 0; 256 > i; ++i ) {
		printf( "i: %d, symb: %d, freq: %u, code: %d\n",
			i,
			ctx->root[i].symb,
			ctx->root[i].freq,
			ctx->root[i].code );
	}//

	for( unsigned int i = 0; 256 > i; ++i ) {
		if( 0 == ctx->root[i].freq ) {
			break;
		}
		++ctx->size;
	}

	/*
	// test
	for( unsigned int i = 0, j = 255, k = ctx->root[0].freq; i < ctx->size; ++i ) {
		if( k == ctx->root[i].freq ) {
			k = ctx->root[i].freq;
			ctx->root[i].freq = j;
		}
		else {
			k = ctx->root[i].freq;
			ctx->root[i].freq = --j;
		}
	}

	for( int i = 0; 256 > i; ++i ) {
		printf( "i: %d, symb: %d, freq: %u, code: %d\n",
			i,
			ctx->root[i].symb,
			ctx->root[i].freq,
			ctx->root[i].code );
	}//
	*/

	printf( "size: %u\n", ctx->size );//
}


void huff_update_enc( huff_enc_t *ctx, const void *data, unsigned int size )
{
	(void)ctx;
	(void)data;
	(void)size;
}

void huff_final_enc( huff_enc_t *ctx )
{
	(void)ctx;
}
