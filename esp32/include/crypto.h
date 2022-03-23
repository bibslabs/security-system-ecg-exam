

void encrypt_data(uint8_t * input, uint8_t * output, size_t size);

void decrypt_data(uint8_t * input, uint8_t * output, size_t size);

void set_crypto(String crypto_str, String key_str);

void set_crypto_bytes(String crypto_str, uint8_t * key_bytes);

void set_crypto_only(String crypto_str);