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

// #define PUBLIC_KEY_LEN  32
// #define SZ_RANDOM       16

// uint8_t my_key[PUBLIC_KEY_LEN] = {0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,
//                                   0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,
//                                   0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,
//                                   0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B};

// static const char* TAG = "DIFFI_HELLMAN";

// typedef struct session {
//     /* Session data */
//     uint32_t id;
//     uint8_t state;
//     uint8_t device_pubkey[PUBLIC_KEY_LEN];
//     uint8_t client_pubkey[PUBLIC_KEY_LEN];
//     uint8_t sym_key[PUBLIC_KEY_LEN];
//     uint8_t rand[SZ_RANDOM];

//     /* mbedtls context data for AES */
//     mbedtls_aes_context ctx_aes;
//     unsigned char stb[16];
//     size_t nc_off;
// } session_t;

// session_t cur_session;

// static void flip_endian(uint8_t *data, size_t len)
// {
//     uint8_t swp_buf;
//     for (int i = 0; i < len/2; i++) {
//         swp_buf = data[i];
//         data[i] = data[len - i - 1];
//         data[len - i - 1] = swp_buf;
//     }
// }

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

