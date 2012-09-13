PROJ	=LDF3
DEBUG	=0
CC	=qcl
CFLAGS_G	= /AS /W2 /Za 
CFLAGS_D	= /Zi /Zr /Gi$(PROJ).mdt /Od 
CFLAGS_R	= /O /Ot /Gs /DNDEBUG 
CFLAGS	=$(CFLAGS_G) $(CFLAGS_R)
LFLAGS_G	= /CP:0xffff /NOI /SE:0x80 /ST:0xbec 
LFLAGS_D	= /CO /INCR 
LFLAGS_R	= 
LFLAGS	=$(LFLAGS_G) $(LFLAGS_R)
RUNFLAGS	=
OBJS_EXT = 	
LIBS_EXT = 	

all:	$(PROJ).exe

ldf3.obj:	ldf3.c

ldffont.obj:	ldffont.c

ldfbmap.obj:	ldfbmap.c

ldflform.obj:	ldflform.c

ldftxt.obj:	ldftxt.c

ldfhead.obj:	ldfhead.c

ldffile.obj:	ldffile.c

$(PROJ).exe:	ldf3.obj ldffont.obj ldfbmap.obj ldflform.obj ldftxt.obj ldfhead.obj \
	ldffile.obj $(OBJS_EXT)
	echo >NUL @<<$(PROJ).crf
ldf3.obj +
ldffont.obj +
ldfbmap.obj +
ldflform.obj +
ldftxt.obj +
ldfhead.obj +
ldffile.obj +
$(OBJS_EXT)
$(PROJ).exe

$(LIBS_EXT);
<<
	link $(LFLAGS) @$(PROJ).crf

run: $(PROJ).exe
	$(PROJ) $(RUNFLAGS)

