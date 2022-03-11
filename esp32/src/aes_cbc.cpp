#include <string.h>
#include <stdio.h>
#include "mbedtls/aes.h"
#include "aes_cbc.h"
/* 
  Tempo de encriptacao: 1802.40us  (9.09 MB/s) com blocos de 16 kB.
*/

//rotina para contar ciclos de clock
static inline int32_t _getCycleCount(void) {
  int32_t ccount;
  __asm__ __volatile__("rsr %0,ccount":"=a" (ccount));
  return ccount;
}

char plaintext[16384];
char encrypted[16384];

static uint8_t key[32];
static uint8_t iv[16];
static uint8_t glob_key[32];
static uint8_t glob_iv[16];;
static esp_aes_context ctx;

//Rotina para testar as capacidades criptograficas do ESP32
//validado AES CBC como criptografia de otima qualidade e velocidade

//ser keys
void cbc_start(uint8_t *my_key, uint8_t *my_iv){
	//se seus dados estao como texto, eh necessario um IV para randomizar o dado ainda mais
	memcpy(glob_key,my_key,sizeof(key));
	memcpy(glob_iv,my_iv,sizeof(iv));
	memcpy(key,glob_key,sizeof(key));
	memcpy(iv,glob_iv,sizeof(iv));
	
	esp_aes_init( &ctx );
	esp_aes_setkey( &ctx, key, 256 );
}

void aes_cbc_reset_iv(uint8_t *my_iv){
	memcpy(iv,glob_iv,sizeof(iv));
}

bool aes_cbc_compress_chunk(const uint8_t *input,uint8_t *output, uint32_t size){
	cbc_start(glob_key,glob_iv);
	return (esp_aes_crypt_cbc( &ctx, ESP_AES_ENCRYPT, size, iv, (uint8_t*)input, (uint8_t*)output ));
}

bool aes_cbc_decrypt_chunk(const uint8_t *input, uint8_t *output, uint32_t size){
	cbc_start(glob_key,glob_iv);
	return (esp_aes_crypt_cbc( &ctx, ESP_AES_DECRYPT, size, iv, (uint8_t*)input, (uint8_t*)output ));
}