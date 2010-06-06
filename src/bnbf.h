/* Header for bnbf

See "COPYING" for license details

James Stanley 2010 */

#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>

#include "config.h"
#include "bigint.h"

/* options.c */
extern int benchmark;
extern int chario;
extern char *eof_value;
extern int maxmem;
extern int noneg;
extern int prompt;
extern int wrap;

void parse_options(int argc, char **argv);

/* program.c */
#define ADD    0
#define SUB    1
#define LEFT   2
#define RIGHT  3
#define SLOOP  4
#define ELOOP  5
#define INPUT  6
#define OUTPUT 7

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
typedef struct Memory {
  int mp;/* memory pointer */
  bigint *pos_mem;/* positive-address memory */
  bigint *neg_mem;/* negative-address memory */
  int pos_len;/* length of pos_mem */
  int neg_len;/* length of neg_mem */
} Memory;

extern bigint zero, ff;

Memory *make_memory(void);
void add(Memory *mem, int amt);
void input(Memory *mem);
void output(Memory *mem);
int is_zero(Memory *mem);
void free_memory(Memory *mem);
