/* Program stuff for bnbf

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

const char *program_name;
int stop_program;

static void **stack;
static int sp = 0;

/* push a pointer on to the stack */
static void push(void *p) {
  stack = realloc(stack, sizeof(void *) * ++sp);
  stack[sp - 1] = p;
}

/* pop a pointer off the stack */
static void *pop(void) {
  void *p;

  p = stack[--sp];
  stack = realloc(stack, sizeof(void *) * sp);

  return p;
}

/* Allocate and return a new instruction for the given program, updating the
   length */
static Inst *new_inst(char type, Inst **program, size_t *len) {
  Inst *i;

  *program = realloc(*program, (*len + 1) * sizeof(Inst));
  i = *program + *len;
  (*len)++;

  i->type = type;
  i->u.loop = NULL;
  i->u.amount = 0;

  return i;
}

/* Runs the program with the given file name. "-" means stdin */
void run_program(const char *name) {
  FILE *fp;
  Inst *program = NULL;
  Inst *inst = NULL;
  Inst *i;
  size_t len = 0;
  int c;
  char *instructions = "+-<>[],.";
  char *p;
  Memory *mem = NULL;

  /* benchmarking counters */
  long inst_count = 0;
  long steps = 0;
  long high_mp = 0;
  long low_mp = 0;

  /* Open the program for reading */
  if(strcmp(name, "-") == 0) {
    fp = stdin;
    name = "stdin";
  } else {
    if(!(fp = fopen(name, "r"))) {
      fprintf(stderr, "%s: unable to open for reading.\n", name);
      return;
    }
  }

  /* Read the program in to the instruction list */
  while((c = fgetc(fp)) != EOF) {
    if(!strchr(instructions, c)) continue;

    inst_count++;

    switch(c) {
      case SLOOP:/* loop entry, push address on to stack */
        inst = new_inst(c, &program, &len);
        push(inst);
        break;

      case ELOOP:/* loop exit, sort out loop addresses */
        inst = new_inst(c, &program, &len);

        if(sp <= 0) {
          /* loop exit with no corresponding start */
          fprintf(stderr, "%s: mismatched loops (missing start-loop).\n", name);
          goto cleanup;
        }

        i = pop();
        i->u.loop = inst;
        inst->u.loop = i;
        break;

      case ADD:  case SUB:   /* addition/subtraction, adjust amount of adds */
      case LEFT: case RIGHT: /* inc/dec pointer, adjust amount of moves     */
        if(inst && inst->type == c && inst->u.amount < INT_MAX) {
          inst->u.amount++;
        } else {
          inst = new_inst(c, &program, &len);
          inst->u.amount = 1;
        }
        break;

      default:
        inst = new_inst(c, &program, &len);
        break;
    }
  }

  /* don't close stdin, we need it for program input */
  if(fp != stdin) fclose(fp);

  /* empty program */
  if(!program) return;

  if(sp > 0) {
    fprintf(stderr, "%s: mismatched loops (missing %d end-loop%s).\n", name,
            sp, sp == 1 ? "" : "s");
    goto cleanup;
  }

  /* add the end-program instruction */
  new_inst(KILL, &program, &len);

  /* get some memory */
  mem = make_memory();

  /* set up globals */
  stop_program = 0;
  program_name = name;

  inst = program;
  while(!stop_program) {
    /* carry out instruction */
    switch(inst->type) {
      case ADD:
        add(mem, inst->u.amount);
        steps += inst->u.amount;
        break;

      case SUB:
        add(mem, -inst->u.amount);
        steps += inst->u.amount;
        break;

      case LEFT:
        mem->mp -= inst->u.amount;
        if(mem->mp < low_mp) low_mp = mem->mp;
        steps += inst->u.amount;
        break;

      case RIGHT:
        mem->mp += inst->u.amount;
        if(mem->mp > high_mp) high_mp = mem->mp;
        steps += inst->u.amount;
        break;

      case INPUT:
        input(mem);
        steps++;
        break;

      case OUTPUT:
        output(mem);
        steps++;
        break;

      case SLOOP:
        if(is_zero(mem)) inst = inst->u.loop;
        steps++;
        break;

      case ELOOP:
        if(!is_zero(mem)) inst = inst->u.loop;
        steps++;
        break;

      case KILL:
        stop_program = 1;
        break;
    }

    inst++;
  }

  /* counter the adjacency optimisations in case of overflow so that the memory
     addresses reached are what they would be if there was no adjacency
     optimisation */
  if(maxmem) {
    if(high_mp > (maxmem + 1)) high_mp = maxmem + 1;
    if(low_mp < (-maxmem - 1)) low_mp = -maxmem - 1;
  }

  /* benchmarking information */
  if(benchmark) {
    fprintf(stderr, "%s: total program length: %ld\n", program_name,
            inst_count);
    fprintf(stderr, "%s: execution steps used: %ld\n", program_name,
            steps);
    fprintf(stderr, "%s: high address visited: %ld\n", program_name,
            high_mp);
    fprintf(stderr, "%s:  low address visited: %ld\n", program_name,
            low_mp);
  }

cleanup:
  free(stack);
  free(program);
  free_memory(mem);
}
