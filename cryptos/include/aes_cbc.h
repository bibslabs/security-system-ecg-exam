

void cbc_start(uint8_t *my_key, uint8_t *my_iv);
bool aes_cbc_compress_chunk(const uint8_t *input,uint8_t *output, uint32_t size);
bool aes_cbc_decrypt_chunk(const uint8_t *input, uint8_t *output, uint32_t size);
void aes_cbc_reset_iv(uint8_t *my_iv);