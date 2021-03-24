
section .text

global _start

_start:

    mov rax, 0x01
    mov rsi, enterPassword
    mov rdi, 1
    mov rdx, [enterPasswordLen]
    syscall
    mov r8, 0
    ScanLoop:
        mov rax, 0x00
        mov rsi, PassBuffer
        add rsi, r8
        mov rdi, 0
        mov rdx, 1
        syscall
        ; mov rax, 0x01
        ; mov rsi, PassBuffer
        ; add rsi, r8
        ; mov rdi, 1
        ; mov rdx, 1
        ; syscall
        ; mov r9b, [PassBuffer + r8]
        inc r8
        cmp byte [rsi], 10
        jne ScanLoop
    mov byte [rsi], 0
    mov rax, PassBuffer
    mov rdx, RightPassword
    mov rcx, [RightPasswordLen]
    ; inc rcx
    CmpLoop:
        cmp rcx, 0
        je AccessGranted
        mov r8b, [rax]
        mov r9b, [rdx]
        cmp r8b, r9b
        jne AccessDenied
        inc rax
        inc rdx
        dec rcx
        jmp CmpLoop
    AccessGranted:
    mov rsi, CorrectPasswordStr
    mov rax, 0x01
    mov rdi, 1
    mov rdx, [CorrectPasswordStrLen]
    syscall
    jmp End
    AccessDenied:
    mov rsi, IncorrectPasswordStr
    mov rax, 0x01
    mov rdi, 1
    mov rdx, [IncorrectPasswordStrLen]
    syscall
    End:
    mov rax, 0x3C      ; exit64 (rdi)
    xor rdi, rdi
    syscall

section     .data

PassBuffer  times 512 db  0
RightPasswordLen dq 11
RightPassword db "helloworld", 0x00
enterPasswordLen dq 21
enterPassword db "Enter your password:", 10
CorrectPasswordStrLen dq 15
CorrectPasswordStr db "Access granted", 10
IncorrectPasswordStrLen dq 14
IncorrectPasswordStr db "Access denied", 10
