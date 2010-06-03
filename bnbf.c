/* bnbf - BigNum BrainFuck

   James Stanley 2010 */

#include "bnbf.h"

int main(int argc, char **argv) {
  int i;

  argp_parse(&argp, argc, argv, 0, 0, NULL);

  /* No inputs? Use stdin */
  if(num_inputs == 0) {
    input = malloc(sizeof(char *));
    *input = "-";
    num_inputs = 1;
  }

  for(i = 0; i < num_inputs; i++) {
    /* run_program(input[i]); */
  }

  return 0;
}
