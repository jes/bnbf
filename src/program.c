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

/* Allocate and return a new instruction for the given program, setting the
   program pointer if necessary, and setting the last instruction's next
   pointer */
static Inst *new_inst(char type, Inst **program, Inst *last) {
  Inst *i;

  i = malloc(sizeof(Inst));
  
  if(*program) last->next = i;
  else *program = i;

  i->type = type;
  i->u.loop = NULL;
  i->u.amount = 0;
  i->next = NULL;

  return i;
}

/* Runs the program with the given file name. "-" means stdin */
void run_program(const char *name) {
  FILE *fp;
  Inst *program = NULL;
  Inst *inst = NULL;
  Inst *i;
  int c;
  int inst_count = 0;
  char *instructions = "+-<>[],.";
  char *p;
  Memory *mem;

  /* benchmarking counters */
  int steps = 0;
  int high_mp = 0;
  int low_mp = 0;

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
    if(!(p = strchr(instructions, c))) continue;

    /* convert to instruction number */
    c = p - instructions;

    inst_count++;

    switch(c) {
      case SLOOP:/* loop entry, push address on to stack */
        inst = new_inst(c, &program, inst);
        push(inst);
        break;
      case ELOOP:/* loop exit, sort out loop addresses */
        inst = new_inst(c, &program, inst);

        if(sp <= 0) {
          /* loop exit with no corresponding start */
          fprintf(stderr, "%s: mismatched loops.\n", name);
          free(stack);
          free_program(program);
          return;
        }

        i = pop();
        i->u.loop = inst;
        inst->u.loop = i;
        break;
      case ADD: case SUB:/* addition/subtraction, adjust amount of adds */
        if(inst && inst->type == c) inst->u.amount += (c == ADD ? 1 : -1);
        else {
          inst = new_inst(c, &program, inst);
          inst->u.amount = (c == ADD ? 1 : -1);
        }
        break;
      case LEFT: case RIGHT:/* inc/dec pointer, adjust amount of moves */
        if(inst && inst->type == c) inst->u.amount += (c == RIGHT ? 1 : -1);
        else {
          inst = new_inst(c, &program, inst);
          inst->u.amount = (c == RIGHT ? 1 : -1);;
        }
        break;
      default:
        inst = new_inst(c, &program, inst);
        break;
    }
  }

  /* don't close stdin, we need it for program input */
  if(fp != stdin) fclose(fp);

  /* empty program */
  if(!program) return;

  if(sp > 0) {
    fprintf(stderr, "%s: mismatched loops.\n", name);
    free(stack);
    free_program(program);
    return;
  }

  /* add the end-program instruction */
  inst->next = malloc(sizeof(Inst));
  inst->next->type = '!';
  inst->next->next = NULL;

  /* get some memory */
  mem = make_memory();

  /* set up globals */
  stop_program = 0;
  program_name = name;

  inst = program;
  while(inst->type != '!' && !stop_program) {
    /* carry out instruction */
    switch(inst->type) {
      case ADD: case SUB:
        add(mem, inst->u.amount);
        break;
      case LEFT: case RIGHT:
        mem->mp += inst->u.amount;
        if(mem->mp > high_mp) high_mp = mem->mp;
        if(mem->mp < low_mp) low_mp = mem->mp;
        break;
      case INPUT:
        input(mem);
        break;
      case OUTPUT:
        output(mem);
        break;
      case SLOOP:
        if(is_zero(mem)) inst = inst->u.loop;
        break;
      case ELOOP:
        if(!is_zero(mem)) inst = inst->u.loop;
        break;
    }

    /* count number of instructions that would have been carried out had it not
       been for the optimising */
    switch(inst->type) {
      case ADD: case SUB: case LEFT: case RIGHT:
        steps += abs(inst->u.amount);
        break;
      default:
        steps++;
        break;
    }

    inst = inst->next;
  }

  /* benchmarking information */
  if(benchmark) {
    fprintf(stderr, "%s: total program length: %d\n", program_name, inst_count);
    fprintf(stderr, "%s: execution steps used: %d\n", program_name, steps);
    fprintf(stderr, "%s: high address visited: %d\n", program_name, high_mp);
    fprintf(stderr, "%s:  low address visited: %d\n", program_name, low_mp);
  }

  free_program(program);
  free_memory(mem);
  /* loop stack is already empty if we got this far, no need to free it */
}

/* Walk the list freeing each node */
void free_program(Inst *prog) {
  Inst *prog_next;

  for(; prog; prog = prog_next) {
    prog_next = prog->next;
    free(prog);
  }
}
