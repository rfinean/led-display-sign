/***************************************************************************
 * Laser Display : Standard Message Inclusion Subroutine  :  addfile8.c    *
 *                                                                         *
 * Copies a standard message to the temporary display message directory.   *
 * If successful, appends the message to the low priority list.            *
 * End time is set to the current time so the message will not be included *
 * in subsequent display lists (unless specifically added in again).       *
 *                                                                         *
 *      R S Barker      20/3/90    7/4/90                                  *
 ***************************************************************************/


#include "header.h"

void add_file(fname,list,fnum,reftime)

char *fname;                                            /* Name of standard file */
F_HEADER list[20];                                      /* Low priority sub-list */
int *fnum;                                              /* Number of entries in sub-list */
time_t reftime;                                         /* Reference time */

{
FILE *finp,*foutp;                                      /* Input and output files */
int c;                                                  /* Bog standard variable */
char stdname[FPATHLNG],tmpname[FPATHLNG];               /* Filename with paths prefixed */


    sprintf(stdname,"%s%s",STDFDIR,fname);              /* Concatenate path and filename */
    if((finp=fopen(stdname,RACCESS))!=(FILE *)NULL)
	{                                               /* Try to copy file to C:\ directory for use by display routine */
	    sprintf(tmpname,"%s%s",TMPDIR,fname);
	    if((foutp=fopen(tmpname,WACCESS))!=(FILE *)NULL)
		{
		    while((c=getc(finp))!=EOF)
			putc(c,foutp);
		    fclose(foutp);
		    strcpy(list[*fnum].name,fname);     /* Append to low priority list */
		    list[*fnum].endtime=reftime;        /* using current time as end time */
		    (*fnum)++;
		    printf("Standard file added : low priority total %d\n",*fnum);
		}
	    else                                        /* Can't open duplicate file */
		{
		    printf("addfile8 : can't open duplicate %s\n",fname);
		}
	    fclose(finp);
	}
    else                                                /* Unable to open original file */
	{
	    printf("addfile8 : can't open original %s\n",fname);
	    remove(stdname);
	}
    return;
}
