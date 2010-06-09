/* Memory handling for bnbf

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

/* Allocates some memory for a brainfuck program */
Memory *make_memory(void) {
  int i;
  Memory *mem;

  mem = malloc(sizeof(Memory));
  mem->mp = 0;

  if(wrap) {
    /* memory cells are unsigned char */
    mem->pos_mem = malloc(INIT_MEM_SIZE);
    mem->pos_len = INIT_MEM_SIZE;
    memset(mem->pos_mem, 0, INIT_MEM_SIZE);

    mem->neg_mem = malloc(INIT_MEM_SIZE);
    mem->neg_len = INIT_MEM_SIZE;
    memset(mem->neg_mem, 0, INIT_MEM_SIZE);
  } else {
    /* memory cells are bigint */
    mem->pos_mem = malloc(INIT_MEM_SIZE * sizeof(bigint));
    mem->pos_len = INIT_MEM_SIZE;

    /* we have to cast to "bigint *" here for the pointer arithmetic to work */
    for(i = 0; i < mem->pos_len; i++) bigint_init((bigint *)mem->pos_mem + i);

    mem->neg_mem = malloc(INIT_MEM_SIZE * sizeof(bigint));
    mem->neg_len = INIT_MEM_SIZE;
    for(i = 0; i < mem->neg_len; i++) bigint_init((bigint *)mem->neg_mem + i);
  }

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
static inline void *get_cell(Memory *memory) {
  int i;
  int *len;
  void **mem;
  int mp;

  /* positive or negative address? */
  if(memory->mp >= 0) {
    len = &(memory->pos_len);
    mp = memory->mp;

    mem = &(memory->pos_mem);
  } else {
    if(noneg) {
      fprintf(stderr, "%s: negative memory, perhaps you need to stop using "
              "the --no-negative option or fix a memory leak in your "
              "program.\n",
              program_name);
      stop_program = 1;
      return NULL;
    }

    len = &(memory->neg_len);
    mp = -memory->mp;

    mem = &(memory->neg_mem);
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
    if(wrap) {
      *mem = realloc(*mem, *len * 2);
      memset(*mem + *len, 0, *len);
    } else {
      *mem = realloc(*mem, *len * 2 * sizeof(bigint));

      /* the cast is to make the pointer arithmetic happy */
      for(i = 0; i < *len; i++) bigint_init((bigint *)*mem + *len + i);
    }

    *len *= 2;
  }

  if(wrap) return *mem + mp;
  else return *(bigint **)mem + mp;
}

/* Adds the given amount to the current cell of memory */
void add(Memory *mem, int amt) {
  void *cell;

  if(!(cell = get_cell(mem))) return;

  if(wrap) *(unsigned char *)cell += amt;
  else bigint_add_by_int(cell, amt);
}

/* Read input to the current cell */
void input(Memory *mem) {
  void *cell;
  bigint input;
  int c;
  char buf[1024];
  char *p;

  if(!(cell = get_cell(mem))) return;

  if(chario) {
    /* character io */
    fflush(stdout);
    c = fgetc(stdin);

    if(c == EOF) goto handle_eof;
    else {
      if(wrap) *(unsigned char *)cell = c;
      else bigint_from_int(cell, c);
    }
  } else {
    /* number io */
    bigint_init(&input);

    /* use bigint for input even if we're reading to wrapping cells */
    do {
      if(prompt) {
        fprintf(stderr, "Input: ");
        fflush(stderr);
      }

      if(!(fgets(buf, 1024, stdin))) goto handle_eof;
      if((p = strchr(buf, '\n'))) *p = '\0';
    } while(bigint_from_string(&input, buf) == BIGINT_ILLEGAL_PARAM);

    if(wrap) {
      bigint_to_int(&input, &c);
      *(unsigned char *)cell = c;
    } else bigint_copy(cell, &input);

    bigint_release(&input);
  }

  return;

 handle_eof:
  if(strcasecmp(eof_value, "nochange") != 0) {
    bigint_from_string(cell, eof_value);
  }
}

/* Write output from the current cell */
void output(Memory *mem) {
  void *cell;
  int c;
  char *buf;

  if(!(cell = get_cell(mem))) return;

  if(chario) {
    /* character io */
    if(wrap) c = *(unsigned char *)cell;
    else bigint_to_int(cell, &c);

    fputc(c, stdout);
  } else {
    /* number io */
    if(wrap) printf("%d\n", *(unsigned char *)cell);
    else {
      buf = malloc(bigint_string_length(cell));

      bigint_to_string(cell, buf);
      fputs(buf, stdout); fputc('\n', stdout);

      free(buf);
    }
  }
}

/* Return 1 if the current cell is 0 and 0 otherwise */
int is_zero(Memory *mem) {
  void *cell;

  /* memory leak, return 0 even though the program is about to die */
  if(!(cell = get_cell(mem))) return 0;

  if(wrap) return *(unsigned char *)cell == 0;
  else return bigint_is_zero(cell);
}
