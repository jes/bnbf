/* bnbf - BigNum BrainFuck

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

int main(int argc, char **argv) {
  parse_options(argc, argv);

  /* Initialise wrapping constants if necessary */
  if(wrap) {
    bigint_init(&zero);
    bigint_init(&ff);
    bigint_add_by_int(&ff, 0xff);
  }

  /* No input files? Use stdin */
  if(optind >= argc) run_program("-");
  else while(optind < argc) run_program(argv[optind++]);

  return 0;
}
