## phase_1

我们首先查看第一段汇编代码：

```asm
(lldb) disassemble -n phase_1
bomb`phase_1:
bomb[0x400ee0] <+0>:  subq   $0x8, %rsp
bomb[0x400ee4] <+4>:  movl   $0x402400, %esi           ; imm = 0x402400
bomb[0x400ee9] <+9>:  callq  0x401338                  ; strings_not_equal
bomb[0x400eee] <+14>: testl  %eax, %eax
bomb[0x400ef0] <+16>: je     0x400ef7                  ; <+23>
bomb[0x400ef2] <+18>: callq  0x40143a                  ; explode_bomb
bomb[0x400ef7] <+23>: addq   $0x8, %rsp
bomb[0x400efb] <+27>: retq
```

我们可以看到，这里直接调用了检测字符串是否相等的函数，所以我们可以查看`0x402400`位置的字符串。

```asm
(lldb) print (char*)0x402400
(char *) $3 = 0x0000000000402400 "Border relations with Canada have never been better."
```

由此得到了第一个字符串。

## phase_2

我们看一下汇编代码：

```asm
(lldb) disassemble -n phase_2
bomb`phase_2:
bomb[0x400efc] <+0>:  pushq  %rbp
bomb[0x400efd] <+1>:  pushq  %rbx
bomb[0x400efe] <+2>:  subq   $0x28, %rsp
bomb[0x400f02] <+6>:  movq   %rsp, %rsi
bomb[0x400f05] <+9>:  callq  0x40145c                  ; read_six_numbers
bomb[0x400f0a] <+14>: cmpl   $0x1, (%rsp)
bomb[0x400f0e] <+18>: je     0x400f30                  ; <+52>
bomb[0x400f10] <+20>: callq  0x40143a                  ; explode_bomb
bomb[0x400f15] <+25>: jmp    0x400f30                  ; <+52>
bomb[0x400f17] <+27>: movl   -0x4(%rbx), %eax
bomb[0x400f1a] <+30>: addl   %eax, %eax
bomb[0x400f1c] <+32>: cmpl   %eax, (%rbx)
bomb[0x400f1e] <+34>: je     0x400f25                  ; <+41>
bomb[0x400f20] <+36>: callq  0x40143a                  ; explode_bomb
bomb[0x400f25] <+41>: addq   $0x4, %rbx
bomb[0x400f29] <+45>: cmpq   %rbp, %rbx
bomb[0x400f2c] <+48>: jne    0x400f17                  ; <+27>
bomb[0x400f2e] <+50>: jmp    0x400f3c                  ; <+64>
bomb[0x400f30] <+52>: leaq   0x4(%rsp), %rbx
bomb[0x400f35] <+57>: leaq   0x18(%rsp), %rbp
bomb[0x400f3a] <+62>: jmp    0x400f17                  ; <+27>
bomb[0x400f3c] <+64>: addq   $0x28, %rsp
bomb[0x400f40] <+68>: popq   %rbx
bomb[0x400f41] <+69>: popq   %rbp
bomb[0x400f42] <+70>: retq
```

我们一段一段来分析。从`+9`开始读入了 6 个数字，存放了栈上。首先比较`(%rsp)`是否为 1，说明读入的第一个数为 1。然后`%rbx = %rsp + 4; %rbp = %rsp + 24`。从`+27`开始，`%eax = %rbx - 4 = %rsp`，说明是比较第一个数的两倍和第二个数是否相等，说明第二个数为 2。然后`%rbx = %rsp + 8;`，并和边界相比较。说明这六个数每个数都是前一个的两倍。那么字符串为`1 2 4 8 16 32。`
