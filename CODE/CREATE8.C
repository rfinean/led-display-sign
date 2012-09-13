/***************************************************************************
 * Laser Display : Display List Creation Subroutine  :  create8.c          *
 *                                                                         *
 * Reviews old high and low priority lists, processes any new files,       *
 * and builds the display list (adding in standard files if necessary)     *
 *                                                                         *
 *      R S Barker      6/2/90    7/4/90                                   *
 ***************************************************************************/


#include "header.h"

extern F_HEADER dlist[MAXFILES];

int create_list(reftime)

time_t reftime;                                         /* Current time on commencement of subroutine - used as reference time */

{
int count,count1;
struct find_t find;                                     /* File finding variable */
static int hfiles = 0;                                  /* Number of entries on high priority sub-list */
static int lfiles = 0;                                  /* Number of entries on low priority sub-list */
static F_HEADER hlist[LSIZE],llist[LSIZE];              /* High and low priority sub-lists */

    review_files(hlist,&hfiles,reftime);                /* Remove from both lists any */
    review_files(llist,&lfiles,reftime);                /* no longer current files */

    if(!_dos_findfirst(MESSDIR DISPFILE,_A_NORMAL,&find))       /* Search for first new message file */
	{
	    process_file(find.name,hlist,&hfiles,llist,&lfiles,reftime);
	    while(!_dos_findnext(&find))                        /* Search for more new message files */
		process_file(find.name,hlist,&hfiles,llist,&lfiles,reftime);
	}
    else
	{
	    printf("create8 : no new message files found\n");
	}

    for (count=0;count<hfiles;count++)                  /* Copy high priority files to display list */
	{
	    strcpy(dlist[count].name,hlist[count].name);
	    dlist[count].endtime=hlist[count].endtime;
	    printf("high priority : %s \t%ld\n",dlist[count].name,dlist[count].endtime);
	}

    if(hfiles > SEVERAL)                                /* Display high priority files only */
	return(hfiles);

    if(hfiles+lfiles < FEW)                             /* Add in standard low priority messages if required */
	{
	    if(!_dos_findfirst(STDFDIR BACKFILE,_A_NORMAL,&find))
		{
		    add_file(find.name,llist,&lfiles,reftime);
		    while(!_dos_findnext(&find))
			add_file(find.name,llist,&lfiles,reftime);
		}
	    else                                        /* no standard files found */
		{
		    printf("create8 : no standard files found\n");
		}
	}

    for(count1=0;count1<lfiles;count1++)                /* Copy low priority files to display list */
	{
	    strcpy(dlist[count].name,llist[count1].name);
	    dlist[count].endtime=llist[count1].endtime;
	    printf("low priority : %s \t%ld\n",dlist[count].name,dlist[count].endtime);
	    count++;
	}

    for(count1=0;count1<hfiles;count1++)                /* Repeat high priority files */
	{
	    strcpy(dlist[count].name,hlist[count1].name);
	    dlist[count].endtime=hlist[count1].endtime;
	    printf("high priority : %s \t%ld\n",dlist[count].name,dlist[count].endtime);
	    count++;
	}
    printf("Total length of display list = %d\n",count);
    return(count);                                      /* Return display list length to main program */
}
