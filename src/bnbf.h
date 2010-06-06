/* Header for bnbf

See "COPYING" for license details

James Stanley 2010 */

#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <gmp.h>

#include "config.h"

/* options.c */
extern int benchmark;
extern int chario;
extern char *eof_value;
extern int maxmem;
extern int noneg;
extern int wrap;

void parse_options(int argc, char **argv);

/* program.c */
typedef struct Inst {
  char type;/* type of instruction */
  struct Inst *loop;/* address of corresponding loop entry/exit */
  struct Inst *next;/* next instruction to execute */
} Inst;

extern const char *program_name;
extern int stop_program;

void run_program(const char *name);
void free_program(Inst *prog);

/* memory.c */
typedef struct Memory {
  int mp;/* memory pointer */
  mpz_t *pos_mem;/* positive-address memory */
  mpz_t *neg_mem;/* negative-address memory */
  int pos_len;/* length of pos_mem */
  int neg_len;/* length of neg_mem */
} Memory;

Memory *make_memory(void);
void add(Memory *mem, int amt);
void input(Memory *mem);
void output(Memory *mem);
int is_zero(Memory *mem);
void free_memory(Memory *mem);

/* bignum.c */
typedef struct Bignum {
  unsigned char *byte;
  char sign;
  int num_bytes;
} Bignum;

Bignum *new_bignum(void);
void add_bignum(Bignum *b, long n);
void print_bignum(Bignum *b, FILE *fp);
