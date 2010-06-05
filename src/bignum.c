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

  if(b->byte[b->num_bytes - 1] & 0x80) {
    /* sign extension */
    memset(b->byte + b->num_bytes, 0xff, b->num_bytes);
  } else {
    memset(b->byte + b->num_bytes, 0x00, b->num_bytes);
  }

  b->num_bytes *= 2;
}

/* Add two positive numbers */
static void add_bignum_pp(Bignum *b, long n) {
  long d, byte = 0, factor = 0;

  for(d = 0; d < b->num_bytes; d++) {
    byte = b->byte[d] + (n & 0xff) + byte;
    b->byte[d] = byte & 0xff;
    n /= 0x100;
    byte /= 0x100;
    factor++;
  }

  if(n || byte) {/* overflow, make more bytes */
    grow_bignum(b);
    add_bignum(b, (n + byte) << (factor * 8));
  }
}

/* Add two negative numbers */
static void add_bignum_nn(Bignum *b, long n) {
  long d, byte = 0, factor = 0;

  for(d = 0; d < b->num_bytes; d++) {
    byte = b->byte[d] + (n & 0xff) + byte;
    b->byte[d] = byte & 0xff;
    n /= 0x100;
    byte /= 0x100;
    factor++;
  }

  if(n || byte) {/* overflow, make more bytes */
    grow_bignum(b);
    add_bignum(b, (n + byte) << (factor * 8));
  }
}

/* Add a negative long to a positive bignum */
static void add_bignum_pn(Bignum *b, long n) {
  long d, byte = 0, factor = 0;

  for(d = 0; d < b->num_bytes; d++) {
    byte = b->byte[d] + (n & 0xff) + byte;
    b->byte[d] = byte & 0xff;
    n /= 0x100;
    byte /= 0x100;
    factor++;
  }

  if(n || byte) {/* overflow, make more bytes */
    grow_bignum(b);
    add_bignum(b, (n + byte) << (factor * 8));
  }
}

/* Add a positive long to a negative bignum */
static void add_bignum_np(Bignum *b, long n) {
  long d, byte = 0, factor = 0;

  for(d = 0; d < b->num_bytes; d++) {
    byte = b->byte[d] + (n & 0xff) + byte;
    b->byte[d] = byte & 0xff;
    n /= 0x100;
    byte /= 0x100;
    factor++;
  }

  if(n || byte) {/* overflow, make more bytes */
    grow_bignum(b);
    add_bignum(b, (n + byte) << (factor * 8));
  }
}

/* Adds an integer amount to the given bignum */
void add_bignum(Bignum *b, long n) {
  /* decide which function to use */
  if(n < 0) {/* n -ve */
    if(b->byte[b->num_bytes - 1] & 0x80) add_bignum_nn(b, n);
    else add_bignum_pn(b, n);
  } else {/* n +ve */
    if(b->byte[b->num_bytes - 1] & 0x80) add_bignum_np(b, n);
    else add_bignum_pp(b, n);
  }
}

/* Prints the given bignum to the given stream
WARNING: not thread-safe for negative values of b! */
void print_bignum(Bignum *b, FILE *fp) {
  const char charac[] = "0123456789abcdef";
  int d;
  int neg = 0;

  /* flip bits and add one if negative to convert to positive */
  if(b->byte[b->num_bytes - 1] & 0x80) {
    neg = 1;

    for(d = 0; d < b->num_bytes; d++) b->byte[d] = ~b->byte[d];
    add_bignum(b, 1);

    fputc('-', fp);
  }

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

  /* take one and flip bits to convert back to negative */
  if(neg) {
    add_bignum(b, -1);
    for(d = 0; d < b->num_bytes; d++) b->byte[d] = ~b->byte[d];
  }
}
