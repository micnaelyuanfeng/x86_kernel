BS_OEMName			DB		'BCFYOS01' ;Must be 8 bytes
BPB_BytePerSec		DW		512
BPB_SecPerClus		DB		1
BPB_RsvdSecCnt		DW		1
BPB_NumFATs			DB		2
BPB_RootEntCnt		DW		224
BPB_TotSec16		DW		2880
BPB_Media			DB		0xF0
BPB_FATSz16			DW		9
BPB_SecPerTrk		DW		18
BPB_NumHeads		DW		2
BPB_HiddSec			DD		0
BPB_TotSec32		DD		0
BS_DrvNum			DB		0
BS_Reserved1		DB		0
BS_BootSig			DB		0x29
BS_VolID			DD 		0
BS_VolLab			DB		'Kernel_impl'
BS_FileSysType		DB		'FAT12'

DeltaSectorNo        	equ    	17          ; sector offset to address loading area
RootDirSectors		 	equ		14 			; changable but usually fixed
SectNoOfRootDir	     	equ		19      	; default RootDirSectors start sector number
SectorNoOfFAT1       	equ		1       	; sector number of FAT1
