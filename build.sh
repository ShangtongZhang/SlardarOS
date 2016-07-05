mkdir generated
cd generated

BOOT_SECTOR_BIN="bootSector.bin"
BOOT_LOADER_BIN="loader.bin"
<<<<<<< Updated upstream
nasm ../boot/bootSector.asm -o $BOOT_SECTOR_BIN
=======

cd boot
nasm bootSector.asm -o ../generated/$BOOT_SECTOR_BIN
nasm bootLoader.asm -o ../generated/$BOOT_LOADER_BIN
clang -fno-builtin -Wall -ggdb -nostdinc -fno-stack-protector -O0 -nostdinc -c bootLoader.S -o ../generated/bootLoader.S.o
clang -fno-builtin -Wall -ggdb -nostdinc -fno-stack-protector -O0 -nostdinc -c bootLoader.c -o ../generated/bootLoader.c.o

cd ../kernel
nasm -f elf kernel.asm -o ../generated/kernel.o
/usr/local/gcc-4.8.1-for-linux32/i586-pc-linux/bin/ld -s -o ../generated/kernel.bin ../generated/kernel.o

cd ../generated
>>>>>>> Stashed changes
dd if=/dev/zero of=empty.img bs=512 count=2880
dd if=$BOOT_SECTOR_BIN of=SlardarOS.img bs=512 count=1
dd if=empty.img of=SlardarOS.img skip=1 seek=1 bs=512 count=2879

<<<<<<< Updated upstream
nasm ../boot/bootLoader.asm -o $BOOT_LOADER_BIN
hdiutil attach SlardarOS.img
cp $BOOT_LOADER_BIN /Volumes/SlardarOS/$BOOT_LOADER_BIN
=======
ld -e start bootLoader.S.o bootLoader.c.o -o bootLoader.o
gobjcopy -S -O binary bootLoader.o bootLoader.bin

hdiutil attach SlardarOS.img
#cp $BOOT_LOADER_BIN /Volumes/SlardarOS/$BOOT_LOADER_BIN
cp kernel.bin /Volumes/SlardarOS/kernel.bin
cp bootLoader.bin /Volumes/SlardarOS/$BOOT_LOADER_BIN
>>>>>>> Stashed changes
hdiutil detach /Volumes/SlardarOS