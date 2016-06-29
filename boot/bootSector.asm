;%define	_BOOT_DEBUG_

%ifdef	_BOOT_DEBUG_
	org  0100h
%else
	org  07c00h
%endif

%ifdef	_BOOT_DEBUG_
BaseOfStack		equ	0100h
%else
BaseOfStack		equ	07c00h
%endif

BaseOfLoader		equ	09000h
OffsetOfLoader		equ	0100h
RootDirSectors		equ	14
SectorNoOfRootDirectory	equ	19

	jmp short LABEL_START
	nop

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

LABEL_START:	
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack
	
	xor	ah, ah
	xor	dl, dl
	int	13h
	
	mov	word [wSectorNo], SectorNoOfRootDirectory
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp	word [wRootDirSizeForLoop], 0
	jz	LABEL_NO_LOADERBIN
	dec	word [wRootDirSizeForLoop]
	mov	ax, BaseOfLoader
	mov	es, ax
	mov	bx, OffsetOfLoader
	mov	ax, [wSectorNo]
	mov	cl, 1
	call	ReadSector

	mov	si, LoaderFileName
	mov	di, OffsetOfLoader
	cld
	mov	dx, 10h
LABEL_SEARCH_FOR_LOADERBIN:
	cmp	dx, 0
	jz	LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR
	dec	dx
	mov	cx, LoaderFileNameLen
LABEL_CMP_FILENAME:
	cmp	cx, 0
	jz	LABEL_FILENAME_FOUND
	dec	cx
	lodsb
	cmp	al, byte [es:di]
	jz	LABEL_GO_ON
	jmp	LABEL_DIFFERENT
LABEL_GO_ON:
	inc	di
	jmp	LABEL_CMP_FILENAME

LABEL_DIFFERENT:
	and	di, 0FFE0h
	add	di, 20h
	mov	si, LoaderFileName
	jmp	LABEL_SEARCH_FOR_LOADERBIN

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
	add	word [wSectorNo], 1
	jmp	LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_LOADERBIN:
	mov	dh, 2
	call	DispStr
%ifdef	_BOOT_DEBUG_
	mov	ax, 4c00h
	int	21h
%else
	jmp	$
%endif

LABEL_FILENAME_FOUND:
	mov dh, 0
	call DispStr
	jmp	BaseOfLoader:OffsetOfLoader
	;jmp $

wRootDirSizeForLoop	dw	RootDirSectors
wSectorNo		dw	0
bOdd			db	0

LoaderFileName		db	"LOADER  BIN", 0
LoaderFileNameLen equ $ - LoaderFileName - 1
MessageLength		equ	9
BootMessage:		db	"Booting  ";
Message1		db	"Ready.   ";
Message2		db	"No LOADER";

DispStr:
	mov	ax, MessageLength
	mul	dh
	add	ax, BootMessage
	mov	bp, ax
	mov	ax, ds
	mov	es, ax
	mov	cx, MessageLength
	mov	ax, 01301h
	mov	bx, 0007h
	mov	dl, 0
	int	10h
	ret

ReadSector:
	push	bp
	mov	bp, sp
	sub	esp, 2

	mov	byte [bp-2], cl
	push	bx
	mov	bl, [BPB_SecPerTrk]
	div	bl
	inc	ah
	mov	cl, ah
	mov	dh, al
	shr	al, 1
	mov	ch, al
	and	dh, 1
	pop	bx
	mov	dl, [BS_DrvNum]
.GoOnReading:
	mov	ah, 2
	mov	al, byte [bp-2]
	int	13h
	jc	.GoOnReading

	add	esp, 2
	pop	bp

	ret

times 	510-($-$$)	db	0
dw 	0xaa55
