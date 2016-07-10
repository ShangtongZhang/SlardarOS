set architecture i8086
target remote localhost:1234
#display /10i $pc
display /h $cs
display /h $eip
display /5i $cs * 16 + $eip
display /xh $eax
display /xh $ebx
display /xh $ebp
display /xh $esp
b *0x7d14