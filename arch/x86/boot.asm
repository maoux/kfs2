;declare constant for the multiboot header
MBALIGN		equ 1 << 0
MEMINFO		equ 1 << 1
FLAGS		equ MBALIGN | MEMINFO 
MAGIC		equ 0x1BADB002
CHECKSUM	equ -(MAGIC + FLAGS)

;declare a multiboot header that marks the program as a kernel
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384
stack_top:


section .text
	global _start:function (_start.end - _start)

	extern keyboard_handler_main

_start:

	mov esp, stack_top

	push ebx
	push eax

	extern kmain
	call kmain

	cli
;--------------------------------------
; Main-Routine
.again:
	in al, 64h       ; get the status
	test al, 1       ; check output buffer
	jz short .nokey
	test al, 20h     ; check if it is a PS2Mouse-byte
	jnz short .nokey
	in al,60h       ; get the key

	push eax
	extern print_key
	call print_key
; insert your code here (maybe for converting into ASCII...)

.nokey:
	jmp .again
;--------------------------------------
; At the end
	cli

.hang:
	hlt
	jmp .hang
.end:
