#include <string.h>
#include <stdio.h>
#include "mbedtls/aes.h"

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

int encrypt_data(void){


	//se seus dados estao como texto, eh necessario um IV para randomizar o dado ainda mais
	memset( iv, 0, sizeof( iv ) );

	//adicionar uma chave, uma chave toda em zeros pode ate ser valida, mas nao recomendada
	memset( key, 0, sizeof( key ) );


	//supondo a criptografia de um textinho
	memset( plaintext, 0, sizeof( plaintext ) );
	strcpy( plaintext, "Hello, world, how are you doing today?" );


	esp_aes_init( &ctx );
	esp_aes_setkey( &ctx, key, 256 );
	esp_aes_crypt_cbc( &ctx, ESP_AES_ENCRYPT, sizeof(plaintext), iv, (uint8_t*)plaintext, (uint8_t*)encrypted );

	return 0;
}

int decrypt_data(void){
	printf( "IV: %02x %02x\n", iv[0], iv[1] );
	memset( iv, 0, sizeof( iv ) );  


	//Desencriptar o bloco cifrado
	esp_aes_crypt_cbc( &ctx, ESP_AES_DECRYPT, sizeof(encrypted), iv, (uint8_t*)encrypted, (uint8_t*)plaintext );
	esp_aes_free( &ctx );
	return 0;
}
