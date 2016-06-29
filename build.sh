mkdir generated
cd generated

BOOT_SECTOR_BIN="bootSector.bin"
BOOT_LOADER_BIN="loader.bin"
nasm ../boot/bootSector.asm -o $BOOT_SECTOR_BIN
dd if=/dev/zero of=empty.img bs=512 count=2880
dd if=$BOOT_SECTOR_BIN of=SlardarOS.img bs=512 count=1
dd if=empty.img of=SlardarOS.img skip=1 seek=1 bs=512 count=2879

nasm ../boot/bootLoader.asm -o $BOOT_LOADER_BIN
hdiutil attach SlardarOS.img
cp $BOOT_LOADER_BIN /Volumes/SlardarOS/$BOOT_LOADER_BIN
hdiutil detach /Volumes/SlardarOS