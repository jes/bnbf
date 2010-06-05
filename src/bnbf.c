/* bnbf - BigNum BrainFuck

   See "COPYING" for license details

   James Stanley 2010 */

#include "bnbf.h"

int main(int argc, char **argv) {
  parse_options(argc, argv);

  /* No input files? Use stdin */
  if(optind >= argc) run_program("-");

  while(optind < argc) run_program(argv[optind++]);

  return 0;
}
