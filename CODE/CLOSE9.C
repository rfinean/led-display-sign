/***************************************************************************
 * Laser Display : Closedown Subroutine  :  close9.c                       *
 *                                                                         *
 * Displays a standard closing down message (defined in dirs.h).           *
 * After a period or if a file cannot be opened, the display is blanked    *
 * and the laser is turned off.                                            *
 * Then waits for morning (of a weekday) to turn the laser on and read and *
 * output 2 opening messages (defined in dirs.h).                          *
 * After allowing the laser time to warm up, the display is unblanked if   *
 * both opening messages were found for output.                            *
 *                                                                         *
 *      R S Barker      6/2/90    8/4/90                                   *
 ***************************************************************************/


#include "header.h"

extern F_HEADER dlist[MAXFILES];
extern char ctrlwd;

void close_down()

{
int count1;
time_t reftime,newtime;                                 /* Time variables (seconds since 0:00 GMT 1/1/70) */
struct tm *bdtime;                                      /* Time broken down into years, months, days etc */
FILE *finp;

    time(&reftime);                                     /* Find present time */
    printf("`Time for bed !', says Zebedee.\nClosing down now\n");
    if((finp=fopen(CLOSEMSG,RACCESS))!=(FILE *)NULL)
	{                                               /* Display closing down message */
	    output_data(finp);
	    fclose(finp);
	    do
		{
		    time(&newtime);
		}
	    while(newtime-reftime < CLOSESECS);
	}
    ctrlwd &= BLANK;
    ctrlwd |= LASEROFF;
    outp(CTRLPORT,ctrlwd);
    printf("Goodnight from the 1989/1990 MEng Group B.\n\n");

    do
	{                                               /* Remain dormant until weekday morning */
	    time(&reftime);
	    bdtime=localtime(&reftime);
	}
    while(bdtime->tm_hour < MORNINGHR || bdtime->tm_hour >= EVENINGHR || !(bdtime->tm_wday%6));

    ctrlwd &= LASERON;
    outp(CTRLPORT,ctrlwd);
    printf("Yaaawwwnnn! : Wakey, wakey!\n");

    if((finp=fopen(OPENMSG1,RACCESS))!=(FILE *)NULL)
	{
	    output_data(finp);
	    fclose(finp);
	    if((finp=fopen(OPENMSG2,RACCESS))!=(FILE *)NULL)
		{
		    output_data(finp);
		    fclose(finp);
		    printf("Morning messages : Laser warm up period\n");

		    do
			{                               /* Allow laser time to warm up (may not be necessary) */
			    time(&newtime);
			}
		    while(newtime-reftime < WARMSECS);

		    ctrlwd |= NOBLANK;                  /* Unblank display if both opening messages found */
		    outp(CTRLPORT,ctrlwd);
		}
	}
    return;
}
