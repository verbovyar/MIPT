 
global main
extern printf

section .text 

main:
        mov rdi, FormatString
        mov rsi, LoveMsg
        mov rdx, 3802
        mov rcx, AndMsg
        mov r8,  YouMsg
        mov r9,  100
        push 255
        push 33

        xor rax, rax
        call printf 

        mov rax, 0x3c 
        xor rdi, rdi            
        syscall

section .data 

FormatString:   db "I %s %x %s %s %d%%%c%x", 10, 0
LoveMsg:        db "love", 0
AndMsg:         db "and", 0
YouMsg:         db "you", 0