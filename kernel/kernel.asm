[section .text]
global _start
_start:
	mov	ax, 0b800h
	mov	gs, ax
	mov	ah, 0Fh
	mov	al, 'K'
	mov	[gs:((80 * 0 + 39) * 2)], ax
	jmp	$
