/* bnbf - BigNum BrainFuck

   James Stanley 2010 */

#include "bnbf.h"

int main(int argc, char **argv) {
  int i;

  argp_parse(&argp, argc, argv, 0, 0, NULL);

  /* No inputs? Use stdin */
  if(num_files == 0) {
    file = malloc(sizeof(char *));
    *file = "-";
    num_files = 1;
  }

  for(i = 0; i < num_files; i++) {
    run_program(file[i]);
  }

  return 0;
}
