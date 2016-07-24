#encoding=utf-8
BUILD_DIR = 'generated/'
dumpFile = open(BUILD_DIR + 'bootLoader.dump')
gdbCmdFile = open(BUILD_DIR + 'gdb.cmd', 'w')
# gdbCmdFile.write('set architecture i8086\n')
gdbCmdFile.write('set architecture i386\n')
# gdbCmdFile.write('file ./generated/bootLoader.o\n')
gdbCmdFile.write('file ./generated/kernel.bin\n')
gdbCmdFile.write('target remote localhost:1234\n')
# gdbCmdFile.write('display /h $cs\n')
# gdbCmdFile.write('display /h $eip\n')
# gdbCmdFile.write('display /i $cs * 16 + $eip\n')
# gdbCmdFile.write('display /xh $eax\n')
# gdbCmdFile.write('display /xh $ebx\n')
# gdbCmdFile.write('display /xh $ebp\n')
# gdbCmdFile.write('display /xh $esp\n')
# gdbCmdFile.write('b *0x7d14\n')
# gdbCmdFile.write('b *0x9116\n')



addr = None
for line in dumpFile.readlines():
    pos = line.find('<protectModeCode>:')
    if pos >= 0:
        addr = line[0:pos].strip()
        break

# gdbCmdFile.write('b *0x' + addr + '\n')
gdbCmdFile.write('b kernelMain\n')
# gdbCmdFile.write('b memoryManage.cpp:29\n')
gdbCmdFile.write('display /i $eip\n')
# gdbCmdFile.write('b *0x92ab\n')
# gdbCmdFile.write('display /i 0x100000 + $eip\n')
# gdbCmdFile.write('display /i 0x' + addr + ' + $eip\n')
dumpFile.close()
gdbCmdFile.close()