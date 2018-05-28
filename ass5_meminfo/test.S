
extern choose ;function/variable = only extern name

[SECTION .data]

strHello        db      "Hello World", 0xA
strLen          equ     $ - strHello

num1st          dd      3
num2nd          dd      4

[SECTION .text]
    global _start
    global myprint

_start:
    push dword [num2nd] ; arguments from right to left 
    push dword [num1st] ; push on stack
    call choose
    add  esp, 8         ; convetion = caller restore stack

    mov ebx, 0
    mov eax, 1
    int 0x80

myprint:
    mov edx, dword [esp + 8]
    mov ecx, dword [esp + 4]
    mov ebx, 1
    mov eax, 4
    int 0x80
    ret
