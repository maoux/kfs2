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

; .again:
; 	in		al, 64h
; 	test	al, 1
; 	jz		short .nokey
; 	test	al, 20h
; 	jnz		short .nokey
; 	mov		edx, eax
; 	in		al, 60h

; 	push	edx
; 	push	eax

; 	extern print_key
; 	call print_key

; .nokey:
; 	jmp .again

; 	cli

.hang:
	hlt
	jmp .hang
.end:
