/* bnbf - BigNum BrainFuck

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

int main(int argc, char **argv) {
  Bignum *b = new_bignum();
  add_bignum(b, 0x11);
  print_bignum(b, stdout);
  putchar('\n');
  add_bignum(b, -1);
  print_bignum(b, stdout);
  putchar('\n');

  parse_options(argc, argv);

  /* No input files? Use stdin */
  if(optind >= argc) run_program("-");

  while(optind < argc) run_program(argv[optind++]);

  return 0;
}
