[BITS 64]
[ORG 0x100000]

start:
    mov rsi, stack_space + stack_size
    mov rsp, rsi

    call kernel_entry

hang:
    jmp hang

section .bss
align 16
stack_space:
    resb 8192
stack_size equ $ - stack_space
