org 7c00h
mov ax, cs
mov es, ax
mov bp, msg
mov cx, msgLen
mov ax, 1300h
mov bx, 000fh
mov dl, 0
int 10h

msg db 'Hello world, here comes SlardarOS!', 0
msgLen equ $-msg
times 510-($-$$) db 0
dw 0aa55h 