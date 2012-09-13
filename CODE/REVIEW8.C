/***************************************************************************
 * Laser Display : Display List Review Subroutine  :  review8.c            *
 *                                                                         *
 * Removes expired message files from display list and discs.              *
 * Tidies up the resulting display list (removes any gaps).                *
 *                                                                         *
 *      R S Barker      6/2/90    7/4/90                                   *
 ***************************************************************************/


#include "header.h"

void review_files(list,fnum,reftime)

F_HEADER list[LSIZE];                                   /* High/low priority list to be reviewed */
int *fnum;                                              /* Initial number of files on list */
time_t reftime;                                         /* Reference time */

{
int oldct,newct;                                        /* Counters for old & new lists */
struct find_t find;                                     /* File finding return variable */
char messname[FPATHLNG],                                /* File names with paths prefixed */
     tempname[FPATHLNG];

    for(oldct=newct=0; oldct<*fnum; oldct++)
	{
	    sprintf(messname,"%s%s",MESSDIR,list[oldct].name);
	    if(list[oldct].endtime<reftime)
		{                                       /* Out of date */
		    sprintf(tempname,"%s%s",TMPDIR,list[oldct].name);
		    remove(messname);                   /* Remove old file from floppy disc (B) */
		    remove(tempname);                   /* and virtual RAM disc */
		}
	    else                                        /* File still valid, check still on disk */
		if(!_dos_findfirst(messname,_A_NORMAL,&find))
		    {
			if(oldct!=newct)
			    {                           /* Move file to 1st free space on list */
				list[newct].endtime=list[oldct].endtime;
				strcpy(list[newct].name,list[oldct].name);
			    }
			newct++;
		    }
	}
    *fnum=newct;                                        /* new total number of files on list */
    return;
}
