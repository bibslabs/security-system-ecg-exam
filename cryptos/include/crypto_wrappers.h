



static void clefia_enc_wrap(const uint8_t * input, uint8_t * output, size_t size){
    r = ClefiaKeySet(rk, key, 256);
    for(unsigned int i = 0; i < size ; i+=16){
        ClefiaEncrypt(&output[i],&input[i],rk,r);
    }

}

static void clefia_dec_wrap(const uint8_t *input, uint8_t * output , size_t size){
    r = ClefiaKeySet(rk, key, 256);
    for(unsigned int i = 0; i < size ; i+=16){
        ClefiaDecrypt(&output[i],&input[i],rk,r);
    }
}

static void speck_enc_wrap(const uint8_t * input, uint8_t * output, size_t size){
    Speck_Init(&simspeck_ctx,cfg_256_128,MODE_CBC,key,key,NULL);
    for(unsigned int i = 0; i < size ; i+=16){
        Speck_Encrypt(simspeck_ctx,&input[i],&output[i]);
    }
}

static void speck_dec_wrap(const uint8_t * input, uint8_t * output, size_t size){
    Speck_Init(&simspeck_ctx,cfg_256_128,MODE_CBC,key,key,NULL);  
    for(unsigned int i = 0; i < size ; i+=16){
        Speck_Decrypt(simspeck_ctx,&input[i],&output[i]);
    }
}

static void present_enc_wrap(const uint8_t * input, uint8_t * output, size_t size){
    for(unsigned int i = 0 ; i < size ; i += 8){
        memcpy(&output[i],&input[i],8);
        present_encrypt(&output[i],key);
    }
}

static void present_dec_wrap(const uint8_t * input, uint8_t * output, size_t size){
    for(unsigned int i = 0; i < size ; i+= 8){
        memcpy(&output[i],&input[i],8);
        present_decrypt(&output[i],key);
    }
}

static void AES_CBC_init_wrapper(uint8_t * key){
  AES_init_ctx_iv(&global_ctx,key,key);
}

static void AES_CBC_enc_wrapper(const uint8_t * input, uint8_t *output, size_t size){
  AES_CBC_init_wrapper(key);
  memcpy(output,input,size);
  AES_CBC_encrypt_buffer(&global_ctx,output,size);
}

static void AES_CBC_dec_wrapper(const uint8_t * input, uint8_t *output, size_t size){
  AES_CBC_init_wrapper(key);
  memcpy(output,input,size);
  AES_CBC_decrypt_buffer(&global_ctx,output,size);
}


