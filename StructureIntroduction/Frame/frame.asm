.model tiny

;const	
;--------------------
video_segment 	    equ 0b800h

symbol		    equ 9e00h
color_symbol	    equ 27h 

left_up_corner	    equ 2ec9h
left_down_corner    equ 2ec8h

right_up_corner     equ 2ebbh
right_down_corner   equ 2ebch

vertical_line       equ 2ebah
horizontal_line     equ 2ecdh

x1		    		equ 15     ;horizontal lengh
x2                  equ 70

y1                  equ 5      ;vertical lenght
y2                  equ 20																																																																									 
;--------------------

.code
org 100h

Start:                  
		call DrawRactangle                		
		call DrawFrame
		call Getstr
		call Atoi

		mov cl, 1d							 ;parametrs for bin, delim
		mov di, 1d 							 ;logical and
		call Converter
		mov di, ((y1 + 1) * 80 + x1 + 7) * 2 ;parametr for video seg
		call Printf
		mov di, ((y1 + 1) * 80 + x1 + 2) * 2
                mov bx, offset msg_bin
		call Notation

		mov cl, 4d							 ;parametrs for hex, delim
		mov di, 15d 						 ;logical and
		call Converter
		mov di, ((y1 + 4) * 80 + x1 + 7) * 2 ;parametr for video seg
		call Printf
		mov di, ((y1 + 4) * 80 + x1 + 2) * 2
                mov bx, offset msg_hex
		call Notation

		mov cl, 3d							 ;parametrs for hex, delim
		mov di, 7d 							 ;logical and
		call Converter
		mov di, ((y1 + 7) * 80 + x1 + 7) * 2
		call Printf
		mov di, ((y1 + 7) * 80 + x1 + 2) * 2
                mov bx, offset msg_oct
		call Notation

		int 16h

		mov ax, 4c00h
		int 21h
;-------------------------------------------
;Function draws a rectangle by your coordinates
;and filling by your symbol
;
;DrawWidth - filling width
;DrawHeight - filling height
;-------------------------------------------
DrawRactangle proc

		mov ax, video_segment     ;first video segment
		mov es, ax
		mov ax, symbol                  ;push symbol

		mov di, (y1 * 80 + x1) * 2      ;first segment
		mov cx, x2 - x1                 ;Width
		mov bx, y2 - y1                 ;Height
		mov si, 0d	                    ;if si == bx -> exit
																																											
Width:
		stosw                           ;push symbol in video seg and di +=2
		call Sleep	
		loop Width													        																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						
		

;<draw horizontal filling>
Height:        
	        add di, (80 - (x2 - x1)) * 2    ;first index of video segment
       	        inc si
       	        mov cx, x2 - x1                 ;width
       	        cmp si, bx 
		call Sleep
		jne Width
																																																		  			
ret
endp
;-------------------------------------------
; This function draws a frame around rectangle
;
; First block put left_up_corner
; Second block draw horizontal_line by width
; Third block put right_up_corner
; Fourth block prepare to draw vertical lines
; DrawVertical - draw vertical line around 
; rectangle
;-------------------------------------------
DrawFrame proc
		mov di, (y1 * 80 + x1 - 81) * 2 ;first video segment
		mov ax, left_up_corner	
		stosw                           ;left up corner draw

		mov ax, horizontal_line         ;horizontal line draw
		mov cx, x2 - x1
		rep stosw

		mov ax, right_up_corner         ;right up corner draw
		stosw

		mov di, (y1 * 80 + x1 - 1) * 2  ;vertical line draw
		mov ax, vertical_line
	       	mov si, 1d
VerCounter:    
		mov cx, y2 - y1                 ; counter for vertical                                  
Vertical:       
		stosw                           ;draw vertical line
                add di, 79d * 2  
		call Sleep                      
                loop Vertical                                    
                                           
                mov di, (y1 * 80 + x2) * 2      ;index
                inc si                          ;si += 1                 
                cmp si, 2d 
		call Sleep           
                je  VerCounter      

                mov ax, left_down_corner        ;draw left corner    
                mov di, (y2 * 80 + x1 - 1) * 2
                stosw

                mov ax, horizontal_line         ;draw horizontal line
                mov cx, x2 - x1
                rep stosw                       ;repeat push
                                       
                mov ax, right_down_corner       ;draw right corner
                stosw
ret 
endp            

;<sleep function Draw a delay>                         
Sleep proc
		push ax cx dx					;save geristers
		mov ah, 86h          			;wait int
		mov cx, 0h						;delay 
		mov dx, 0480h					;in mlseconds
		int 15h
		pop dx cx ax
ret
endp
;-------------------------------------------
;Getstr scan a string to free memory by one symbol
;
;Entry parameters:
;- no entry params
;
;Constant:
;ah - int for keyboard scaning
;
;Changed:
;bx - offset of free memory
;-------------------------------------------
Getstr proc
		mov bx, offset free_memory		;calloc memory for number
Enter:
		mov ah, 01h						;int for keyboard scaning
		int 21h
		mov [bx], ax					;get symbol
		inc bx
		cmp al, 0ah						;scan symbols while not ASKI code of enter
		jne Enter						
ret
endp		
;-------------------------------------------
;Atoi function convert symbols(ASCI code) in to //caller save: list of non const 
;numbers (get numbers)
;in al non const - number -> mul 10 and + next number
;
;Entry parameters:
;- no entry param
;
;Changed:
;bx - offset of free memory
;dl - temp variable for one symbol
;-------------------------------------------
Atoi proc        
                xor ax, ax       
                mov bx, offset free_memory		;get offset of our number
                mov al, [bx]
               	sub al, '0'						;sub ASKI code of zero
                inc bx 

GetNumb:        
		mov dx, 10d						;put 10 in dx
		mul dx							;and mul ax on 10

		xor dh, dh	
		mov dl, [bx]					;put symbol in dl
		sub dl, '0'						;sub ASKI code of zero

		add ax, dx						;add new symbol with number

		inc bx
		cmp byte ptr [bx], 0ah			;do while not ASKI code of enter
		jne GetNumb	
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
Converter proc
		pop bp							;pop bp - Converter exit code

		mov dx, ax						;copy ax in dx and work with dx
		mov si, 0d						;number counter
Division:	
		;int 3h
		mov bx, dx						;copy dx in bx 
		and bx, di						;di - parametr, do logical and with di parametr
		push bx 						;save remainder in stack
		inc si							;count of numeral
		shr dx, cl						;cl - parametr, logical mov on cl parametr
		cmp dx, 0d
		jne Division

		push bp							;push exit code of Converter
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
		mov cx, si						;copy si(counetr of numerals) in cx for loop
Output:		
		pop ax							;pop first numeral from stack
		mov ah, 1ah						;color 
		cmp al, 10d						;if al > 10 -> like letter other like number
		jb Numb

		;<Draw a letter>
		add al, 65d						;ASKI code 'A'			
		sub al, 10d						;sub 10
		jmp Write

Numb:		add al, '0'						;add ASKI code of zero

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
;This function write msg like (BIN:) int the frame
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

.data
msg_bin 	db 'BIN:$'
msg_hex		db 'HEX:$'	
msg_oct 	db 'OCT:$'

free_memory:

																																																																																																										
end Start 
