/***************************************************************************
 *      1989/90 Group B Design Project : Laser Information Display System  *
 *                                                                         *
 *      Message Display Control Software                                   *
 *                                                                         *
 *      dtest8.c  :  The latest (all singing, all dancing) version         *
 *                                                                         *
 *      R S Barker     28/11/89    20/4/90                                 *
 ***************************************************************************/


#include "header.h"
#include <graph.h>

F_HEADER dlist[MAXFILES];                               /* The display list */
char ctrlwd;                                            /* Printer control port byte */

void main(void)
{
    struct find_t find;                                 /* For finding filenames in directory */
    time_t reftime,newtime;                             /* Time variables (seconds since 0:00 on 1/1/70) */
    struct tm *bdnewtime;                               /* Broken down time (into years, months, days etc) */
    int listlength,                                     /* Length of display list */
	count;                                          /* Bog standard counter */


    timezone=0;                                         /* Use local time */
    printf("This system uses LOCAL time\n\n");
    system("date");                                     /* Request current date from user */
    system("time");                                     /* Request current time from user */
    ctrlwd = BLANK & LOSTROBE & LASERON;                /* Set initial control port byte */
    outp(CTRLPORT,ctrlwd);                              /* and output */

    restore_files();                                    /* Return any M*.BAK files to M*.LDM */

    for(;;)                                             /* Main program loop (infinite) */
	{
	    _clearscreen(0);
	    getmail();                                  /* Log on to VAX, send acknowledgements and return with any */
							/* new messages and deletion instructions from gdpb mailbox */

	    time(&reftime);                             /* Obtain current time */
	    listlength = create_list(reftime);          /* Compile display list */
	    for(count=0; count<listlength; count++)
		printf("Message %d : \t%s\t ends at time = %ld\n",count,dlist[count].name,dlist[count].endtime);
	    do
		{
		    display(listlength);                /* Repeat display of whole list */
		    time(&newtime);                     /* for at least a minimun time (LISTSECS) */
		}
	    while(newtime-reftime < LISTSECS);

	    bdnewtime=localtime(&newtime);
	    if(bdnewtime->tm_hour >= EVENINGHR)         /* Evening closedown */
		close_down();
	}
    exit(0);
}                                                       /* End of main */
