OBJS := 

install : clean mount 
.PHONY  : install 

mount:
  dd
  sudo mount -o loop a.img /mnt/floppy
  sudo cp $() /mnt/floppy/ -v
  sudo umount /mnt/floppy/

clean:
  rm *.o
  rm *.bin
  rm *.img
  
 run:
  bochs -f bochsrc 
