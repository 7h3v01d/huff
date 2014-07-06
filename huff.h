#ifndef __HUFF_H__
#define __HUFF_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct {
	char data[4096];
} huff_enc_t;


typedef void (huff_proc_t)( const void *data, unsigned int size, void *arg );


void huff_init_enc( huff_enc_t *ctx, const huff_proc_t *proc, const void *arg );
void huff_update_tree_enc( huff_enc_t *ctx, const void *data, unsigned int size );
void huff_final_tree_enc( huff_enc_t *ctx );
void huff_update_enc( huff_enc_t *ctx, const void *data, unsigned int size );
void huff_final_enc( huff_enc_t *ctx );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __HUFF_H__ */
