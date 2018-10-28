Program build and execution
---------------------------

Your program should be built using the command:

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


