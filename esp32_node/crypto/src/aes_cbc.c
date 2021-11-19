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

uint8_t key[32];
uint8_t iv[16];
esp_aes_context ctx;

//Rotina para testar as capacidades criptograficas do ESP32
//validado AES CBC como criptografia de otima qualidade e velocidade

//ser keys
void cbc_start(uint8_t *my_key, uint8_t *my_iv){
	//se seus dados estao como texto, eh necessario um IV para randomizar o dado ainda mais
	memcpy(key,my_key,sizeof(key));
	memcpy(iv,my_iv,sizeof(iv));

	esp_aes_init( &ctx );
	esp_aes_setkey( &ctx, key, 256 );
}

bool aes_cbc_compress_chunk(const uint8_t *input,uint8_t *output, uint32_t size){
	return (esp_aes_crypt_cbc( &ctx, ESP_AES_ENCRYPT, size, iv, (uint8_t*)input, (uint8_t*)output ));
}
