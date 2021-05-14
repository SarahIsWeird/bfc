section .text
    foo:
    default rel
    db "hola"

global _main
_main:
    lea rbx, [foo]
    mov byte [foo], 0Ah

    mov rdx, 1
    mov rsi, rbx
    mov rdi, 1
    mov rax, 0x02000004
    syscall

    mov rax, 0x02000001
    xor rdi, rdi
    syscall