DisplayDigit:
    ;read argument from calling stack
    ;save context regs which will be used
    ;locally
    push ecx
    push edx
    push edi

    mov edi, []

DisplayInteger:
    ;Get Argument from calling stack of caller
    ;from right to left, MRB goes to stack first
    mov eax, [esp + 4]
    shr eax, 24
    call DispalyDigit
DisplayString:
    

DisplayReturn:
    push 
    call DisplayString
    add esp, 4
    ret