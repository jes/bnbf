/* Memory handling for bnbf

   James Stanley 2010 */

#include "bnbf.h"

/* Allocates some memory for a brainfuck program */
Memory *make_memory(void) {
  int i;
  Memory *mem;

  mem = malloc(sizeof(Memory));
  mem->mp = 0;

  mem->pos_mem = malloc(INIT_MEM_SIZE * sizeof(mpz_t));
  mem->pos_len = INIT_MEM_SIZE;
  for(i = 0; i < mem->pos_len; i++) mpz_init(mem->pos_mem[i]);

  mem->neg_mem = malloc(INIT_MEM_SIZE * sizeof(mpz_t));
  mem->neg_len = INIT_MEM_SIZE;
  for(i = 0; i < mem->neg_len; i++) mpz_init(mem->neg_mem[i]);

  return mem;
}

/* Free the memory for a program */
void free_memory(Memory *mem) {
  free(mem->pos_mem);
  free(mem->neg_mem);
  free(mem);
}

/* Returns a pointer to the current cell of memory in mem. Sorts out the
   growing of memory where necessary */
static mpz_t *get_cell(Memory *memory) {
  int i;
  int *len;
  mpz_t **mem;
  int mp;

  /* positive or negative address? */
  if(memory->mp >= 0) {
    len = &(memory->pos_len);
    mem = &(memory->pos_mem);
    mp = memory->mp;
  } else {
    len = &(memory->neg_len);
    mem = &(memory->neg_mem);
    mp = -memory->mp;
  }

  /* check that mp is within range */
  if(maxmem && mp > maxmem) {
    fprintf(stderr, "%s: memory overflow, perhaps you need to adjust the "
            "--max-mem option or fix a memory leak in your program.\n",
            program_name);
    stop_program = 1;
    return NULL;
  }

  /* double the size of memory until we are within range */
  while(mp >= *len) {
    *len *= 2;

    *mem = realloc(*mem, *len * sizeof(mpz_t));

    for(i = *len / 2; i < *len; i++) mpz_init((*mem)[i]);
  }

  return *mem + mp;
}

/* Adds the given amount to the current cell of memory */
void add(Memory *mem, int amt) {
  mpz_t *cell;

  if(!(cell = get_cell(mem))) return;

  /* TODO: Handle wrapping if enabled */

  if(amt > 0) mpz_add_ui(*cell, *cell, amt);
  else mpz_sub_ui(*cell, *cell, -amt);
}

/* Read input to the current cell */
void input(Memory *mem) {
  mpz_t *cell;
  size_t n;

  cell = get_cell(mem);

  if(chario) {
    /* character io */
    mpz_set_ui(*cell, fgetc(stdin));
  } else {
    /* number io */
    do {
      printf("Input: ");
      n = mpz_inp_str(*cell, stdin, 0);
    } while(n == 0);

    /* TODO: sort out errors (*/
    while(!mpz_inp_str(*cell, stdin, 0));
  }
}

/* Write output from the current cell */
void output(Memory *mem) {
  mpz_t *cell;

  cell = get_cell(mem);

  if(chario) {
    /* character io */
    fputc(mpz_get_ui(*cell), stdout);
  } else {
    /* number io */
    mpz_out_str(stdout, 10, *cell);
    putchar('\n');
  }
}

/* Return 1 if the current cell is 0 and 0 otherwise */
int is_zero(Memory *mem) {
  mpz_t *cell;

  cell = get_cell(mem);

  return mpz_cmp_ui(*cell, 0) == 0;
}
