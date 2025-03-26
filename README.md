# Basic C++ virtual machine

Input file containing assembly. Machine operates on the assembly. 

Currently supports the following ops:
- exit
- movi
- add
- mov
- sub
- mul
- div
- inc
- dec
- jmp
- cmpeq
- cmplt
- jt
- jnt
- push
- pop
- load
- store
- loadi
- storei
- loadsc
- storesc
- pendown
- penup
- fwd
- back


Sample usage:
```
const char* argv[] = {"tests/tests", "input/.psm"};
ProcessCommandArgs(2, argv);
```

Apologies for the little documentation. More coming soon. 
