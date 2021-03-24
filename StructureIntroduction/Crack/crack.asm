global _start

section .data

user_password          equ 128
right_password         equ 5
count_of_smbls         equ 1
correct_password_len   equ 17
incorrect_password_len equ 19

section .text

;nasm -f elf64 -l crack.lst crack.asm && ld -s -o crack  crack.o && ./crack

_start:
        mov r9, UserPasswordBuffer
        dec r9

Scanf:  
        inc r9

        mov rax, 0
        mov rdi, 0
        mov rsi, r9
        mov rdx, count_of_smbls
        syscall

        cmp byte [r9], 0x0a
        jne Scanf

        mov r8, CorrectPasswordBuf
        mov r9, UserPasswordBuffer

        xor rcx, rcx

MyLoop:
        mov r10b, byte [r8]
        mov r11b, byte [r9]

        inc r8
        inc r9

        inc rcx

        cmp r10b, r11b
        je MyLoop

        cmp rcx, right_password
        je CorrectPassword

IncorrectPasword:
        mov rsi, IncorrectMsg
        mov rdx, incorrect_password_len
        jmp WriteMsg

CorrectPassword:
        mov rsi, CorrectMsg
        mov rdx, correct_password_len

WriteMsg:
        mov rax, 0x01
        mov rdi, 1
        syscall

        mov rax, 0x3c
        xor rdi, rdi            
        syscall

section .data

CorrectMsg:
        db "Correct password", 10, 0
IncorrectMsg:
        db "Incorrect password", 10, 0

UserPasswordBuffer:
        resb user_password
CorrectPasswordBuf:
        db "1337", 0
