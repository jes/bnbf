/* Header for bnbf */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <argp.h>
#include <gmp.h>

#include "config.h"

/* options.c */
extern struct argp argp;
extern int chario;
extern int wrap;
extern char **file;
extern int num_files;

/* program.c */
typedef struct Inst {
  char type;/* type of instruction */
  struct Inst *loop;/* address of corresponding loop entry/exit */
  struct Inst *next;/* next instruction to execute */
} Inst;

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
