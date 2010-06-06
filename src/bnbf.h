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
extern struct argp argp;
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
  union {
    struct Inst *loop;/* address of corresponding loop entry/exit */
    long amount;/* number of this instruction to carry out */
  } u;
  struct Inst *next;/* next instruction to execute */
} Inst;

extern const char *program_name;
extern int stop_program;

void run_program(const char *name);
void free_program(Inst *prog);

/* memory.c */
#define POS 0
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
