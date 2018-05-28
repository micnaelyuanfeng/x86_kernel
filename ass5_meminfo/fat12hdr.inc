BS_OEMName	            DB 'BCFYKRNL'	

BPB_BytsPerSec	        DW 512		
BPB_SecPerClus	        DB 1		
BPB_RsvdSecCnt	        DW 1		
BPB_NumFATs	            DB 2	
BPB_RootEntCnt	        DW 224		
BPB_TotSec16	        DW 2880		
BPB_Media	            DB 0xF0	
BPB_FATSz16	            DW 9		
BPB_SecPerTrk	        DW 18		
BPB_NumHeads	        DW 2		
BPB_HiddSec	            DD 0		
BPB_TotSec32	        DD 0		

BS_DrvNum	            DB 0		
BS_Reserved1	        DB 0		
BS_BootSig	            DB 29h		
BS_VolID	            DD 0		
BS_VolLab	            DB 'kernel_v01'
BS_FileSysType	        DB 'FAT12   '	


; RootDirSectors = ((BPB_RootEntCnt*32)+(BPB_BytsPerSec–1))/BPB_BytsPerSec
RootDirSectors		    equ	14

; Root Directory first Sector Bo	= BPB_RsvdSecCnt + (BPB_NumFATs * FATSz)
SectorNoOfRootDirectory	equ	19

; FAT1 的第一个扇区号	= BPB_RsvdSecCnt
SectorNoOfFAT1		    equ	1

; DeltaSectorNo = BPB_RsvdSecCnt + (BPB_NumFATs * FATSz) - 2
; 文件的开始Sector号 = DirEntry中的开始Sector号 + 根目录占用Sector数目
;                      + DeltaSectorNo
DeltaSectorNo		    equ	17
