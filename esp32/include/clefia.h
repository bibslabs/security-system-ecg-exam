void ByteCpy(unsigned char *dst, const unsigned char *src, int bytelen);
void ByteXor(unsigned char *dst, const unsigned char *a, const unsigned char *b, int bytelen);

unsigned char ClefiaMul2(unsigned char x);
void ClefiaF0Xor(unsigned char *y, const unsigned char *x, const unsigned char *rk);
void ClefiaF1Xor(unsigned char *y, const unsigned char *x, const unsigned char *rk);
void ClefiaGfn4(unsigned char *y, const unsigned char *x, const unsigned char *rk, int r);
void ClefiaGfn8(unsigned char *y, const unsigned char *x, const unsigned char *rk, int r);
void ClefiaGfn4Inv(unsigned char *y, const unsigned char *x, const unsigned char *rk, int r);

void ClefiaDoubleSwap(unsigned char *lk);
void ClefiaConSet(unsigned char *con, const unsigned char *iv, int lk);
void ClefiaKeySet128(unsigned char *rk, const unsigned char *skey);
void ClefiaKeySet192(unsigned char *rk, const unsigned char *skey);
void ClefiaKeySet256(unsigned char *rk, const unsigned char *skey);

int ClefiaKeySet(unsigned char *rk, const unsigned char *skey, const int key_bitlen);
void ClefiaEncrypt(unsigned char *ct, const unsigned char *pt, const unsigned char *rk, const int r);
void ClefiaDecrypt(unsigned char *pt, const unsigned char *ct, const unsigned char *rk, const int r);
