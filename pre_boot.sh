nasm boot.asm -o boot.bin
dd if=/dev/zero of=empty.img bs=512 count=2880
dd if=boot.bin of=SlardarOS.img bs=512 count=1
dd if=empty.img of=SlardarOS.img skip=1 seek=1 bs=512 count=2879
rm empty.img
rm boot.bin