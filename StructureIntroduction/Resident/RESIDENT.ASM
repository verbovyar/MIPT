.model tiny

;const
;----------------------------//array
keyboard_port       equ 60h
tilda_scan_code     equ 29h

video_segment       equ 0b800h

symbol		   		equ 9e00h
color_symbol	    equ 27h 

left_up_corner	    equ 2ec9h
left_down_corner    equ 2ec8h

right_up_corner     equ 2ebbh
right_down_corner   equ 2ebch

vertical_line       equ 2ebah
horizontal_line     equ 2ecdh

green_color_code	equ	1ah

term_symbol			equ 10d

x1                  equ 65d
x2                  equ 79d
y1                  equ 2d
y2                  equ 14d
;----------------------------

.code
org 100h

Start:

                ; make function
                xor bx, bx
                mov es, bx
                ;const
                mov bx, 9 * 4                   ; interruption 1 (and each address is 4 bytes)

                ;<save old 09h int>
                mov ax, es:[bx]
                mov oldKbdIntOffset, ax
                mov ax, es:[bx + 2]
                mov oldKbdIntSeg, ax
                
                cli                               ; clear IF and set 0

                mov es:[bx], offset kbdIntercept
                mov ax, cs
                mov es:[bx + 2], ax

                sti                                ; set int to 1

                ;<Terminating the program and leaving code and data in memory>
                mov ax, 3100h                    ; al = exit code
                mov dx, offset PROGRAM_END       ; dx = memory  
                shr dx, 4                        ;16-byte paragraphs
                inc dx                           ;if program mod 16 != 0, then save
                                                 ;one extra paragraph 
                int 21h
;----------------------------------------
;This function change standart keyboard int
;and write regs in hex if pushed tilda ASCCI code
;
;Functions: DrawRactangle, DrawFrame -
;draw Frame     
;----------------------------------------
kbdIntercept:
                push ds ax

                mov ax, cs
			    mov ds, ax

                in al, keyboard_port              ;save recent scan code
                
                cmp al, tilda_scan_code           ;if tilda scan code, than jmp
                jne endInt

                pop ax
                call SaveRegs
                push ax

                call DrawRactangle                 ;Draw ractangle
                call DrawFrame                     ;Draw frame

                mov di, ((y1 + 1) * 80 + x1 + 2) * 2
                mov bx, offset rax_msg
                mov dx, ((y1 + 1) * 80 + x1 + 7) * 2
                mov ax, [registers]
                call WriteReg

                mov di, ((y1 + 4) * 80 + x1 + 2) * 2
                mov bx, offset rbx_msg
                mov dx, ((y1 + 4) * 80 + x1 + 7) * 2
                mov ax, [registers + 2]
                call WriteReg

                mov di, ((y1 + 7) * 80 + x1 + 2) * 2
                mov bx, offset rcx_msg
                mov dx, ((y1 + 7) * 80 + x1 + 7) * 2
                mov ax, [registers + 4]
                call WriteReg

                mov di, ((y1 + 10) * 80 + x1 + 2) * 2
                mov bx, offset rdx_msg
                mov dx, ((y1 + 10) * 80 + x1 + 7) * 2
                mov ax, [registers + 6]
                call WriteReg

                call RestoreRegs

endInt:         pop ax ds

                db 0eah                             ;JMP opcode

oldKbdIntOffset dw 0
oldKbdIntSeg    dw 0

endp
;----------------------------------------
;This function cinvert regs in hex and write regs
;
;Entry parameters:
;ax - content of reg
;di - starting point for writing notation
;dx - starting point for writing reg
;bx - offset of reg msg
;
;ax - const
;di - non const
;dx - non const
;----------------------------------------
WriteReg proc
                call Notation

                mov cl, 4d							 ;parametrs for bin, delim
                mov ch, 15d
                call Convert

                mov di, dx                           ;parametr for video seg
                mov dl, green_color_code
				call Printf
ret
endp 
;-------------------------------------------
;This function write msg like (RAX:) int the frame
;
;Entry parametrs:-
;di - starting point for writing notation
;bx - offset of reg msg
;-------------------------------------------
Notation proc
                mov cl, byte ptr [bx] 
Msg:
				mov byte ptr es:[di], cl
				mov es:[di + 1], 12h		;const
				inc bx
				inc di
				inc di
				mov cl, byte ptr [bx]
				cmp cl, '$'
				jne Msg
ret
endp

;-------------------------------------------
;Convert - translate your number in to 2 pow
;notation
;
;Entry parametrs:
;di - parametr, do logical and with di parametr
;cl - parametr, logical mov on cl parametr
;
;ax - const, our number
;bx, dx - non const, copies of ax
;from the bx we get the remainder
;from the dx we get the quotient
;remainder push in to stack
;-------------------------------------------
Convert proc
				pop bp							;pop bp - Convert exit code
                
                xor bh, bh
                rept 4
				mov bl, al						;copy dx in bx 
				and bl, ch						
				push bx 						;save remainder in stack

				shr ax, cl						
                endm 

				push bp							;push exit code of Convert
ret
endp

;-------------------------------------------
;Printf function output your number in notation
;
;Entry parameters:
;cx - non const, counter for loop(count of numerals)
;ax - non const, number
;es - video segment
;if numeral > 10 in notation -> draw letter
;or draw numeral (jmp on Write)
;-------------------------------------------
Printf proc
				pop bp							;pop bp - printf exit code
				mov bx, ax						;copy ax in bx
				mov cx, 4d						;copy si(counetr of numerals) in cx for loop
Output:		
				pop ax							;pop first numeral from stack
				mov ah, dl 		                ;color 
				cmp al, term_symbol				;if al > 10 -> like letter other like number
				jb Numb

				;<Draw a letter>
				add al, 'A'						;ASCII code 'A'			
				sub al, term_symbol				;sub 10
				jmp Write

Numb:			add al, '0'						;add ASCII code of zero

Write:		
				mov byte ptr es:[di], al		;put numeral
				mov byte ptr es:[di + 1], ah	;color
				inc di
				inc di
				loop Output

				mov ax, bx						
				push bp							;pop bp - printf exit code
ret
endp
;-------------------------------------------
;SaveRegs and RestoreRegs - save regs in registers 
;and remove it
;-------------------------------------------
SaveRegs proc
                mov (registers),      ax
                mov (registers + 2),  bx
                mov (registers + 4),  cx
                mov (registers + 6),  dx
                mov (registers + 8),  es
                mov (registers + 10), si
                mov (registers + 12), di
                mov (registers + 14), bp
ret
endp

RestoreRegs proc
                mov ax, (registers)
                mov bx, (registers + 2)
                mov cx, (registers + 4)
                mov dx, (registers + 6)
                mov es, (registers + 8)
                mov si, (registers + 10)
                mov di, (registers + 12)
                mov bp, (registers + 14)
ret
endp

.data
rax_msg        db 'RAX:$'
rbx_msg        db 'RBX:$'
rcx_msg        db 'RCX:$'
rdx_msg        db 'RDX:$' 

registers      dw 8 dup(0) 

include DrawFrame.asm

PROGRAM_END:

end Start
        