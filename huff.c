#ifndef NULL
#define NULL ( (void *)0 )
#endif /* NULL */


typedef void (huff_proc_t)( const void *data, unsigned int size, void *arg );


typedef struct __attribute__( (__packed__) ) {
	unsigned char symb;
	unsigned int freq;
	unsigned char code;
	struct huff_node_t *left;
	struct huff_node_t *right;
} huff_node_t;

typedef struct __attribute__( (__packed__) ) {
	huff_node_t *left;
	huff_node_t *right;
} huff_root_t;

typedef struct __attribute__( (__packed__) ) {
	huff_proc_t *proc;
	void *arg;
	huff_root_t root;
	unsigned char start;
	unsigned char size;
	huff_node_t prob[256];
} huff_enc_t;


void huff_init_enc( huff_enc_t *ctx, const huff_proc_t *proc, const void *arg )
{
	ctx->proc = proc;
	ctx->arg = (void *)arg;
	ctx->root.left = NULL;
	ctx->root.right = NULL;
	ctx->start = 0;
	ctx->size = 0;

	for( unsigned int i = 0; 256 > i; ++i ) {
		ctx->prob[i].symb = (unsigned char)i;
		ctx->prob[i].freq = 0;
		ctx->prob[i].code = 0;
		ctx->prob[i].left = NULL;
		ctx->prob[i].right = NULL;
	}
}

void huff_update_tree_enc( huff_enc_t *ctx, const void *data, unsigned int size )
{
	for( unsigned int i = 0; i < size; ++i ) {
		++ctx->prob[((unsigned char *)data)[i]].freq;
	}
}
#include <stdio.h>//
void huff_final_tree_enc( huff_enc_t *ctx )
{
	ctx->proc( ctx->prob, sizeof(huff_node_t)*256, ctx->arg );//

	for( int i = 0; 256 > i; ++i ) {
		printf( "i: %d, symb: %d, freq: %u, code: %d\n",
			i,
			ctx->prob[i].symb,
			ctx->prob[i].freq,
			ctx->prob[i].code );
	}//

	/* TODO: this is bubblesort O(n^2) -> use quicksort O(n log n) instead. */
	for( unsigned int i = 0; 255 > i; ++i ) {
		for( unsigned int j = i+1; 256 > j; ++j ) {
			if( ctx->prob[i].freq > ctx->prob[j].freq ) {
				huff_node_t t = ctx->prob[i];
				ctx->prob[i] = ctx->prob[j];
				ctx->prob[j] = t;
			}
		}
	}

	for( int i = 0; 256 > i; ++i ) {
		printf( "i: %d, symb: %d, freq: %u, code: %d\n",
			i,
			ctx->prob[i].symb,
			ctx->prob[i].freq,
			ctx->prob[i].code );
	}//

	for( unsigned int i = 0; 256 > i; ++i ) {
		if( 0 != ctx->prob[i].freq ) {
			break;
		}
		++ctx->start;
	}

	ctx->size = 256-ctx->start;

	/*
	// test
	for( unsigned int i = 0, j = 255, k = ctx->prob[0].freq; i < ctx->size; ++i ) {
		if( k == ctx->prob[i].freq ) {
			k = ctx->prob[i].freq;
			ctx->prob[i].freq = j;
		}
		else {
			k = ctx->prob[i].freq;
			ctx->prob[i].freq = --j;
		}
	}

	for( int i = 0; 256 > i; ++i ) {
		printf( "i: %d, symb: %d, freq: %u, code: %d\n",
			i,
			ctx->prob[i].symb,
			ctx->prob[i].freq,
			ctx->prob[i].code );
	}//
	*/

	printf( "start: %u\n", ctx->start );//
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
