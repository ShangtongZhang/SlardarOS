BS_OEMName	DB 'Slardar '	; OEM String, must be 8 bytes
BPB_BytsPerSec	DW 512
BPB_SecPerClus	DB 1
BPB_RsvdSecCnt	DW 1
BPB_NumFATs	DB 2
BPB_RootEntCnt	DW 224
BPB_TotSec16	DW 2880
BPB_Media	DB 0xF0
BPB_FATSz16	DW 9
BPB_SecPerTrk	DW 18
BPB_NumHeads	DW 2
BPB_HiddSec	DD 0
BPB_TotSec32	DD 0
BS_DrvNum	DB 0
BS_Reserved1	DB 0
BS_BootSig	DB 29h
BS_VolID	DD 0
BS_VolLab	DB 'SlardarOS  '; Name of disk, must be 11 bytes
BS_FileSysType	DB 'FAT12   ';Type of filesystem, must be 8 bytes

BaseOfLoader		equ	09000h
;OffsetOfLoader		equ	100h
OffsetOfLoader		equ	0
RootDirSectors		equ	14
DeltaSectorNo		equ	17
SectorNoOfFAT1		equ	1
SectorNoOfRootDirectory	equ	19