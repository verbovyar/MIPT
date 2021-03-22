;------------------------------------------------
;Copy from register to str buffer

;Entry: 1 = Copyed register 
;       rdi = num of symbol from buffer
;       r11 = offset in buffer           
;
;Exit:  r11 = new offset in buffer
;       rdi = num of symbol
;
;Non const: rax, rbx, rcx, rdx
;------------------------------------------------
%macro WriteToStrBuf 1

        xor rcx, rcx
        mov cl, byte %1
        call CopyToStrBuf
        
%endmacro

;------------------
string_buffer_size equ 512
digit_buffer_size  equ 32
;------------------

; nasm -f elf64 -l Printf.lst Printf.asm && ld -s -o Printf  Printf.o && ./Printf

section .text

global _start 

_start:     
        push 15
        push 31
        push '!'
        push 100
        push 3802
        push LoveMsg
        push 15
        push 31
        push '!'
        push 100
        push 3802
        push LoveMsg
        push DedMsg
        call MyPrintf
        
        add rsp, 8 * 13;   
        
        push LoveMsg
        call MyPrintf
        
        add rsp, 8
        
        push rax
        push TestMsg
        call MyPrintf

        add rsp, 8 * 2
        
        push TestMsg1
        call MyPrintf
        
        add rsp, 8
        
        push rax
        push TestMsg2
        call MyPrintf
        
        add rsp, 8*2
        
        mov rax, 0x3C      ; exit64 (rdi)
        xor rdi, rdi
        syscall


MyPrintf:

        push rbp
        mov rbp, rsp
        push rbx
        push r10

        mov r8, [rbp + 8 * 2]
        lea r9, [rbp + 8 * 3]
        
        xor rdi, rdi
        xor r11, r11
        
        mov rsi, StringBuffer
        mov r10, DigitsBuffer + digit_buffer_size - 1
        
PrintfLoop:

        cmp byte [r8], '%'         ; checking for %   
        je VarProc
        
        cmp byte [r8], 0           ; checking for end of format string
        je PrintfEnd  
                                
        WriteToStrBuf [r8]         ; copy symbol
        
Increment:  

        inc r8                     ; mov to next symbol  
        jmp PrintfLoop
        
VarProc: 

        inc r8                     ; checking kind of variable 

        cmp byte [r8], 'b'         
        jb ParamType
        
        cmp byte [r8], 'x'
        ja ParamType
        
        xor rcx, rcx
        
        mov cl, byte [r8]          ; calculate switch table
        sub cl, 'b'
        shl cl, 3
        
        jmp [rcx + JumpTable]  ; jumping to switch table

Symbol:
        
        WriteToStrBuf [r9] ; copy symbol
        
        jmp VarProcEnd
;=================================================================================================================
String:
        
        mov rbx, [r9] ; copy string ptr from stack
        
StringLoop:
        
        cmp byte [rbx], 0          ; copy symbols from string till '\0'
        je VarProcEnd
        
        WriteToStrBuf [rbx]
        
        inc rbx ; move to next symbol
        jmp StringLoop
;===================================================================================================================
Integer:

        mov eax, dword [r9] ; copy integer from stack
        
        mov rcx, 10
        
IntegerLoop:

        dec r10             ; r10 - pointer of end Digits buffer       
        mov byte [r10], "0" 
                
        xor rdx, rdx
        div rcx
        
        add byte [r10], dl  ; save digit in Digits buffer
        
        or rax, rax
        
        jnz IntegerLoop 
        
        call WriteToDigitBuffer                  
        
        jmp VarProcEnd

;===================================================================================================================

Octal:

        mov cl, 3            ; 2^3 is base of the number system
        
        jmp NumbProc

;===================================================================================================================

Hexademical:

        mov cl, 4            ; 2^4 is base of the number system

;===================================================================================================================

NumbProc:
        
        mov eax, dword [r9]  ; copy integer in stack
        
        call TranslateNumber
        
        call WriteToDigitBuffer

        jmp VarProcEnd

Binary:
        
        mov ecx, dword [r9]  ; copy integer in stack
        
BinaryLoop:                        
        
        dec r10              ; r10 - pointer of end Digit buffer
        
        mov byte [r10], "0"
        shr ecx, 1
        jnb BinaryInc
        
        inc byte [r10]
        
BinaryInc:

        or ecx, ecx
        jnz BinaryLoop ; translate number while not zero in ecx
        
        call WriteToDigitBuffer
        
VarProcEnd:

        add r9, 8              ; moving to next arg in stack
        jmp Increment

;===================================================================================================================
        
ParamType:

        dec r8                 ; move to %
        
        WriteToStrBuf [r8]     ; copy %

        inc r8                 ; move to printf parameters
        
        cmp byte [r8], '%'
        je Increment
        
        cmp byte [r8], 0
        je PrintfEnd
        
        WriteToStrBuf [r8]
        
        jmp Increment
        
Incorrect:

        dec r8                  ; move to %
        WriteToStrBuf [r8]      ; copy %
        
        inc r8                  ; move to incorrect param
        WriteToStrBuf [r8]      ; copy incorrect param
        
        jmp Increment
        
PrintfEnd:

        mov r10, rdi            ; saving num in str buffer
        
        call SystemWrite
        
        mov rdx, r11            ; move num of symbols needed to be printed
        
        syscall                 ; write64
        
        mov rax, r10
        shl rax, 9
        add rax, rdx             ; rax is num of printed symbols
        
        pop r10
        pop rbx
        
        mov rsp, rbp
        pop rbp
        
        ret

SystemWrite:

;------------------------------------------------
;Syscall write


;Entry: Non
;
;Exit:  rax, rdi for next syscall write using your rsi and rdx just by typing 'syscall'
;
;Non const: rax, rdi
;------------------------------------------------

        mov rax, 0x01
        mov rdi, 1

        ret

CopyToStrBuf:

;------------------------------------------------
;Copt char to str buffer

;Entry: cl  = symbol need to be moved
;       rdi = num of writings from buffer
;       r11 = offset inside buffer           
;
;Exit:  rdi = num of writings from buffer
;       r11 = new offser insise buffer
;
;Non const: rax, rbx, rcx, rdx, rdi, r11

;------------------------------------------------

        mov byte [r11 + StringBuffer], cl ; save symbol in cl to str buffer
                        
        inc r11                                    ; increase offset inside buffer
                        
        cmp r11, string_buffer_size                       
        jne CopyToStrBufReturn                  ; check buffer overflow
                
        mov rbx, rdi                                ; save rdi in rbx
        mov rdx, r11
        
        call SystemWrite
                        
        syscall                     ; sys write64
                        
        mov rdi, rbx 
        inc rdi                     ; increase number of full writes from str buffer
        
        xor r11, r11 ; offset inside buffer after write64

CopyToStrBufReturn:           

        ret

WriteToDigitBuffer:

;------------------------------------------------
;Copies char from Digit buffer to str buffer

;Entry: r10 = addres of Digit buffer
;
;Exit:  r10 = addres of first '\0' in Digit buffer
;
;Non const: rax, rbx, rcx, rdx, rdi, r10, r11
;------------------------------------------------

        cmp byte [r10], 0 ; checking for '\0'
        je WriteToDigitBufferExit
                
        WriteToStrBuf [r10] ; r10 is pointer to the end of Digit buffer
                        
        inc r10
        
        jmp WriteToDigitBuffer 

WriteToDigitBufferExit:

        ret                 

TranslateNumber:

;------------------------------------------------
;Translates number, copies it into Digit buffer

;Entry: eax = number needed to be transleted
;       cl  = x, when 2^x is base of number system
;       r10 = pointer on Digit buffer
;
;Exit:  r10 = pointer on translated number
;
;Non const: ebx, rdx, r10
;------------------------------------------------

        mov rdx, 1
        shl rdx, cl
        dec rdx        ; rdx bit mask
        
TranslateNumberLoop:

        dec r10        ; r10 - pointer of end Digit buffer
        
        mov ebx, eax   ; copy number
        and ebx, edx   ; get first bits
        
        mov bl, byte [ebx + SymbolsBuffer] ; copy digit
        
        mov byte [r10], bl ; save digit
        
        shr eax, cl
        jnz TranslateNumberLoop
        
        ret

section .data            

JumpTable:

                    dq Binary           ;%b
                    dq Symbol           ;%c
                    dq Integer          ;%d              
times 'o' - 'd' - 1 dq Incorrect
                    dq Octal            ;%o
times 's' - 'o' - 1 dq Incorrect
                    dq String           ;%s
times 'x' - 's' - 1 dq Incorrect
                    dq Hexademical      ;%x

StringBuffer:
        resb string_buffer_size

DigitsBuffer:
        resb digit_buffer_size

SymbolsBuffer:
        db "0123456789ABCDEF"

DedMsg:      db "I %s %x%d%%%c%b  %o %~", 0x0a, "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234!!!abcdefghijklmnopqrstuvwxyz", 0x0a, "I %s %x%d%%%c%b  %o %!", 0x0a, 0

LoveMsg:     db "love", 0

TestMsg:     db " == %d2", 0x0a, 0

TestMsg1:    db "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234", 0 ;516

TestMsg2:    db " = %d", 0x0a, 0 
