/* Santa Zhang wrote the original version of this code

   "I'd release it in public domain. Just do whatever you want to :)
   But it does not support division, and currently I don't have a plan to
   implement that, sorry :)"
*/

#ifndef PRJ_KNUTH_BIGINT_H_
#define PRJ_KNUTH_BIGINT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

  // point to the raw data
  // when the bigint is finalized, this will be set to NULL
  int* p_data;

  // sign of the number
  // could be 0, 1, -1
  int sign;

  // length of raw data array (data_len <= mem_size)
  int data_len;

  // allocated size in memory (data_len <= mem_size)
  int mem_size;

} bigint;

typedef enum {

  BIGINT_NOERR = 0,

  BIGINT_ILLEGAL_PARAM = 1,

  BIGINT_OVERFLOW = 2,

} bigint_errno;

// initialize a big int, set it to 0
void bigint_init(bigint* p_bigint);

// release a big int, free the raw data, and set it to
void bigint_release(bigint* p_bigint);

// set the bigint to a int value
void bigint_from_int(bigint* p_bigint, int value);

// set the bigint to a double value in text
// the double value will be rounded towards the nearest int
//
// eg:
// 0.1  ->  0
// 0.51 ->  1
// -0.3  ->  0
// -0.9  -> -1
//
// if the double is NaN or Inf, then the bigint will be set to 0, and
// returned bigint_errno will be 'BIGINT_ILLEGAL_PARAM'
// else, then returned bigint_errno will be 'BIGINT_NOERR' (0)
//
// example of supported string:
// '0', '+0', '-0', '0.0'
// '-234234', '0.3'
// '3e3'
//
// literally, here is the constraint:
// (1) a number is represented in 3 parts: fixed, friction, mantissa
// (2) fixed is required, and could be preceded by an optional sign ('+', '-'),
//     space is NOT allowed between the sign and the fixed part
// (3) friction is optional. if it exists, the fixed part and friction
//     part MUST be separated by a period, space is NOT allowed
// (4) mantissa is optional. if it exists, it must be preceded by a 'e' or 'E'.
//     the mantiss could have an optional sign, and space is NOT allowed
//
// grammar for a parse could be like:
//
// value ::= fixed friction_opt mantissa_opt ;
//
// fixed ::= ('+'|'-'|'')[0-9]+ ;
//
// friction_opt ::= '.'[0-9]+
//                | ''
//                ;
//
// mantissa_opt ::= ('e'|'E')('+'|'-'|'')[0-9]+
//                | ''
//                ;
//
bigint_errno bigint_from_string(bigint* p_bigint, char* str);

// get number of digits in a bigint
int bigint_digit_count(bigint* p_bigint);

// return the minimum length of char array to store the text representation
// of a bigint
int bigint_string_length(bigint* p_bigint);

// convert the bigint into string
void bigint_to_string(bigint* p_bigint, char* str);

// convert the bigint into int
// if overflow, the returned bigint_errno will be '-BIGINT_OVERFLOW'
// else it will be '-BIGINT_NOERR'
bigint_errno bigint_to_int(bigint* p_bigint, int* p_int);

// copy a big int's value into another bigint
void bigint_copy(bigint* p_dst, bigint* p_src);

// changes the sign of the bigint
void bigint_change_sign(bigint* p_bigint);

// test if positive, return 1 if yes, and 0 if not
int bigint_is_positive(bigint* p_bigint);

// test if negative, return 1 if yes, and 0 if not
int bigint_is_negative(bigint* p_bigint);

// test if zero. return 1 if yes, and 0 if not
int bigint_is_zero(bigint* p_bigint);

// set a bigint to 0
void bigint_set_zero(bigint* p_bigint);

// set a bigint to 1
void bigint_set_one(bigint* p_bigint);

// addition, p_dst could be the same with p_scr
void bigint_add_by(bigint* p_dst, bigint* p_src);

// add by a single int value
void bigint_add_by_int(bigint* p_dst, int value);

// return -1 if p_bigint1 < p_bigint2
// return 0 if p_bigint1 == p_bigint2
// return 1 if p_bigint1 > p_bigint2
int bigint_compare(bigint* p_bigint1, bigint* p_bigint2);

// return 1 if p_bigint1 == p_bigint2 (value equal)
int bigint_equal(bigint* p_bigint1, bigint* p_bigint2);

// gets the nth digit in the bigint
// if nth < 0, then return -1
// if nth is bigger than the bigint's length, return will always be 0
int bigint_nth_digit(bigint* p_bigint, int nth);

#ifdef __cplusplus
};
#endif

#endif
