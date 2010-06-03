/* Options parsing for bnbf

   James Stanley 2010 */

#include "bnbf.h"

static error_t parse_option(int key, char *arg, struct argp_state *state);

const char *argp_program_version = VERSION;
const char *argp_program_bug_address = MAINTAINER;

static const char doc[] =
  VERSION " - a brainfuck interpreter with bignum memory";

static const char args_doc[] = "FILES";

static const struct argp_option options[] = {
  { "char-io", 'c', 0, 0, "Use character I/O instead of numbers" },
  { "wrap", 'w', 0, 0, "Wrap memory as if it consisted of unsigned bytes" },
  { NULL }
};

struct argp argp = { options, parse_option, args_doc, doc };

/* command line options */
int chario = 0;
int wrap = 0;
char **input;
int num_inputs = 0;

/* Parse a single option for argp_parse */
static error_t parse_option(int key, char *arg, struct argp_state *state) {
  switch(key) {
  case 'c':
    fprintf(stderr, "Chario unimplemented.\n");
    chario = 1;
    break;
  case 'w':
    fprintf(stderr, "Wrapping unimplemented.\n");
    wrap = 1;
    break;
  case ARGP_KEY_ARG:
    input = realloc(input, sizeof(char *) * ++num_inputs);
    input[num_inputs - 1] = arg;
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}
