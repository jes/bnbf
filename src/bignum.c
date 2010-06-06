/* bignum stuff for bnbf

This is not a general-purpose bignum library and only contains what is
necessary for bnbf

See "COPYING" for license details

James Stanley 2010 */

#include "bnbf.h"

/* Allocates, and returns a pointer to, a new bignum */
Bignum *new_bignum(void) {
  Bignum *b;

  b = malloc(sizeof(Bignum));
  b->byte = malloc(sizeof(char));
  *(b->byte) = 0;
  b->num_bytes = 1;

  return b;
}

/* Double the number of bytes allocated for the given bignum */
static void grow_bignum(Bignum *b) {
  b->byte = realloc(b->byte, b->num_bytes * 2);
  memset(b->byte + b->num_bytes, 0x00, b->num_bytes);
  b->num_bytes *= 2;
}

/* Adds an integer amount to the given bignum */
void add_bignum(Bignum *b, long n) {
  long d = 0;
  int neg = 0;

  while(n) {
    n += b->byte[d];

    b->byte[d++] = n & 0xff;
    n /= 0x100;

    if(d == b->num_bytes) grow_bignum(b);
  }
}

/* Prints the given bignum to the given stream
   WARNING: not thread-safe for negative values of b! */
void print_bignum(Bignum *b, FILE *fp) {
  const char charac[] = "0123456789abcdef";
  int d;

  /* print a minus sign if negative */
  if(b->sign == -1) fputc('-', stdout);

  /* find the most significant byte that is non-zero */
  for(d = (b->num_bytes - 1); (b->byte[d] == 0) && (d >= 0); d--);

  /* TODO: base 10 */
  if(d < 0) {
    fputs("0x00", fp);
  } else {
    fputs("0x", fp);
    for(; d >= 0; d--) {
      fputc(charac[(b->byte[d] >> 4) & 0xf], fp);
      fputc(charac[b->byte[d] & 0xf], fp);
    }
  }
}