
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "aes_cbc.h"
#include "crypto_test.h"
#include "ec25519.h"
#include "present.h"
#include "simon.h"
#include "speck.h"
#include "esp_log.h"
#include "simon.h"

#define STR_SIZE (512)

static const char *TAG = "CRYPTO TEST";

static char sequencia[STR_SIZE] = "Esta eh uma grande string que contem muitos dados, ela ira passar por uma criptografia para que seja validada a sua capacidade de cifrar e decifrar dados";
static char resultado[STR_SIZE];

static char temp[STR_SIZE];

///aes CBC keys
static uint8_t key_256[32] = {0xAA,0xBB,0xCC,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x22,0x23,0x24,0x25,0x02}; //AES CBC
static uint8_t key_128[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x22,0x23,0x24,0x25,0x027}; //simon, speck
static uint8_t key_80[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10};  //present


void test_all_cryptos(void){
    (void)key_80;
    (void)key_128;
    //test aes CBC
    ESP_LOGI(TAG,"Starting cryptography test");
    ESP_LOGI(TAG,"AES CBC");
    ESP_LOGI(TAG,"Phrase -> %s",sequencia);
    cbc_start(key_256,key_128); //usando a chave de 128 como init vector
    aes_cbc_compress_chunk((uint8_t*)sequencia,(uint8_t*)resultado,STR_SIZE);
    // ESP_LOGI(TAG,"Result %s",resultado);
    //necessario resetar o init vector quando iniciar uma descriptografia senao o primeiro bloco vem corrompido
    aes_cbc_reset_iv(key_128);
    aes_cbc_decrypt_chunk((uint8_t*)resultado,(uint8_t*)temp,STR_SIZE);
    ESP_LOGI(TAG,"Result decrypto -> %s",temp);

    //test PRESENT
        
    //test SIMON
    ESP_LOGI(TAG,"Simon block cipher 128 bits");
    ESP_LOGI(TAG,"Phrase -> %s",sequencia);
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=64){
        Simon_Encrypt_128(40,key_128,(uint8_t*)&sequencia[i],(uint8_t*)&resultado[i]);
    }
    ESP_LOGI(TAG,"Result %s",resultado);
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=64){
        Simon_Decrypt_128(40,key_128,(uint8_t*)&resultado[i],(uint8_t*)&temp[i]);
    }
    ESP_LOGI(TAG,"Decrypted -> %s",temp);
    //test SPECK
    ESP_LOGI(TAG,"Speck block cipher 128 bits");
    ESP_LOGI(TAG,"Phrase -> %s",sequencia);
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=64){
        Speck_Encrypt_128(40,key_128,(uint8_t*)&sequencia[i],(uint8_t*)&resultado[i]);
    }
    ESP_LOGI(TAG,"Result %s",resultado);
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=64){
        Speck_Decrypt_128(40,key_128,(uint8_t*)&resultado[i],(uint8_t*)&temp[i]);
    }
}