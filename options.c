/* Options parsing for bnbf

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

static error_t parse_option(int key, char *arg, struct argp_state *state);

const char *argp_program_version = VERSION;
const char *argp_program_bug_address = MAINTAINER;

static const char doc[] =
  VERSION " - a brainfuck interpreter with bignum memory";

static const char args_doc[] = "FILES";

static const struct argp_option options[] = {
  { "benchmark", 'b', 0, 0,
    "Print benchmarking information after program termination" },
  { "char-io", 'c', 0, 0,
    "Use character I/O instead of numbers" },
  { "eof", 'e', "value", 0,
    "Set the value to use when EOF is encountered on input (default: "
    "\"nochange\")" },
  { "max-mem", 'm', "address", 0,
    "Set limit on highest valid memory address" },
  { "wrap", 'w', 0, 0,
    "Wrap values as if they were unsigned bytes" },
  { NULL }
};

struct argp argp = { options, parse_option, args_doc, doc };

/* command line options */
int benchmark = 0;
int chario = 0;
char *eof_value = "nochange";
int maxmem = 0;
int wrap = 0;
char **file;
int num_files = 0;

/* Parse a single option for argp_parse */
static error_t parse_option(int key, char *arg, struct argp_state *state) {
  switch(key) {
  case 'b':
    benchmark = 1;
    break;
  case 'c':
    chario = 1;
    break;
  case 'e':
    eof_value = arg;
    break;
  case 'm':
    maxmem = atoi(arg);
    break;
  case 'w':
    wrap = 1;
    break;
  case ARGP_KEY_ARG:
    file = realloc(file, sizeof(char *) * ++num_files);
    file[num_files - 1] = arg;
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}
