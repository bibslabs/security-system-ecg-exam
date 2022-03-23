
#include <Arduino.h>
#include "clefia.h"
#include "speck.h"
#include "aes_cbc.h"
#include "crypto.h"
#include "speck.h"

typedef void (*enc_func)( uint8_t * input, uint8_t * output, size_t size);

typedef void (*dec_func)( uint8_t * input, uint8_t * output, size_t size);

static uint8_t key[32];

static enc_func enc_f = NULL;
static dec_func dec_f = NULL;
unsigned char clefia_rk[8 * 26 + 16];
unsigned char clefia_r;// clefia round
SimSpk_Cipher cipher_object;
static void clefia_enc_wrap(uint8_t * input, uint8_t *output,size_t size);
static void clefia_dec_wrap( uint8_t * input, uint8_t *output,size_t size);
static void aes_cbc_enc_wrap(uint8_t * input, uint8_t *output, size_t size);
static void aes_cbc_dec_wrap(uint8_t * input, uint8_t *output, size_t size);
static void speck_enc_wrap(uint8_t * input, uint8_t *output, size_t size);
static void speck_dec_wrap(uint8_t * input, uint8_t *output, size_t size);
static void no_encrypt(uint8_t * input, uint8_t *output, size_t size);
static void no_decrypt(uint8_t * input, uint8_t *output, size_t size);

void encrypt_data(uint8_t * input, uint8_t * output, size_t size){
    enc_f(input,output,size);
}

void decrypt_data(uint8_t * input, uint8_t * output, size_t size){
    dec_f(input,output,size);
}


void set_crypto_only(String crypto_str){
    if(crypto_str.equals("CLEFIA")){
        enc_f = clefia_enc_wrap;
        dec_f = clefia_dec_wrap;
        Serial.printf("Configured Clefia cryptographic algorithm\n");
    }else if (crypto_str == "AESCBC"){
        cbc_start(key,key);
        enc_f = aes_cbc_enc_wrap;
        dec_f = aes_cbc_dec_wrap;
        Serial.printf("Configured AES CBC 256 cryptographic algorithm\n");

    }else if(crypto_str == "SPECK"){
        enc_f = speck_enc_wrap;
        dec_f = speck_dec_wrap;
        Serial.printf("Configured Speck cryptographic algorithm\n");

    }else if(crypto_str == "NONE"){
        enc_f = no_encrypt;
        dec_f = no_decrypt;
        Serial.printf("Configure NO encryption");
    }
}

void set_crypto_bytes(String crypto_str, uint8_t * key_bytes){
    memcpy(key,key_bytes,32);
    Serial.printf("crypto algorithm -> %s, ",crypto_str.c_str());
    for(uint8_t i = 0 ; i < 32 ; i++){
        Serial.printf("%02X",key[i]);
    }
    Serial.printf("\n");
    String clefia = "CLEFIA";
    if(crypto_str.equals(clefia)){
        enc_f = clefia_enc_wrap;
        dec_f = clefia_dec_wrap;
        Serial.printf("Configured Clefia cryptographic algorithm\n");
    }else if (crypto_str == "AESCBC"){
        cbc_start(key,key);
        enc_f = aes_cbc_enc_wrap;
        dec_f = aes_cbc_dec_wrap;
        Serial.printf("Configured AES CBC 256 cryptographic algorithm\n");

    }else if(crypto_str == "SPECK"){
        enc_f = speck_enc_wrap;
        dec_f = speck_dec_wrap;
        Serial.printf("Configured Speck cryptographic algorithm\n");

    }else if(crypto_str == "NONE"){
        enc_f = no_encrypt;
        dec_f = no_decrypt;
        Serial.printf("Configure NO encryption");
    }    
}

void set_crypto(String crypto_str, String key_str){
    memcpy(key,key_str.c_str(),32);
    Serial.printf("crypto algorithm -> %s, ",crypto_str.c_str());
    for(uint8_t i = 0 ; i < 32 ; i++){
        Serial.printf("%c",key[i]);
    }
    Serial.printf("\n");
    String clefia = "CLEFIA";
    if(crypto_str.equals(clefia)){
        enc_f = clefia_enc_wrap;
        dec_f = clefia_dec_wrap;
        Serial.printf("Configured Clefia cryptographic algorithm\n");
    }else if (crypto_str == "AESCBC"){
        cbc_start(key,key);
        enc_f = aes_cbc_enc_wrap;
        dec_f = aes_cbc_dec_wrap;
        Serial.printf("Configured AES CBC 256 cryptographic algorithm\n");

    }else if(crypto_str == "SPECK"){
        enc_f = speck_enc_wrap;
        dec_f = speck_dec_wrap;
        Serial.printf("Configured Speck cryptographic algorithm\n");

    }else if(crypto_str == "NONE"){
        enc_f = no_encrypt;
        dec_f = no_decrypt;
        Serial.printf("Configure NO encryption");
    }   
}

static void clefia_enc_wrap(uint8_t * input, uint8_t *output,size_t size){
    clefia_r = ClefiaKeySet(clefia_rk, key, 256);
    for(uint32_t i = 0 ; i < size ;){
        // Serial.printf("piquezin\n");
        ClefiaEncrypt((unsigned char*)&output[i], (unsigned char*)&input[i], clefia_rk , clefia_r);
        i = i + 16;
    }
}

static void clefia_dec_wrap(uint8_t * input, uint8_t *output,size_t size){
    clefia_r = ClefiaKeySet(clefia_rk, key, 256);
    for(uint32_t i = 0 ; i < size ;){
        ClefiaDecrypt( (unsigned char*)&output[i], (unsigned char*)&input[i], clefia_rk, clefia_r);
        i = i + 16;
    }
}

static void aes_cbc_enc_wrap(uint8_t * input, uint8_t *output, size_t size){
    // cbc_start(key,key);
    aes_cbc_compress_chunk(input,output,size);
}

static void aes_cbc_dec_wrap(uint8_t * input, uint8_t *output, size_t size){
    // cbc_start(key,key);
    aes_cbc_decrypt_chunk(input,output,size);
}

static void speck_enc_wrap(uint8_t * input, uint8_t *output, size_t size){
    Speck_Init(&cipher_object,cfg_256_128,CBC,key,key,NULL);
    for(uint32_t i = 0 ; i < size; i+=16){
        Speck_Encrypt(cipher_object,(uint8_t*)&input[i],(uint8_t*)&output[i]);
    }
}

static void speck_dec_wrap(uint8_t * input, uint8_t *output, size_t size){
    Speck_Init(&cipher_object,cfg_256_128,CBC,key,key,NULL);
    for(uint32_t i = 0 ; i < size; i+=16){
        Speck_Decrypt(cipher_object,(uint8_t*)&input[i],(uint8_t*)&output[i]);
    }
}

static void no_encrypt(uint8_t * input, uint8_t *output, size_t size){
    (void)input;
    (void)output;
    (void)size;
}
static void no_decrypt(uint8_t * input, uint8_t *output, size_t size){
    (void)input;
    (void)output;
    (void)size;
}
