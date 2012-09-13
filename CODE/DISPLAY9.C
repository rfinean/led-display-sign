/***************************************************************************
 * Laser Display : Display Subroutine File  :  display9.c                  *
 *                                                                         *
 * Displays each file in the display list in turn.                         *
 * If a file cannot be opened, it is simply skipped over.                  *
 *                                                                         *
 *      R S Barker      6/2/90    8/4/90                                   *
 ***************************************************************************/


#include "header.h"
#include <graph.h>

extern F_HEADER dlist[MAXFILES];
extern char ctrlwd;

void display(listlength)

int listlength;                                         /* Length of display list */

{
int count;
time_t tstart,tnow;
FILE *finp;
char tempname[FPATHLNG];

    time(&tstart);
    ctrlwd |= NOBLANK;                                  /* Ensure no blanking */

    for(count=0;count<listlength;count++)               /* Repeat for each entry on display list */
      {
	sprintf(tempname,"%s%s",TMPDIR,dlist[count].name);      /* Concatenate path and filename */
	if((finp=fopen(tempname,RACCESS))!=(FILE *)NULL)
	    {
		_clearscreen(0);
		printf("outputting file no. %d  :  %s\n",count,dlist[count].name);
		output_data(finp);                      /* Output bitmap via printer port */
		fclose(finp);
		screen_display(tempname);               /* Display on computer screen */
		time(&tnow);

		while((tnow-tstart)<DISPSECS)           /* Wait for duration of message */
		    time(&tnow);
		tstart=tnow;
	    }
      }
    return;
}


