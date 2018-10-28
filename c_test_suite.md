
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

