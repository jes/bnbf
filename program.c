/* Program stuff for bnbf

   James Stanley 2010 */

#include "bnbf.h"

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

/* Runs the program with the given file name. "-" means stdin */
void run_program(const char *name) {
  FILE *fp;
  Inst *program = NULL;
  Inst *inst = NULL;
  Inst *i;
  int c;
  char *instructions = "+-<>[],.";
  Memory *mem;

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

    if(program) {
      inst->next = malloc(sizeof(Inst));
      inst = inst->next;
    } else {
      inst = malloc(sizeof(Inst));
      program = inst;
    }

    inst->type = c;
    inst->loop = NULL;
    
    if(c == '[') {
      /* loop entry, push its address on to the stack */
      push(inst);
    } else if(c == ']') {
      if(sp <= 0) {
        fprintf(stderr, "%s: mismatched loops.\n", name);
        free(stack);
        free_program(program);
        return;
      }

      /* loop exit, fill in loop addresses */
      i = pop();
      i->loop = inst;
      inst->loop = i;
    }
  }
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

  inst = program;
  while(inst->type != '!') {
    switch(inst->type) {
    case '+':
      add(mem, 1);
      break;
    case '-':
      add(mem, -1);
      break;
    case '>':
      mem->mp++;
      break;
    case '<':
      mem->mp--;
      break;
    case ',':
      input(mem);
      break;
    case '.':
      output(mem);
      break;
    case '[':
      if(is_zero(mem)) inst = inst->loop;
      break;
    case ']':
      if(!is_zero(mem)) inst = inst->loop;
      break;
    }

    inst = inst->next;
  }

  free_program(program);
  free_memory(mem);
  /* stack is already empty if we got this far */
}

/* Walk the list freeing each node */
void free_program(Inst *prog) {
  Inst *prog_next;

  for(; prog; prog = prog_next) {
    prog_next = prog->next;
    free(prog);
  }
}
