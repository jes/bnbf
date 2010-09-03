/* Options parsing for bnbf

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

/* command line options */
int benchmark = 0;
int chario = 0;
char *eof_value = "nochange";
int maxmem = 0;
int noneg = 0;
int prompt = 0;
int wrap = 0;

static struct option long_options[] = {
  { "benchmark",   no_argument,       NULL, 'b' },
  { "char-io",     no_argument,       NULL, 'c' },
  { "eof",         required_argument, NULL, 'e' },
  { "help",        no_argument,       NULL, 'h' },
  { "max-mem",     required_argument, NULL, 'm' },
  { "no-negative", no_argument,       NULL, 'n' },
  { "prompt",      no_argument,       NULL, 'p' },
  { "usage",       no_argument,       NULL, 'U' },
  { "version",     no_argument,       NULL, 'V' },
  { "wrap",        no_argument,       NULL, 'w' },
  { NULL,          no_argument,       NULL, '\0' }
};

/* Output for --help */
static void help(void) {
  puts(
  "Usage: bnbf [OPTION...] FILE...\n"
  VERSION " - a brainfuck interpreter with bignum memory\n"
  "\n"
  "  -b, --benchmark         Print benchmarking information after program\n"
  "                          termination\n"
  "  -c, --char-io           Use character I/O instead of numbers\n"
  "  -e, --eof=value         Set the value to use when EOF is encountered on\n"
  "                          input (default: \"nochange\")\n"
  "  -m, --max-mem=address   Set limit on highest valid memory address\n"
  "  -n, --no-negative       Prevent negative memory address from being used\n"
  "  -p, --prompt            Print a prompt when waiting for number input\n"
  "  -w, --wrap              Wrap cell values as if they were unsigned bytes\n"
  "  -h, --help              Give this help list\n"
  "      --usage             Give a short usage message\n"
  "  -V, --version           Print program version\n"
  "\n"
  "Report bugs to " MAINTAINER ".");

  exit(0);
}

/* Output for --version */
static void version(void) {
  puts(VERSION);

  exit(0);
}

/* Output for --usage */
static void usage(void) {
  puts(
  "Usage: bnbf [-bchnpwV] [-e value] [-m address] [--benchmark] [--char-io]\n"
  "            [--eof=value] [--max-mem=address] [--no-negative] [--prompt]\n"
  "            [--wrap] [--help] [--usage] [--version] FILE...");

  exit(0);
}

/* Parses the program options using getopt_long() */
void parse_options(int argc, char **argv) {
  int c;
  int option_index = 0;

  while((c = getopt_long(argc, argv, "bce:hm:npVw", long_options,
                         &option_index)) != -1) {
    switch(c) {
    case 'b': benchmark = 1;         break;
    case 'c': chario = 1;            break;
    case 'e': eof_value = optarg;    break;
    case 'h': help();                break;
    case 'm': maxmem = atoi(optarg); break;
    case 'n': noneg = 1;             break;
    case 'p': prompt = 1;            break;
    case 'U': usage();               break;
    case 'V': version();             break;
    case 'w': wrap = 1;              break;
    case '?': exit(1);
    }
  }
}
