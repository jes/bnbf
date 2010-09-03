# bnbf - the bignum brainfuck interpreter

bnbf is a brainfuck interpreter featuring bignum memory cells and many runtime
options.

There is a git repository at http://github.com/jes/bnbf which is where new
development happens.

  1. Compiling
  2. Usage
  3. Contact

## 1. Compiling

Before running bnbf you will need to compile it. This is relatively
straightforward, simply run  
  `$ make`  
in the directory containing this file and the `Makefile`, and bnbf will be
compiled. The executable will be the file `src/bnbf`, relative to the directory
containing the `Makefile`.

After you've compiled bnbf, it will probably be a good idea to run the test
cases to make sure everything is in order. To do this, run  
  `$ make test`  
and ensure that there are 0 failures.

Having compiled and tested bnbf, you may want to install it. If you are the root
user, running  
  `$ make install`  
will do this for you, otherwise just copy `src/bnbf` to somewhere in your path
(for example, `~/bin`). There is also a man page in the file `bnbf.1` which you
can put somewhere in your MANPATH.

## 2. Usage

The man page is the place you should go to for help, but running  
  `$ bnbf --help`  
or  
  `$ bnbf --usage`  
will probably tell you all you need to know. The general method of running bnbf
is, for example  
  `$ bnbf --char-io ~/brainfuck/hello_world`  
if you have a program in `~/brainfuck/hello_world` which needs to output
characters. As bnbf was designed with numeric computation in mind, the default
is for I/O to be numeric instead of character-based.

If you wish to emulate the byte-array memory of other implementations, you can
pass the `--wrap` option, which will cause cell values to wrap back to 0 when
incrementing 255, and to wrap up to 255 when decrementing 0. This is no longer
implemented using bignum, so there is a fairly significant (roughly 10x)
performance advantage to using `--wrap` where possible or appropriate.

With `--wrap` enabled, bnbf is currently about 30% slower than the `beef`
interpreter available from `http://kiyuko.org/software/beef`. I suspect this is
because there are a lot of places where bnbf tests whetherthe flag for the
`--wrap` option is set, whereas beef does not support bignum cells, so it knows
always to use byte-array memory.

## 3. Contact

If you find a bug or just want to contact me for any reason, send an email to  
  James Stanley <james@incoherency.co.uk>  
or find me on IRC as the user `jamesstanley` on `irc.freenode.net`. I am
usually in the channel `#maximilian`, so you'll probably find me in there.
