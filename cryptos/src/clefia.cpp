/******************************************************************************
 * Copyright 2007, 2008 Sony Corporation
 *
 * clefia_ref.c
 *
 * "The 128-bit Blockcipher CLEFIA"
 * Reference ANSI C code
 *
 * Version  1.0.1 (August 26 2008)
 *
 * NOTICE
 * This reference code is written for a clear understanding of the CLEFIA
 * blockcipher algorithm based on the specification of CLEFIA.
 * Therefore, this code does not include any optimizations for
 * high-speed or low-cost implementations or any countermeasures against
 * implementation attacks.
 *
 *****************************************************************************/

// void inline ByteCpy(unsigned char *dst, const unsigned char *src, int bytelen);
void ByteXor(unsigned char *dst, const unsigned char *a, const unsigned char *b, int bytelen);

void inline ClefiaF0Xor(unsigned char *y, const unsigned char *x, const unsigned char *rk);
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

#define ByteCpy(x,y,z) memcpy(x,y,z)

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
/* S0 (8-bit S-box based on four 4-bit S-boxes) */
const unsigned char clefia_s0[256] = {
  0x57U, 0x49U, 0xd1U, 0xc6U, 0x2fU, 0x33U, 0x74U, 0xfbU,
  0x95U, 0x6dU, 0x82U, 0xeaU, 0x0eU, 0xb0U, 0xa8U, 0x1cU,
  0x28U, 0xd0U, 0x4bU, 0x92U, 0x5cU, 0xeeU, 0x85U, 0xb1U,
  0xc4U, 0x0aU, 0x76U, 0x3dU, 0x63U, 0xf9U, 0x17U, 0xafU,
  0xbfU, 0xa1U, 0x19U, 0x65U, 0xf7U, 0x7aU, 0x32U, 0x20U,
  0x06U, 0xceU, 0xe4U, 0x83U, 0x9dU, 0x5bU, 0x4cU, 0xd8U,
  0x42U, 0x5dU, 0x2eU, 0xe8U, 0xd4U, 0x9bU, 0x0fU, 0x13U,
  0x3cU, 0x89U, 0x67U, 0xc0U, 0x71U, 0xaaU, 0xb6U, 0xf5U,
  0xa4U, 0xbeU, 0xfdU, 0x8cU, 0x12U, 0x00U, 0x97U, 0xdaU,
  0x78U, 0xe1U, 0xcfU, 0x6bU, 0x39U, 0x43U, 0x55U, 0x26U,
  0x30U, 0x98U, 0xccU, 0xddU, 0xebU, 0x54U, 0xb3U, 0x8fU,
  0x4eU, 0x16U, 0xfaU, 0x22U, 0xa5U, 0x77U, 0x09U, 0x61U,
  0xd6U, 0x2aU, 0x53U, 0x37U, 0x45U, 0xc1U, 0x6cU, 0xaeU,
  0xefU, 0x70U, 0x08U, 0x99U, 0x8bU, 0x1dU, 0xf2U, 0xb4U,
  0xe9U, 0xc7U, 0x9fU, 0x4aU, 0x31U, 0x25U, 0xfeU, 0x7cU,
  0xd3U, 0xa2U, 0xbdU, 0x56U, 0x14U, 0x88U, 0x60U, 0x0bU,
  0xcdU, 0xe2U, 0x34U, 0x50U, 0x9eU, 0xdcU, 0x11U, 0x05U,
  0x2bU, 0xb7U, 0xa9U, 0x48U, 0xffU, 0x66U, 0x8aU, 0x73U,
  0x03U, 0x75U, 0x86U, 0xf1U, 0x6aU, 0xa7U, 0x40U, 0xc2U,
  0xb9U, 0x2cU, 0xdbU, 0x1fU, 0x58U, 0x94U, 0x3eU, 0xedU,
  0xfcU, 0x1bU, 0xa0U, 0x04U, 0xb8U, 0x8dU, 0xe6U, 0x59U,
  0x62U, 0x93U, 0x35U, 0x7eU, 0xcaU, 0x21U, 0xdfU, 0x47U,
  0x15U, 0xf3U, 0xbaU, 0x7fU, 0xa6U, 0x69U, 0xc8U, 0x4dU,
  0x87U, 0x3bU, 0x9cU, 0x01U, 0xe0U, 0xdeU, 0x24U, 0x52U,
  0x7bU, 0x0cU, 0x68U, 0x1eU, 0x80U, 0xb2U, 0x5aU, 0xe7U,
  0xadU, 0xd5U, 0x23U, 0xf4U, 0x46U, 0x3fU, 0x91U, 0xc9U,
  0x6eU, 0x84U, 0x72U, 0xbbU, 0x0dU, 0x18U, 0xd9U, 0x96U,
  0xf0U, 0x5fU, 0x41U, 0xacU, 0x27U, 0xc5U, 0xe3U, 0x3aU,
  0x81U, 0x6fU, 0x07U, 0xa3U, 0x79U, 0xf6U, 0x2dU, 0x38U,
  0x1aU, 0x44U, 0x5eU, 0xb5U, 0xd2U, 0xecU, 0xcbU, 0x90U,
  0x9aU, 0x36U, 0xe5U, 0x29U, 0xc3U, 0x4fU, 0xabU, 0x64U,
  0x51U, 0xf8U, 0x10U, 0xd7U, 0xbcU, 0x02U, 0x7dU, 0x8eU
};

/* S1 (8-bit S-box based on inverse function) */
const unsigned char clefia_s1[256] = {
  0x6cU, 0xdaU, 0xc3U, 0xe9U, 0x4eU, 0x9dU, 0x0aU, 0x3dU,
  0xb8U, 0x36U, 0xb4U, 0x38U, 0x13U, 0x34U, 0x0cU, 0xd9U,
  0xbfU, 0x74U, 0x94U, 0x8fU, 0xb7U, 0x9cU, 0xe5U, 0xdcU,
  0x9eU, 0x07U, 0x49U, 0x4fU, 0x98U, 0x2cU, 0xb0U, 0x93U,
  0x12U, 0xebU, 0xcdU, 0xb3U, 0x92U, 0xe7U, 0x41U, 0x60U,
  0xe3U, 0x21U, 0x27U, 0x3bU, 0xe6U, 0x19U, 0xd2U, 0x0eU,
  0x91U, 0x11U, 0xc7U, 0x3fU, 0x2aU, 0x8eU, 0xa1U, 0xbcU,
  0x2bU, 0xc8U, 0xc5U, 0x0fU, 0x5bU, 0xf3U, 0x87U, 0x8bU,
  0xfbU, 0xf5U, 0xdeU, 0x20U, 0xc6U, 0xa7U, 0x84U, 0xceU,
  0xd8U, 0x65U, 0x51U, 0xc9U, 0xa4U, 0xefU, 0x43U, 0x53U,
  0x25U, 0x5dU, 0x9bU, 0x31U, 0xe8U, 0x3eU, 0x0dU, 0xd7U,
  0x80U, 0xffU, 0x69U, 0x8aU, 0xbaU, 0x0bU, 0x73U, 0x5cU,
  0x6eU, 0x54U, 0x15U, 0x62U, 0xf6U, 0x35U, 0x30U, 0x52U,
  0xa3U, 0x16U, 0xd3U, 0x28U, 0x32U, 0xfaU, 0xaaU, 0x5eU,
  0xcfU, 0xeaU, 0xedU, 0x78U, 0x33U, 0x58U, 0x09U, 0x7bU,
  0x63U, 0xc0U, 0xc1U, 0x46U, 0x1eU, 0xdfU, 0xa9U, 0x99U,
  0x55U, 0x04U, 0xc4U, 0x86U, 0x39U, 0x77U, 0x82U, 0xecU,
  0x40U, 0x18U, 0x90U, 0x97U, 0x59U, 0xddU, 0x83U, 0x1fU,
  0x9aU, 0x37U, 0x06U, 0x24U, 0x64U, 0x7cU, 0xa5U, 0x56U,
  0x48U, 0x08U, 0x85U, 0xd0U, 0x61U, 0x26U, 0xcaU, 0x6fU,
  0x7eU, 0x6aU, 0xb6U, 0x71U, 0xa0U, 0x70U, 0x05U, 0xd1U,
  0x45U, 0x8cU, 0x23U, 0x1cU, 0xf0U, 0xeeU, 0x89U, 0xadU,
  0x7aU, 0x4bU, 0xc2U, 0x2fU, 0xdbU, 0x5aU, 0x4dU, 0x76U,
  0x67U, 0x17U, 0x2dU, 0xf4U, 0xcbU, 0xb1U, 0x4aU, 0xa8U,
  0xb5U, 0x22U, 0x47U, 0x3aU, 0xd5U, 0x10U, 0x4cU, 0x72U,
  0xccU, 0x00U, 0xf9U, 0xe0U, 0xfdU, 0xe2U, 0xfeU, 0xaeU,
  0xf8U, 0x5fU, 0xabU, 0xf1U, 0x1bU, 0x42U, 0x81U, 0xd6U,
  0xbeU, 0x44U, 0x29U, 0xa6U, 0x57U, 0xb9U, 0xafU, 0xf2U,
  0xd4U, 0x75U, 0x66U, 0xbbU, 0x68U, 0x9fU, 0x50U, 0x02U,
  0x01U, 0x3cU, 0x7fU, 0x8dU, 0x1aU, 0x88U, 0xbdU, 0xacU,
  0xf7U, 0xe4U, 0x79U, 0x96U, 0xa2U, 0xfcU, 0x6dU, 0xb2U,
  0x6bU, 0x03U, 0xe1U, 0x2eU, 0x7dU, 0x14U, 0x95U, 0x1dU
};

const unsigned char clefia_mul_table[256] = {
0, 2, 4, 6, 8, 10, 12, 14, 16, 
18, 20, 22, 24, 26, 28, 30, 32, 
34, 36, 38, 40, 42, 44, 46, 48, 
50, 52, 54, 56, 58, 60, 62, 64, 
66, 68, 70, 72, 74, 76, 78, 80, 
82, 84, 86, 88, 90, 92, 94, 96, 
98, 100, 102, 104, 106, 108, 110, 112, 
114, 116, 118, 120, 122, 124, 126, 128, 
130, 132, 134, 136, 138, 140, 142, 144, 
146, 148, 150, 152, 154, 156, 158, 160, 
162, 164, 166, 168, 170, 172, 174, 176, 
178, 180, 182, 184, 186, 188, 190, 192, 
194, 196, 198, 200, 202, 204, 206, 208, 
210, 212, 214, 216, 218, 220, 222, 224, 
226, 228, 230, 232, 234, 236, 238, 240, 
242, 244, 246, 248, 250, 252, 254, 29, 
31, 25, 27, 21, 23, 17, 19, 13, 
15, 9, 11, 5, 7, 1, 3, 61, 
63, 57, 59, 53, 55, 49, 51, 45, 
47, 41, 43, 37, 39, 33, 35, 93, 
95, 89, 91, 85, 87, 81, 83, 77, 
79, 73, 75, 69, 71, 65, 67, 125, 
127, 121, 123, 117, 119, 113, 115, 109, 
111, 105, 107, 101, 103, 97, 99, 157, 
159, 153, 155, 149, 151, 145, 147, 141, 
143, 137, 139, 133, 135, 129, 131, 189, 
191, 185, 187, 181, 183, 177, 179, 173, 
175, 169, 171, 165, 167, 161, 163, 221, 
223, 217, 219, 213, 215, 209, 211, 205, 
207, 201, 203, 197, 199, 193, 195, 253, 
255, 249, 251, 245, 247, 241, 243, 237, 
239, 233, 235, 229, 231, 225, 227
};

unsigned char ClefiaMul2(unsigned char x)
{
  /* multiplication over GF(2^8) (p(x) = '11d') */
  if(x & 0x80U){
    x ^= 0x0eU;
  }
  return ((x << 1) | (x >> 7));
}

inline void ByteXor(unsigned char *dst, const unsigned char *a, const unsigned char *b, int bytelen)
{
  switch(bytelen){
    case 4:
    *(uint32_t*)&dst[0] = *(uint32_t*)&a[0]^ *(uint32_t*)&b[0];
    break;
    case 8:
    *(uint32_t*)&dst[0] = *(uint32_t*)&a[0]^ *(uint32_t*)&b[0];
    *(uint32_t*)&dst[4] = *(uint32_t*)&a[4]^ *(uint32_t*)&b[4];


    break;
    case 12:
    *(uint32_t*)&dst[0] = *(uint32_t*)&a[0]^ *(uint32_t*)&b[0];
    *(uint32_t*)&dst[4] = *(uint32_t*)&a[4]^ *(uint32_t*)&b[4];
    *(uint32_t*)&dst[8] = *(uint32_t*)&a[8]^ *(uint32_t*)&b[8];

    break;
    case 16:
    *(uint32_t*)&dst[0] = *(uint32_t*)&a[0]^ *(uint32_t*)&b[0];
    *(uint32_t*)&dst[4] = *(uint32_t*)&a[4]^ *(uint32_t*)&b[4];
    *(uint32_t*)&dst[8] = *(uint32_t*)&a[8]^ *(uint32_t*)&b[8];
    *(uint32_t*)&dst[12] = *(uint32_t*)&a[12]^ *(uint32_t*)&b[12];

    break;
  }
  // while(bytelen-- > 0){
  //   *dst++ = *a++ ^ *b++;
  // }
}


#define ClefiaMul2(_x) (clefia_mul_table[_x])
#define ClefiaMul4(_x) (ClefiaMul2(ClefiaMul2((_x))))
#define ClefiaMul6(_x) (ClefiaMul2((_x)) ^ ClefiaMul4((_x)))
#define ClefiaMul8(_x) (ClefiaMul2(ClefiaMul4((_x))))
#define ClefiaMulA(_x) (ClefiaMul2((_x)) ^ ClefiaMul8((_x)))

void inline ClefiaF0Xor(unsigned char *dst, const unsigned char *src, const unsigned char *rk)
{
  unsigned char x[4], y[4], z[4];

  /* F0 */
  /* Key addition */
  ByteXor(x, src, rk, 4);
  /* Substitution layer */
  z[0] = clefia_s0[x[0]];
  z[1] = clefia_s1[x[1]]; 
  z[2] = clefia_s0[x[2]];
  z[3] = clefia_s1[x[3]];


  /* Diffusion layer (M0) */
  y[0] =            z[0]  ^ ClefiaMul2(z[1]) ^ ClefiaMul4(z[2]) ^ ClefiaMul6(z[3]);
  y[1] = ClefiaMul2(z[0]) ^            z[1]  ^ ClefiaMul6(z[2]) ^ ClefiaMul4(z[3]);
  y[2] = ClefiaMul4(z[0]) ^ ClefiaMul6(z[1]) ^            z[2]  ^ ClefiaMul2(z[3]);
  y[3] = ClefiaMul6(z[0]) ^ ClefiaMul4(z[1]) ^ ClefiaMul2(z[2]) ^            z[3] ;

  /* Xoring after F0 */
  ByteCpy(dst + 0, src + 0, 4);
  ByteXor(dst + 4, src + 4, y, 4);
}

void inline ClefiaF1Xor(unsigned char *dst, const unsigned char *src, const unsigned char *rk)
{
  unsigned char x[4], y[4], z[4];

  /* F1 */
  /* Key addition */
  ByteXor(x, src, rk, 4);
  /* Substitution layer */
  z[0] = clefia_s1[x[0]];
  z[1] = clefia_s0[x[1]];
  z[2] = clefia_s1[x[2]];
  z[3] = clefia_s0[x[3]];
  /* Diffusion layer (M1) */
  y[0] =            z[0]  ^ ClefiaMul8(z[1]) ^ ClefiaMul2(z[2]) ^ ClefiaMulA(z[3]);
  y[1] = ClefiaMul8(z[0]) ^            z[1]  ^ ClefiaMulA(z[2]) ^ ClefiaMul2(z[3]);
  y[2] = ClefiaMul2(z[0]) ^ ClefiaMulA(z[1]) ^            z[2]  ^ ClefiaMul8(z[3]);
  y[3] = ClefiaMulA(z[0]) ^ ClefiaMul2(z[1]) ^ ClefiaMul8(z[2]) ^            z[3] ;

  /* Xoring after F1 */
  ByteCpy(dst + 0, src + 0, 4);
  ByteXor(dst + 4, src + 4, y, 4);
}

void inline ClefiaGfn4(unsigned char *y, const unsigned char *x, const unsigned char *rk, int r)
{
  unsigned char fin[16], fout[16];

  ByteCpy(fin, x, 16);
  while(r-- > 0){
    ClefiaF0Xor(fout + 0, fin + 0, rk + 0);
    ClefiaF1Xor(fout + 8, fin + 8, rk + 4);
    rk += 8;
    if(r){ /* swapping for encryption */
      // ByteCpy(fin + 0,  fout + 4, 12);
      memmove(fin+0,fout+4,12);
      // ByteCpy(fin + 12, fout + 0, 4);
      memmove(fin+12,fout+0,4);
    }
  }
  ByteCpy(y, fout, 16);
}

void inline ClefiaGfn8(unsigned char *y, const unsigned char *x, const unsigned char *rk, int r)
{
  unsigned char fin[32], fout[32];

  ByteCpy(fin, x, 32);
  while(r-- > 0){
    ClefiaF0Xor(fout + 0,  fin + 0,  rk + 0);
    ClefiaF1Xor(fout + 8,  fin + 8,  rk + 4);
    ClefiaF0Xor(fout + 16, fin + 16, rk + 8);
    ClefiaF1Xor(fout + 24, fin + 24, rk + 12);
    rk += 16;
    if(r){ /* swapping for encryption */
      // ByteCpy(fin + 0,  fout + 4, 28);
      memmove(fin + 0,  fout + 4, 28);
      // ByteCpy(fin + 28, fout + 0, 4);
      memmove(fin + 28, fout + 0, 4);
    }
  }
  ByteCpy(y, fout, 32);
}

void ClefiaGfn4Inv(unsigned char *y, const unsigned char *x, const unsigned char *rk, int r)
{
  unsigned char fin[16], fout[16];

  rk += (r - 1) * 8;
  ByteCpy(fin, x, 16);
  while(r-- > 0){
    ClefiaF0Xor(fout + 0, fin + 0, rk + 0);
    ClefiaF1Xor(fout + 8, fin + 8, rk + 4);
    rk -= 8;
    if(r){ /* swapping for decryption */
      ByteCpy(fin + 0, fout + 12, 4);
      ByteCpy(fin + 4, fout + 0,  12);
    }
  }
  ByteCpy(y, fout, 16);
}

void ClefiaDoubleSwap(unsigned char *lk)
{
  unsigned char t[16];

  t[0]  = (lk[0] << 7) | (lk[1]  >> 1);
  t[1]  = (lk[1] << 7) | (lk[2]  >> 1);
  t[2]  = (lk[2] << 7) | (lk[3]  >> 1);
  t[3]  = (lk[3] << 7) | (lk[4]  >> 1);
  t[4]  = (lk[4] << 7) | (lk[5]  >> 1);
  t[5]  = (lk[5] << 7) | (lk[6]  >> 1);
  t[6]  = (lk[6] << 7) | (lk[7]  >> 1);
  t[7]  = (lk[7] << 7) | (lk[15] & 0x7fU);

  t[8]  = (lk[8]  >> 7) | (lk[0]  & 0xfeU);
  t[9]  = (lk[9]  >> 7) | (lk[8]  << 1);
  t[10] = (lk[10] >> 7) | (lk[9]  << 1);
  t[11] = (lk[11] >> 7) | (lk[10] << 1);
  t[12] = (lk[12] >> 7) | (lk[11] << 1);
  t[13] = (lk[13] >> 7) | (lk[12] << 1);
  t[14] = (lk[14] >> 7) | (lk[13] << 1);
  t[15] = (lk[15] >> 7) | (lk[14] << 1);

  ByteCpy(lk, t, 16);
}

void ClefiaConSet(unsigned char *con, const unsigned char *iv, int lk)
{
  unsigned char t[2];
  unsigned char tmp;

  ByteCpy(t, iv, 2);
  while(lk-- > 0){
    con[0] = t[0] ^ 0xb7U; /* P_16 = 0xb7e1 (natural logarithm) */
    con[1] = t[1] ^ 0xe1U;
    con[2] = ~((t[0] << 1) | (t[1] >> 7));
    con[3] = ~((t[1] << 1) | (t[0] >> 7));
    con[4] = ~t[0] ^ 0x24U; /* Q_16 = 0x243f (circle ratio) */
    con[5] = ~t[1] ^ 0x3fU;
    con[6] = t[1];
    con[7] = t[0];
    con += 8;

    /* updating T */
    if(t[1] & 0x01U){
      t[0] ^= 0xa8U;
      t[1] ^= 0x30U;
    }
    tmp = t[0] << 7;
    t[0] = (t[0] >> 1) | (t[1] << 7);
    t[1] = (t[1] >> 1) | tmp;
  }    
}

void ClefiaKeySet128(unsigned char *rk, const unsigned char *skey)
{
  const unsigned char iv[2] = {0x42U, 0x8aU}; /* cubic root of 2 */
  unsigned char lk[16];
  unsigned char con128[4 * 60];
  int i;

  /* generating CONi^(128) (0 <= i < 60, lk = 30) */
  ClefiaConSet(con128, iv, 30);
  /* GFN_{4,12} (generating L from K) */
  ClefiaGfn4(lk, skey, con128, 12);

  ByteCpy(rk, skey, 8); /* initial whitening key (WK0, WK1) */
  rk += 8;
  for(i = 0; i < 9; i++){ /* round key (RKi (0 <= i < 36)) */
    ByteXor(rk, lk, con128 + i * 16 + (4 * 24), 16);
    if(i % 2){
      ByteXor(rk, rk, skey, 16); /* Xoring K */
    }
    ClefiaDoubleSwap(lk); /* Updating L (DoubleSwap function) */
    rk += 16;
  }
  ByteCpy(rk, skey + 8, 8); /* final whitening key (WK2, WK3) */
}

void ClefiaKeySet192(unsigned char *rk, const unsigned char *skey)
{
  const unsigned char iv[2] = {0x71U, 0x37U}; /* cubic root of 3 */
  unsigned char skey256[32];
  unsigned char lk[32];
  unsigned char con192[4 * 84];
  int i;

  ByteCpy(skey256, skey, 24);
  for(i = 0; i < 8; i++){
    skey256[i + 24] = ~skey[i];
  }

  /* generating CONi^(192) (0 <= i < 84, lk = 42) */
  ClefiaConSet(con192, iv, 42);
  /* GFN_{8,10} (generating L from K) */
  ClefiaGfn8(lk, skey256, con192, 10);

  ByteXor(rk, skey256, skey256 + 16, 8); /* initial whitening key (WK0, WK1) */
  rk += 8;
  for(i = 0; i < 11; i++){ /* round key (RKi (0 <= i < 44)) */
    if((i / 2) % 2){
      ByteXor(rk, lk + 16, con192 + i * 16 + (4 * 40), 16); /* LR */
      if(i % 2){
        ByteXor(rk, rk, skey256 + 0,  16); /* Xoring KL */
      }
      ClefiaDoubleSwap(lk + 16); /* updating LR */
    }else{
      ByteXor(rk, lk + 0,  con192 + i * 16 + (4 * 40), 16); /* LL */
      if(i % 2){
        ByteXor(rk, rk, skey256 + 16, 16); /* Xoring KR */
      }
      ClefiaDoubleSwap(lk + 0);  /* updating LL */
    }
    rk += 16;
  }
  ByteXor(rk, skey256 + 8, skey256 + 24, 8); /* final whitening key (WK2, WK3) */
}

void ClefiaKeySet256(unsigned char *rk, const unsigned char *skey)
{
  const unsigned char iv[2] = {0xb5, 0xc0U}; /* cubic root of 5 */
  unsigned char lk[32];
  unsigned char con256[4 * 92];
  int i;

  /* generating CONi^(256) (0 <= i < 92, lk = 46) */
  ClefiaConSet(con256, iv, 46);
  /* GFN_{8,10} (generating L from K) */
  ClefiaGfn8(lk, skey, con256, 10);

  ByteXor(rk, skey, skey + 16, 8); /* initial whitening key (WK0, WK1) */
  rk += 8;
  for(i = 0; i < 13; i++){ /* round key (RKi (0 <= i < 52)) */
    if((i / 2) % 2){
      ByteXor(rk, lk + 16, con256 + i * 16 + (4 * 40), 16); /* LR */
      if(i % 2){
        ByteXor(rk, rk, skey + 0,  16); /* Xoring KL */
      }
      ClefiaDoubleSwap(lk + 16); /* updating LR */
    }else{
      ByteXor(rk, lk + 0,  con256 + i * 16 + (4 * 40), 16); /* LL */
      if(i % 2){
        ByteXor(rk, rk, skey + 16, 16); /* Xoring KR */
      }
      ClefiaDoubleSwap(lk + 0);  /* updating LL */
    }
    rk += 16;
  }
  ByteXor(rk, skey + 8, skey + 24, 8); /* final whitening key (WK2, WK3) */
}


int ClefiaKeySet(unsigned char *rk, const unsigned char *skey, const int key_bitlen)
{
  if(128 == key_bitlen){
    ClefiaKeySet128(rk, skey);
    return 18;
  }else if(192 == key_bitlen){
    ClefiaKeySet192(rk, skey);
    return 22;
  }else if(256 == key_bitlen){
    ClefiaKeySet256(rk, skey);
    return 26;
  }

  return 0; /* invalid key_bitlen */
}

void ClefiaEncrypt(unsigned char *ct, const unsigned char *pt, const unsigned char *rk, const int r)
{
  unsigned char rin[16], rout[16];

  ByteCpy(rin,  pt,  16);
  ByteXor(rin + 4,  rin + 4,  rk + 0, 4); /* initial key whitening */
  ByteXor(rin + 12, rin + 12, rk + 4, 4);
  rk += 8;

  ClefiaGfn4(rout, rin, rk, r); /* GFN_{4,r} */

  ByteCpy(ct, rout, 16);
  ByteXor(ct + 4,  ct + 4,  rk + r * 8 + 0, 4); /* final key whitening */
  ByteXor(ct + 12, ct + 12, rk + r * 8 + 4, 4);
}

void ClefiaDecrypt(unsigned char *pt, const unsigned char *ct, const unsigned char *rk, const int r)
{
  unsigned char rin[16], rout[16];

  ByteCpy(rin, ct, 16);

  ByteXor(rin + 4,  rin + 4,  rk + r * 8 + 8,  4); /* initial key whitening */
  ByteXor(rin + 12, rin + 12, rk + r * 8 + 12, 4);
  rk += 8;

  ClefiaGfn4Inv(rout, rin, rk, r); /* GFN^{-1}_{4,r} */

  ByteCpy(pt, rout, 16);
  ByteXor(pt + 4,  pt + 4,  rk - 8, 4); /* final key whitening */
  ByteXor(pt + 12, pt + 12, rk - 4, 4);
}



/* Test */

#ifdef _CLEFIA_TEST

#include <stdio.h>

void BytePut(const unsigned char *data, int bytelen)
{
  while(bytelen-- > 0){
    printf("%02x", *data++);
  }
  printf("\n");
}

int main(void)
{
  const unsigned char skey[32] = {
    0xffU,0xeeU,0xddU,0xccU,0xbbU,0xaaU,0x99U,0x88U,
    0x77U,0x66U,0x55U,0x44U,0x33U,0x22U,0x11U,0x00U,
    0xf0U,0xe0U,0xd0U,0xc0U,0xb0U,0xa0U,0x90U,0x80U,
    0x70U,0x60U,0x50U,0x40U,0x30U,0x20U,0x10U,0x00U
  };
  const unsigned char pt[16] = {
    0x00U,0x01U,0x02U,0x03U,0x04U,0x05U,0x06U,0x07U,
    0x08U,0x09U,0x0aU,0x0bU,0x0cU,0x0dU,0x0eU,0x0fU
  };
  unsigned char ct[16];
  unsigned char dst[16];
  unsigned char rk[8 * 26 + 16]; /* 8 bytes x 26 rounds(max) + whitening keys */
  int r;

  printf("--- Test ---\n");
  printf("plaintext:  "); BytePut(pt, 16);
  printf("secretkey:  "); BytePut(skey, 32);

  /* for 128-bit key */
  printf("--- CLEFIA-128 ---\n");
  /* encryption */
  r = ClefiaKeySet(rk, skey, 128);
  ClefiaEncrypt(dst, pt, rk, r);
  printf("ciphertext: "); BytePut(dst, 16);
  /* decryption */
  ByteCpy(ct, dst, 16);
  r = ClefiaKeySet(rk, skey, 128);
  ClefiaDecrypt(dst, ct, rk, r);
  printf("plaintext : "); BytePut(dst, 16);

  /* for 192-bit key */
  printf("--- CLEFIA-192 ---\n");
  /* encryption */
  r = ClefiaKeySet(rk, skey, 192);
  ClefiaEncrypt(dst, pt, rk, r);
  printf("ciphertext: "); BytePut(dst, 16);
  /* decryption */
  ByteCpy(ct, dst, 16);
  r = ClefiaKeySet(rk, skey, 192);
  ClefiaDecrypt(dst, ct, rk, r);
  printf("plaintext : "); BytePut(dst, 16);

  /* for 256-bit key */
  printf("--- CLEFIA-256 ---\n");
  /* encryption */
  r = ClefiaKeySet(rk, skey, 256);
  ClefiaEncrypt(dst, pt, rk, r);
  printf("ciphertext: "); BytePut(dst, 16);
  /* decryption */
  ByteCpy(ct, dst, 16);
  r = ClefiaKeySet(rk, skey, 256);
  ClefiaDecrypt(dst, ct, rk, r);
  printf("plaintext : "); BytePut(dst, 16);

  return 0;
}

#endif /* _CLEFIA_TEST */



/* end of file */

