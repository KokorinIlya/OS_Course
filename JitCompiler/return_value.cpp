#include <cstdio>

int return_value()
{
    return 42;
}

int main()
{
    printf("main() from return_value called");
    return 0;
}
/*
Line 4 of "return_value.cpp" starts at address 0x400526 <return_value()> and ends at 0x40052a <return_value()+4>.
*/

/*
Dump of assembler code for function return_value():
   0x0000000000400526 <+0>:	55	push   %rbp
   0x0000000000400527 <+1>:	48 89 e5	mov    %rsp,%rbp
   0x000000000040052a <+4>:	b8 2a 00 00 00	mov    $0x2a,%eax
   0x000000000040052f <+9>:	5d	pop    %rbp
   0x0000000000400530 <+10>:	c3	retq
End of assembler dump.
 */