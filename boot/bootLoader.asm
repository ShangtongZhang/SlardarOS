mov ax, 0b800h
mov gs, ax
mov edi, (80 * 0 + 39) * 2
mov ah, 0ch
mov al, 'P'
mov [gs:edi], ax
jmp $