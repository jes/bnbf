/* Memory handling for bnbf

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

bigint zero, ff;

/* Allocates some memory for a brainfuck program */
Memory *make_memory(void) {
  int i;
  Memory *mem;

  mem = malloc(sizeof(Memory));
  mem->mp = 0;

  mem->pos_mem = malloc(INIT_MEM_SIZE * sizeof(bigint));
  mem->pos_len = INIT_MEM_SIZE;
  for(i = 0; i < mem->pos_len; i++) bigint_init(mem->pos_mem + i);

  mem->neg_mem = malloc(INIT_MEM_SIZE * sizeof(bigint));
  mem->neg_len = INIT_MEM_SIZE;
  for(i = 0; i < mem->neg_len; i++) bigint_init(mem->neg_mem + i);

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
static bigint *get_cell(Memory *memory) {
  int i;
  int *len;
  bigint **mem;
  int mp;

  /* positive or negative address? */
  if(memory->mp >= 0) {
    len = &(memory->pos_len);
    mem = &(memory->pos_mem);
    mp = memory->mp;
  } else {
    if(noneg) {
      fprintf(stderr, "%s: memory overflow, perhaps you need to stop using "
              "the --no-negative option or fix a memory leak in your "
              "program.\n",
              program_name);
      stop_program = 1;
      return NULL;
    }

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

    *mem = realloc(*mem, *len * sizeof(bigint));

    for(i = *len / 2; i < *len; i++) bigint_init(*mem + i);
  }

  return *mem + mp;
}

/* Adds the given amount to the current cell of memory */
void add(Memory *mem, int amt) {
  bigint *cell;

  if(!(cell = get_cell(mem))) return;

  bigint_add_by_int(cell, amt);

  /* pretend we have 8-bit cells */
  if(wrap) {
    while(bigint_compare(cell, &zero) < 0)
      bigint_add_by_int(cell, 0x100);

    while(bigint_compare(cell, &ff) > 0)
      bigint_sub_by_int(cell, 0x100);
  }
}

/* Read input to the current cell */
void input(Memory *mem) {
  bigint *cell;
  int c;
  char buf[1024];
  char *p;

  if(!(cell = get_cell(mem))) return;

  if(chario) {
    /* character io */
    fflush(stdout);
    c = fgetc(stdin);

    if(c == EOF) goto handle_eof;
    else bigint_from_int(cell, c);
  } else {
    /* number io */
    do {
      if(prompt) {
        fprintf(stderr, "Input: ");
        fflush(stderr);
      }

      if(!(fgets(buf, 1024, stdin))) goto handle_eof;
      if((p = strchr(buf, '\n'))) *p = '\0';
    } while(bigint_from_string(cell, buf) == -BIGINT_ILLEGAL_PARAM);
  }

  return;

 handle_eof:
  if(strcasecmp(eof_value, "nochange") != 0) {
    bigint_from_string(cell, eof_value);
  }
}

/* Write output from the current cell */
void output(Memory *mem) {
  bigint *cell;
  int c;
  char *buf;

  if(!(cell = get_cell(mem))) return;

  if(chario) {
    /* character io */
    bigint_to_int(cell, &c);
    fputc(c, stdout);
  } else {
    /* number io */
    buf = malloc(bigint_string_length(cell));

    bigint_to_string(cell, buf);
    fputs(buf, stdout); fputc('\n', stdout);

    free(buf);
  }
}

/* Return 1 if the current cell is 0 and 0 otherwise */
int is_zero(Memory *mem) {
  bigint *cell;

  /* memory leak, return 0 even though the program is about to die */
  if(!(cell = get_cell(mem))) return 0;

  return bigint_is_zero(cell);
}
