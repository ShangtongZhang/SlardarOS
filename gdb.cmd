set architecture i8086
target remote localhost:1234
#display /10i $pc
display /h $cs
display /h $eip
display /10i $cs * 16 + $eip
b *0x7d14