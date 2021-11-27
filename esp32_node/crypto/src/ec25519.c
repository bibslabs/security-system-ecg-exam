#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>

#include <mbedtls/aes.h>
#include <mbedtls/sha256.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/ecdh.h>
#include <mbedtls/error.h>
#include <mbedtls/ssl_internal.h>

#include "ec25519.h"


// static void hexdump(const char *msg, uint8_t *buf, int len)
// {
//     ESP_LOGI(TAG, "%s:", msg);
//     ESP_LOG_BUFFER_HEX_LEVEL(TAG, buf, len, ESP_LOG_DEBUG);
// }

void init_diffie_hellman(void){

    // int ret = 1;
    // int exit_code;
    // mbedtls_ecdh_context ctx_cli, ctx_srv;
    // mbedtls_entropy_context entropy;
    // mbedtls_ctr_drbg_context ctr_drbg;
    // unsigned char cli_to_srv[32], srv_to_cli[32];
    // const char pers[] = "ecdh";

    // mbedtls_ecdh_init( &ctx_cli );
    // mbedtls_ecdh_init( &ctx_srv );
    // mbedtls_ctr_drbg_init( &ctr_drbg );

}

