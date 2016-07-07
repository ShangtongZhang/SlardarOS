rm -rf generated
mkdir generated

BOOT_SECTOR_BIN="bootSector.bin"
BOOT_LOADER_BIN="loader.bin"
GCC="/usr/local/gcc-4.8.1-for-linux32/i586-pc-linux/bin/gcc"

OSX_PATH="$PATH"
LINUX_PATH="/usr/local/gcc-4.8.1-for-linux32/i586-pc-linux/bin:/usr/local/gcc-4.8.1-for-linux32/libexec/gcc/i586-pc-linux/4.8.1"

cd boot
nasm bootSector.asm -o ../generated/$BOOT_SECTOR_BIN
export PATH="$LINUX_PATH"
gcc -m32 -fno-builtin -Wall -ggdb -gstabs -nostdinc -fno-stack-protector -O0 -nostdinc -c bootLoader.S -o ../generated/bootLoader.S.o
gcc -m32 -fno-builtin -Wall -ggdb -gstabs -nostdinc -fno-stack-protector -O0 -nostdinc -c bootLoader.c -o ../generated/bootLoader.c.o

# cd ../kernel
# nasm -f elf kernel.asm -o ../generated/kernel.o
# /usr/local/gcc-4.8.1-for-linux32/i586-pc-linux/bin/ld -s -o ../generated/kernel.bin ../generated/kernel.o

export PATH="$OSX_PATH"
cd ../generated
dd if=/dev/zero of=empty.img bs=512 count=2880
dd if=$BOOT_SECTOR_BIN of=SlardarOS.img bs=512 count=1
dd if=empty.img of=SlardarOS.img skip=1 seek=1 bs=512 count=2879

export PATH="$LINUX_PATH"
ld -m elf_i386 -nostdlib -N -e start -Ttext 0x9000 bootLoader.S.o bootLoader.c.o -o bootLoader.o
objcopy -S -O binary bootLoader.o bootLoader.bin
objdump -S -D bootLoader.o > bootLoader.dump
objdump -S bootLoader.S.o > bootLoader.S.dump
objdump -S bootLoader.c.o > bootLoader.c.dump

export PATH="$OSX_PATH"
hdiutil attach SlardarOS.img
# cp kernel.bin /Volumes/SlardarOS/kernel.bin
cp bootLoader.bin /Volumes/SlardarOS/$BOOT_LOADER_BIN
hdiutil detach /Volumes/SlardarOS