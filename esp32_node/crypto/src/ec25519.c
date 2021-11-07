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


#define PUBLIC_KEY_LEN  32
#define SZ_RANDOM       16

static const char* TAG = "DIFFI_HELLMAN";

typedef struct session {
    /* Session data */
    uint32_t id;
    uint8_t state;
    uint8_t device_pubkey[PUBLIC_KEY_LEN];
    uint8_t client_pubkey[PUBLIC_KEY_LEN];
    uint8_t sym_key[PUBLIC_KEY_LEN];
    uint8_t rand[SZ_RANDOM];

    /* mbedtls context data for AES */
    mbedtls_aes_context ctx_aes;
    unsigned char stb[16];
    size_t nc_off;
} session_t;

session_t cur_session;

static void flip_endian(uint8_t *data, size_t len)
{
    uint8_t swp_buf;
    for (int i = 0; i < len/2; i++) {
        swp_buf = data[i];
        data[i] = data[len - i - 1];
        data[len - i - 1] = swp_buf;
    }
}

static void hexdump(const char *msg, uint8_t *buf, int len)
{
    ESP_LOGD(TAG, "%s:", msg);
    ESP_LOG_BUFFER_HEX_LEVEL(TAG, buf, len, ESP_LOG_DEBUG);
}

void init_diffie_hellman(void){

    ESP_LOGD(TAG, "Start diffie hellman");
    esp_err_t ret;
    int mbed_err;

    mbedtls_ecdh_context     *ctx_server = malloc(sizeof(mbedtls_ecdh_context));
    mbedtls_entropy_context  *entropy    = malloc(sizeof(mbedtls_entropy_context));
    mbedtls_ctr_drbg_context *ctr_drbg   = malloc(sizeof(mbedtls_ctr_drbg_context));
    if (!ctx_server || !entropy || !ctr_drbg) {
        ESP_LOGE(TAG, "Failed to allocate memory for mbedtls context");
        free(ctx_server);
        free(entropy);
        free(ctr_drbg);
        return ESP_ERR_NO_MEM;
    }

    mbedtls_ecdh_init(ctx_server);
    mbedtls_ctr_drbg_init(ctr_drbg);
    mbedtls_entropy_init(entropy);

    mbed_err = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func,
                                     entropy, NULL, 0);
    if (mbed_err != 0) {
        ESP_LOGE(TAG, "Failed at mbedtls_ctr_drbg_seed with error code : -0x%x", -mbed_err);
        ret = ESP_FAIL;
        goto exit_cmd0;
    }

    mbed_err = mbedtls_ecp_group_load(&ctx_server->grp, MBEDTLS_ECP_DP_CURVE25519);
    if (mbed_err != 0) {
        ESP_LOGE(TAG, "Failed at mbedtls_ecp_group_load with error code : -0x%x", -mbed_err);
        ret = ESP_FAIL;
        goto exit_cmd0;
    }

    mbed_err = mbedtls_ecdh_gen_public(&ctx_server->grp, &ctx_server->d, &ctx_server->Q,
                                       mbedtls_ctr_drbg_random, ctr_drbg);
    if (mbed_err != 0) {
        ESP_LOGE(TAG, "Failed at mbedtls_ecdh_gen_public with error code : -0x%x", -mbed_err);
        ret = ESP_FAIL;
        goto exit_cmd0;
    }

    mbed_err = mbedtls_mpi_write_binary(&ctx_server->Q.X,
                                        cur_session.device_pubkey,
                                        PUBLIC_KEY_LEN);
    if (mbed_err != 0) {
        ESP_LOGE(TAG, "Failed at mbedtls_mpi_write_binary with error code : -0x%x", -mbed_err);
        ret = ESP_FAIL;
        goto exit_cmd0;
    }
    flip_endian(cur_session.device_pubkey, PUBLIC_KEY_LEN);

    // memcpy(cur_session->client_pubkey, in->sc0->client_pubkey.data, PUBLIC_KEY_LEN);

    uint8_t *dev_pubkey = cur_session.device_pubkey;
    uint8_t *cli_pubkey = cur_session.client_pubkey;
    hexdump("Device pubkey", dev_pubkey, PUBLIC_KEY_LEN);
    hexdump("Client pubkey", cli_pubkey, PUBLIC_KEY_LEN);

    mbed_err = mbedtls_mpi_lset(&ctx_server->Qp.Z, 1);
    if (mbed_err != 0) {
        ESP_LOGE(TAG, "Failed at mbedtls_mpi_lset with error code : -0x%x", -mbed_err);
        ret = ESP_FAIL;
        goto exit_cmd0;
    }

exit_cmd0:
    mbedtls_ecdh_free(ctx_server);
    free(ctx_server);

    mbedtls_ctr_drbg_free(ctr_drbg);
    free(ctr_drbg);

    mbedtls_entropy_free(entropy);
    free(entropy);

    return ret;
}

