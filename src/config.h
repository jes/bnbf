/* Compile-time configuration of bnbf */

/* To choose between GMP (faster) and built-in bigint, see Makefile */

#define VERSION "bnbf 0.1"

/* NOTE: Remember to update the BUGS section in the man page */
#define MAINTAINER "James Stanley <james@incoherency.co.uk>"

/* Must be a power of 2 */
#define INIT_MEM_SIZE (1 << 12)
