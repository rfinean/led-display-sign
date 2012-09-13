PROJ	=LASER
DEBUG	=0
CC	=qcl
CFLAGS_G	= /AS /W2 /Ze 
CFLAGS_D	= /Zi /Zr /Gi$(PROJ).mdt /Od 
CFLAGS_R	= /O /Ot /Gs /DNDEBUG 
CFLAGS	=$(CFLAGS_G) $(CFLAGS_R)
LFLAGS_G	= /CP:0xffff /NOI /SE:0x80 /ST:0xc00 
LFLAGS_D	= /CO /INCR 
LFLAGS_R	= 
LFLAGS	=$(LFLAGS_G) $(LFLAGS_R)
RUNFLAGS	=
OBJS_EXT = 	
LIBS_EXT = 	

all:	$(PROJ).exe

dtest8.obj:	dtest8.c

addfile8.obj:	addfile8.c

ldf4.obj:	ldf4.c

ldfbmap.obj:	ldfbmap.c

ldffile.obj:	ldffile.c

ldffont.obj:	ldffont.c

ldfhead.obj:	ldfhead.c

ldflform.obj:	ldflform.c

ldftxt.obj:	ldftxt.c

procf8.obj:	procf8.c

restore8.obj:	restore8.c

review8.obj:	review8.c

viewldm2.obj:	viewldm2.c

close9.obj:	close9.c

create8.obj:	create8.c

display9.obj:	display9.c

outppp9.obj:	outppp9.c

$(PROJ).exe:	dtest8.obj addfile8.obj ldf4.obj ldfbmap.obj ldffile.obj ldffont.obj \
	ldfhead.obj ldflform.obj ldftxt.obj procf8.obj restore8.obj review8.obj viewldm2.obj \
	close9.obj create8.obj display9.obj outppp9.obj $(OBJS_EXT)
	echo >NUL @<<$(PROJ).crf
dtest8.obj +
addfile8.obj +
ldf4.obj +
ldfbmap.obj +
ldffile.obj +
ldffont.obj +
ldfhead.obj +
ldflform.obj +
ldftxt.obj +
procf8.obj +
restore8.obj +
review8.obj +
viewldm2.obj +
close9.obj +
create8.obj +
display9.obj +
outppp9.obj +
$(OBJS_EXT)
$(PROJ).exe

$(LIBS_EXT);
<<
	link $(LFLAGS) @$(PROJ).crf

run: $(PROJ).exe
	$(PROJ) $(RUNFLAGS)

