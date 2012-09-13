/***************************************************************************
 * Laser Display : Message File Processing Subroutine  :  PROCF8.C         *
 *                                                                         *
 * Determines whether a specified message file is current.                 *
 * If it is out of date or the file can't be opened then it is deleted.    *
 * If current, the file is renamed *.BAK from *.LDM to avoid reprocessing. *
 * If the renamed file cannot be opened then it is deleted.                *
 * A duplicate is created in the c:\tmp directory if possible.             *
 * If successful, then the file is added to the appropriate file sub-list. *
 * If the duplicate cannot be created then the original reverts to *.LDM   *
 *                                                                         *
 *      R S Barker      6/2/90    7/4/90                                   *
 ***************************************************************************/


#include "header.h"

void process_file(fname,hlist,hfiles,llist,lfiles,reftime)

char *fname;                                            /* Name of file for processing */
time_t reftime;                                         /* Time to be used as reference */
F_HEADER hlist[LSIZE],llist[LSIZE];                     /* High and low priority file sub-lists */
int *lfiles,*hfiles;                                    /* Current totals of files in each sub-list */

{
F_HEADER temphead;                                      /* Temporary store for filename and endtime */
int c,count,count1,priority;                            /* General variables and message priority */
FILE *finp,*foutp;                                      /* Input and output files */
long start;                                             /* Message start time */
char messname[FPATHLNG],mbakname[FPATHLNG],             /* File names with paths prefixed */
     tmpbakname[FPATHLNG];

							/* Prepare back-up filename */
    for(count=0; (temphead.name[count]=*(fname+count))!=DOT; count++);
    for(count1=0; count1<4; temphead.name[++count]=BAKFEXT[count1++]);

    sprintf(messname,"%s%s",MESSDIR,fname);             /* Concatenate paths with filenames */
    sprintf(mbakname,"%s%s",MESSDIR,temphead.name);
    sprintf(tmpbakname,"%s%s",TMPDIR,temphead.name);

    if((finp=fopen(messname,RACCESS))!=(FILE *)NULL)
	{
	    fscanf(finp,"%*s\n%*s\n%ld\n%ld\n%d\n",&start,&temphead.endtime,&priority);         /* Read and store header */
	    fclose(finp);
	    printf("processing file : %s %ld %ld %d\n",fname,start,temphead.endtime,priority);
	    if (temphead.endtime<reftime)
		{                                       /* Message out of date */
		    remove(messname);
		    printf("removed %s\n",fname);
		    return;
		}

	    if (start>reftime)                          /* Message not yet current */
		return;

	    if((priority && (*hfiles>=LSIZE)) || (~priority && (*lfiles>=LSIZE)))
		return;                                 /* Sub-list already full */

	    rename(messname,mbakname);

							/* Try to copy file to C:\ directory for use by display routine */
	    if((finp=fopen(mbakname,RACCESS))!=(FILE *)NULL)
		{
		    if((foutp=fopen(tmpbakname,WACCESS))!=(FILE *)NULL)
			{
			    while((c=getc(finp))!=EOF)
				putc(c,foutp);
			    fclose(foutp);
							/* Append to appropriate sub-list */
			    if (priority)
				{
				    hlist[*hfiles].endtime=temphead.endtime;
				    strcpy(hlist[*hfiles].name,temphead.name);
				    (*hfiles)++;
				    printf("high priority file total %d\n",*hfiles);
				}
			    else
				{
				    llist[*lfiles].endtime=temphead.endtime;
				    strcpy(llist[*lfiles].name,temphead.name);
				    (*lfiles)++;
				    printf("low priority file total %d\n",*lfiles);
				}


			}
		    else                                /* Can't open duplicate file */
			{
			    printf("procf8 : can't open duplicate %s\n",temphead.name);
			    rename(mbakname,messname);
			}
		    fclose(finp);
		}
	    else                                        /* Unable to reopen file */
		{
		    printf("procf8 : can't reopen %s\n",temphead.name);
		    remove(mbakname);
		}
	}
    else                                                /* Unable to open original file */
	{
	    printf("procf8 : can't open original %s\n",fname);
	    remove(messname);
	}
    return;
}
