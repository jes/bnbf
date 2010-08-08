/* Header for bnbf

   See "COPYING" for license details

   James Stanley 2010 */

#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
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
#define ADD    '+'
#define SUB    '-'
#define LEFT   '<'
#define RIGHT  '>'
#define SLOOP  '['
#define ELOOP  ']'
#define INPUT  ','
#define OUTPUT '.'
#define KILL   '!'

typedef struct Inst {
  char type;/* type of instruction */
  union {
    size_t loop;/* index of corresponding loop entry/exit */
    int amount;/* number of this instruction to carry out */
  } u;
} Inst;

extern const char *program_name;
extern int stop_program;

void run_program(const char *name);

/* memory.c */
typedef struct Memory {
  int mp;/* memory pointer */
  void *pos_mem;/* positive-address memory */
  void *neg_mem;/* negative-address memory */
  int pos_len;/* length of pos_mem */
  int neg_len;/* length of neg_mem */
} Memory;

Memory *make_memory(void);
void add(Memory *mem, int amt);
void input(Memory *mem);
void output(Memory *mem);
int is_zero(Memory *mem);
void free_memory(Memory *mem);
