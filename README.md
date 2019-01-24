Create a compiler for the C language
====================================

The program can read C source code from a file, and write
MIPS assembly to another file.

Compiler build and execution
---------------------------

The program is built using the command:

    make bin/c_compiler

The compilation function is invoked using the flag `-S`, with
the source file and output file specified on the command line:

    bin/c_compiler -S [source-file.c] -o [dest-file.s]
    
Input Format
------------

The input format is the same as for the translator (pre-processed C90).

Output Format
-------------

The output format is MIPS1 assembly code.

It is possible to assemble and link this code
against a C run-time, and have it execute correctly
on a MIPS processor as emulated by `qemu-mips`.


Test case format
----------------

Each test case has a name ${NAME}, and consists of two files:

- `${NAME}.c` : The source file to be compiled by the compiler under test.
  This should be very minimal, and use the smallest amount of code needed
  to perform the test.

- `${NAME}_driver.c` : A driver file to be compiled by GCC, which knows how
  to invoke the functionality in the tested file. This can contain any
  C code at all, as it will be compiled by GCC.

The testing process for a test-case is then:

1 - Compile `${NAME}.c` using the compiler under test into assembly.

2 - Compile `${NAME}_driver.c` using MIPS GCC.

3 - Link the generated assembly and the driver object into a MIPS executable.

4 - Run the executable under QEMU

5 - If the executable returns 0 (via `$?` in the shell), then the test-case has passed.

If any of these steps fail, then either the test-case is malformed,
or the compiler under test is not generating correct code.



Translator build and execution
---------------------------

The program is built using the command:

    make bin/c_compiler

The translator function is invoked using the flag `--translate`, with
the source file and output file specified on the command line:

    bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    
You can assume that the command-line arguments will always be in this
order, and that there will be no spaces in source or destination paths.

Input format
------------

The input file will be pre-processed [ANSI C](https://en.wikipedia.org/wiki/ANSI_C),
also called C90 or C89. It's what's generally thought of as "classic" or "normal" C,
but not the _really_ old one without function prototypes (you may never have come
across that). C90 is still often used in embedded systems, and pretty much the
entire linux kernel is in C90.


Output format
-------------

The output format will be a Python3equivalent to the input C.

As an example, we would translate the input program:

```
int f(int x)
{
  return x*x;
}
```
into the python:

def f(x):
    return x*x
```

Or this program:
```
int f(int x)
{
  if(x>1){
    return x*f(x-1);
  }else{
    return x;
  }
}
```
turns into this:
```
def f(x):
   if x>1:
       return x*f(x-1)
   else:
       return x
```


