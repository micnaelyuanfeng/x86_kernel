#############################################
#                 MakeFile                  #
#############################################
BOOT_SRC := boot.asm
LOAD_SRC := loader.asm

BOOT_BIN := $(subst .asm,.bin, $(BOOT_SRC))
LOAD_BIN := $(subst .asm,.bin, $(LOAD_SRC))


.PHONY: all

##############################################
#  Compile BootLoader and Load Kernel Image  #
##############################################
all: $(BOOT_BIN) $(LOAD_BIN) 

$(BOOT_BIN): $(BOOT_SRC)
	nasm $< -o $@

$(LOAD_BIN): $(LOAD_SRC)
	nasm $< -o $@

