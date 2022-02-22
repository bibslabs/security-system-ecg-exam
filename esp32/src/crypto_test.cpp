#include "Arduino.h"
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
#include "clefia.h"

#define STR_SIZE (16384)
#define ALGOS (5)

#define AES_ENC     "AESCBC  encrypt"
#define AES_DEC     "AESCBC  decrypt"
#define SIMON_ENC   "Simon   encrypt"
#define SIMON_DEC   "Simon   decrypt"
#define SPECK_ENC   "Speck   encrypt"
#define SPECK_DEC   "Speck   decrypt"
#define PRESENT_ENC "Present encrypt"
#define PRESENT_DEC "Present decrypt"
#define CLEFIA_ENC  "Clefia  encrypt"
#define CLEFIA_DEC  "Clefia  decrypt"


typedef struct bench_results {
    double MBs;
    char * algo;
    uint32_t latency;
}bench_results_t;

static bench_results_t benchs[ALGOS*2] = {
    {.MBs = 0.0,.algo = AES_ENC,.latency = 0},
    {.MBs = 0.0,.algo = AES_DEC,.latency = 0},
    {.MBs = 0.0,.algo = SIMON_ENC,.latency = 0},
    {.MBs = 0.0,.algo = SIMON_DEC,.latency = 0},
    {.MBs = 0.0,.algo = SPECK_ENC,.latency = 0},
    {.MBs = 0.0,.algo = SPECK_DEC,.latency = 0},
    {.MBs = 0.0,.algo = PRESENT_ENC,.latency = 0},
    {.MBs = 0.0,.algo = PRESENT_DEC,.latency = 0},
    {.MBs = 0.0,.algo = CLEFIA_ENC,.latency = 0},
    {.MBs = 0.0,.algo = CLEFIA_DEC,.latency = 0},
};

// static const char *TAG = "CRYPTO TEST";

static char sequencia[STR_SIZE] = "Esta eh uma grande string que contem muitos dados, ela ira passar por uma criptografia para que seja validada a sua capacidade de cifrar e decifrar dados, ela possui 16 KBYTES de tamanho, mas esta frase está contida apenas em seu inicio \0\0\0\0\0\0";
static char resultado[STR_SIZE];

static char temp[STR_SIZE];

///aes CBC keys
static uint8_t key_256[32] = {0xAA,0xBB,0xCC,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x22,0x23,0x24,0x25,0x02}; //AES CBC
static uint8_t key_128[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x22,0x23,0x24,0x25,0x027}; //simon, speck
static uint8_t key_80[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10};  //present


static inline int32_t _getCycleCount(void) {
  int32_t ccount;
  __asm__ __volatile__("rsr %0,ccount":"=a" (ccount));
  return ccount;
}

static uint32_t cycle_start,cycle_end;
static uint8_t mbai = 0; //my bench array index
static void test_aes_cbc(void){
    Serial.printf("AES CBC\n\r");
    Serial.printf("Phrase -> %s \n\r",sequencia);
    cycle_start = _getCycleCount();
    cbc_start(key_256,key_128); //usando a chave de 128 como init vector
    aes_cbc_compress_chunk((uint8_t*)sequencia,(uint8_t*)resultado,STR_SIZE);
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end / (STR_SIZE/16);
    
    Serial.printf("Result (%d) cycles) %s \n\n\r",cycle_end,resultado);
    //necessario resetar o init vector quando iniciar uma descriptografia senao o primeiro bloco vem corrompido
    aes_cbc_reset_iv(key_128);
    cycle_start = _getCycleCount();
    aes_cbc_decrypt_chunk((uint8_t*)resultado,(uint8_t*)temp,STR_SIZE);
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end / (STR_SIZE/16);
    Serial.printf("Result decrypto (%d cycles) -> %s \n\n\r",cycle_end,temp);
}

static void test_simon(void){
    Serial.printf("Simon block cipher 256 bits\n\r");
    SimSpk_Cipher cipher_object;
    Simon_Init(&cipher_object,cfg_256_128,CBC,key_256,key_128,NULL);
    Serial.printf("Phrase -> %s \n\n\r",sequencia);

    cycle_start = _getCycleCount();
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=16){
        // Simon_Encrypt_128(40,key_128,&sequencia[i],&resultado[i]);
        Simon_Encrypt(cipher_object,(uint8_t*)&sequencia[i],(uint8_t*)&resultado[i]);
    }
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end/(STR_SIZE/16);

    Serial.printf("Result (%d cycles) %s \n\n\r",cycle_end,resultado);
    cycle_start = _getCycleCount();
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=16){
        Simon_Decrypt(cipher_object,(uint8_t*)&resultado[i],(uint8_t*)&temp[i]);
    }
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end / (STR_SIZE/16);
    Serial.printf("Decrypted (%d cycles) -> %s \n\n\r",cycle_end,temp);
}

static void test_speck(void){
    SimSpk_Cipher cipher_object;
    Serial.printf("Speck block cipher 256 bits\n\r");
    Speck_Init(&cipher_object,cfg_256_128,CBC,key_256,key_128,NULL);
    Serial.printf("Phrase -> %s \n\n\r",sequencia);
    cycle_start = _getCycleCount();
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=16){
        Speck_Encrypt(cipher_object,(uint8_t*)&sequencia[i],(uint8_t*)&resultado[i]);
    }
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end / (STR_SIZE/16);

    Serial.printf("Result (%d cycles) -> %s \n\n\r",cycle_end,resultado);
    cycle_start = _getCycleCount();
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=16){
        Speck_Decrypt(cipher_object,(uint8_t*)&resultado[i],(uint8_t*)&temp[i]);
    }
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end/ (STR_SIZE/16);

    Serial.printf("Decrypted (%d cycles) -> %s \n\n\r",cycle_end,temp);
}

static void test_present(void){
    Serial.printf("Present Block cipher\n\r");
    Serial.printf("Phrase -> %s \n\n\r",sequencia);
    cycle_start = _getCycleCount();
    for(uint32_t i = 0 ; i < sizeof(sequencia); i+=8){
        // result = present_encrypt(&sequencia[i],(char*)&key_128[0]);
        present_encrypt((uint8_t*)&sequencia[i], key_128);
    }
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end / (STR_SIZE/8);

    Serial.printf("Encrypted Result (%d cycles) -> %s \n\n\r",cycle_end,resultado);
    cycle_start = _getCycleCount();
     for(uint32_t i = 0 ; i < sizeof(sequencia); i+=8){
        // present_decrypt(&sequencia[i],(char*)&key_80[0]);
        present_decrypt((uint8_t*)&resultado[i], key_128);
    }   
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end/(STR_SIZE/8);

    Serial.printf("Decrypted Result (%d cycles) -> %s \n\n\r",cycle_end,temp);
}

static void test_clefia(void){
  Serial.printf("Clefia Block cipher\n\r");
    //CLEFI CONFIG
    unsigned char rk[8 * 26 + 16];
    int r = ClefiaKeySet(rk, key_256, 256);
    cycle_start = _getCycleCount();
    for(uint32_t i = 0 ; i < STR_SIZE ;){
        ClefiaEncrypt((unsigned char*)&resultado[i], (unsigned char*)&temp[i], rk , r);
        i = i + 16;
    }
    //CLEFIA ENCRYPT
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end / (STR_SIZE/16);

    Serial.printf("Result (%d cycles) -> %s \n\n\r",cycle_end,resultado);
    //CLEFIA DECRYPT
    r = ClefiaKeySet(rk, key_256, 256);
    cycle_start = _getCycleCount();
    for(uint32_t i = 0 ; i < STR_SIZE ;){
        ClefiaDecrypt( (unsigned char*)&temp[i], (unsigned char*)&resultado[i], rk, r);
        i = i + 16;
    }
    cycle_end = _getCycleCount() - cycle_start;
    benchs[mbai].MBs = (STR_SIZE*1.0)/(cycle_end/240.0);
    benchs[mbai++].latency = cycle_end / (STR_SIZE/16);

    Serial.printf("Decrypted (%d cycles) -> %s \n\n\r",cycle_end,temp);


}

void test_all_cryptos(void){
    Serial.println("Testando todas as criptografias");
    (void)key_80;
    (void)key_128;



    Serial.printf("Starting cryptography test\n\r");

    test_aes_cbc();

    test_simon();

    test_speck();

    test_present();

    test_clefia();

  
    Serial.printf("\n\n\r@@ FIM TO TESTE DE CRIPTOGRAFIAS\n\r");
    Serial.printf("Resultados:\n\r");
    for(uint8_t i = 0 ; i < (ALGOS * 2) ; i++){
        Serial.printf("%s, Throughput: %f MB/s, Latency: %d cycles/block \n\r",benchs[i].algo,benchs[i].MBs,benchs[i].latency);
    }



}