# Microsoft Visual C++ generated build script - Do not modify

PROJ = LDF
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = c:\rjf\meng\gdpb\test\t1.txt
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\RJF\MENG\GDPB\CODE\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = LDF.C       
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W3 /Zi /AM /O1 /D "_DEBUG" /D "_DOS" /FR /Fd"LDF.PDB"
CFLAGS_R_DEXE = /nologo /Gs /W3 /AM /O1 /Ox /D "NDEBUG" /D "_DOS" 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE 
LIBS_D_DEXE = oldnames mlibce 
LIBS_R_DEXE = oldnames mlibce 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = LDF.SBR \
		LDFFONT.SBR \
		LDFTXT.SBR \
		LDFLFORM.SBR \
		LDFBMAP.SBR \
		LDFHEAD.SBR \
		LDFFILE.SBR


LDF_DEP = c:\rjf\meng\gdpb\code\ldf.h


LDFFONT_DEP = c:\rjf\meng\gdpb\code\ldf.h


LDFTXT_DEP = c:\rjf\meng\gdpb\code\ldf.h


LDFLFORM_DEP = c:\rjf\meng\gdpb\code\ldf.h


LDFBMAP_DEP = c:\rjf\meng\gdpb\code\ldf.h


LDFHEAD_DEP = c:\rjf\meng\gdpb\code\ldf.h \
	c:\rjf\meng\gdpb\code\dirs.h


LDFFILE_DEP = c:\rjf\meng\gdpb\code\ldf.h \
	c:\rjf\meng\gdpb\code\dirs.h


all:	$(PROJ).EXE

LDF.OBJ:	LDF.C $(LDF_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c LDF.C

LDFFONT.OBJ:	LDFFONT.C $(LDFFONT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LDFFONT.C

LDFTXT.OBJ:	LDFTXT.C $(LDFTXT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LDFTXT.C

LDFLFORM.OBJ:	LDFLFORM.C $(LDFLFORM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LDFLFORM.C

LDFBMAP.OBJ:	LDFBMAP.C $(LDFBMAP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LDFBMAP.C

LDFHEAD.OBJ:	LDFHEAD.C $(LDFHEAD_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LDFHEAD.C

LDFFILE.OBJ:	LDFFILE.C $(LDFFILE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LDFFILE.C

$(PROJ).EXE::	LDF.OBJ LDFFONT.OBJ LDFTXT.OBJ LDFLFORM.OBJ LDFBMAP.OBJ LDFHEAD.OBJ \
	LDFFILE.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
LDF.OBJ +
LDFFONT.OBJ +
LDFTXT.OBJ +
LDFLFORM.OBJ +
LDFBMAP.OBJ +
LDFHEAD.OBJ +
LDFFILE.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
c:\msvc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
