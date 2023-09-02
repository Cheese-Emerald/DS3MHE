PROJ	=DS3MHE
DEBUG	=0
CC	=qcl
AS	=qcl
CFLAGS_G	= /AT /W1 /Za 
CFLAGS_D	= /Zi /Zr /Od 
CFLAGS_R	= /O /Ol /Gs /DNDEBUG 
CFLAGS	=$(CFLAGS_G) $(CFLAGS_D)
AFLAGS_G	= /Cx /W1 /P2 
AFLAGS_D	= /Zi /l /Sa /Sd 
AFLAGS_R	= /DNDEBUG 
AFLAGS	=$(AFLAGS_G) $(AFLAGS_D)
LFLAGS_G	= /CP:0xfff /NOE /SE:0x80 /T /ST:0x800 CRTCOM.LIB 
LFLAGS_D	= /CO 
LFLAGS_R	= 
LFLAGS	=$(LFLAGS_G) $(LFLAGS_D)
RUNFLAGS	=test.s3m
OBJS_EXT = 	
LIBS_EXT = 	

.asm.obj: ; $(AS) $(AFLAGS) -c $*.asm

all:	$(PROJ).COM

main.obj:	main.c $(H)

$(PROJ).COM:	main.obj $(OBJS_EXT)
	echo >NUL @<<$(PROJ).crf
main.obj +
$(OBJS_EXT)
$(PROJ).COM

$(LIBS_EXT);
<<
	ilink -a -e "qlink $(LFLAGS) @$(PROJ).crf" $(PROJ)

run: $(PROJ).COM
	$(PROJ) $(RUNFLAGS)

