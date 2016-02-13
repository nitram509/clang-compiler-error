


## BUG: clang++ compiler generates return code instead of void (nothig)

### Description

When compiling c++ code, clang is able to optimize code.
Such can also mean removing un-necessary if-clauses to check for NULL.

The attached example still compiles a return value into the effective binary,
which shouldn't be there.

Comparing clang for OS X and Linux, the apple version contains the return value,
the linux version doesn't.
Tested against:
* Apple LLVM version 7.0.2 (clang-700.1.81), Target: x86_64-apple-darwin15.3.0
* Ubuntu clang version 3.6.0-2ubuntu1 (tags/RELEASE_360/final) (based on LLVM 3.6.0)
Apple LLVM version 7.0.2 (clang-700.1.81), Target: x86_64-apple-darwin15.3.0

### Steps to reproduce

1. copy both attached files to a temporary folder
2. run 'make' within this folder
3. execute the binary 'example_constructor_check_wrong_return_code'
4. you will see two lines

```
   example_constructor_check_wrong_return_code() = bad
   example_control_may_reach_end_of_non_void_function() = 2a     // ???
```
The second line may have a different value then '2a'

### Expected Results

The first line should NOT print 'bad'.

Technically spoken, because of the optimizer within the clang
is removing the conditional statement (line:22) if (rect==null)
there should be no explicit '0xbad' return code for the function example_constructor_check_wrong_return_code().

When disassembling, the method
example_constructor_check_wrong_return_code()
should look like the method
example_control_may_reach_end_of_non_void_function()

To be precise.

;-- _example_control_may_reach_end_of_non_void_function:
;-- func.100000e90:
    │   0x100000e90      55             push rbp
    │   0x100000e91      4889e5         mov rbp, rsp
    │   0x100000e94      bf08000000     mov edi, 8
    │   0x100000e99      e866000000     call sym.imp._Znwm
    │   0x100000e9e      48c700000000.  mov qword [rax], 0
    │   0x100000ea5      488905740100.  mov qword [rip + 0x174], rax ; [0x100001020:8]=0x1000007feedfacf  LEA section.8.__common ; section.8.__common
    │   0x100000eac      be2a000000     mov esi, 0x2a              ; '*' ; "PAGEZERO" @ 0x2a
    │   0x100000eb1      ba17000000     mov edx, 0x17
    │   0x100000eb6      4889c7         mov rdi, rax
    │   0x100000eb9      5d             pop rbp
    └─< 0x100000eba      e981ffffff     jmp sym.__ZN9Rectangle10set_valuesEii
        0x100000ebf      90             nop

### Actual Results

As written in the expected results,
the current binary code of method
example_constructor_check_wrong_return_code()
contains a return statement, with a given return value '0xbad'.

;-- _example_constructor_check_wrong_return_code:
;-- func.100000e50:
    │   0x100000e50      55             push rbp
    │   0x100000e51      4889e5         mov rbp, rsp
    │   0x100000e54      bf08000000     mov edi, 8
    │   0x100000e59      e8a6000000     call sym.imp._Znwm
    │   0x100000e5e      48c700000000.  mov qword [rax], 0
    │   0x100000e65      488905b40100.  mov qword [rip + 0x1b4], rax
    │   0x100000e6c      be2a000000     mov esi, 0x2a
    │   0x100000e71      ba17000000     mov edx, 0x17
    │   0x100000e76      4889c7         mov rdi, rax
    │   0x100000e79      e8c2ffffff     call sym.__ZN9Rectangle10set_valuesEii
    │   0x100000e7e      b8ad0b0000     mov eax, 0xbad      <<<=== this shouldn't be there
    │   0x100000e83      5d             pop rbp
    │   0x100000e84      c3             ret

### Environment, Versions

XCode Version: Version 7.2.1 (7C1002)
OSX Version: 10.11.3 (15D21)
Clang: Apple LLVM version 7.0.2 (clang-700.1.81), Target: x86_64-apple-darwin15.3.0