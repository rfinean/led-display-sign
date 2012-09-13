/***************************************************************************
 * Laser Display : Bit-map Data Output Subroutine  :  outppp9.c            *
 *                                                                         *
 * Outputs a complete bit-map through parallel printer port.               *
 *                                                                         *
 *      R S Barker      8/4/90                                             *
 ***************************************************************************/


#include <header.h>

extern char ctrlwd;

void output_data(finp)

FILE *finp;                                             /* Pointer to file for output */

{
int count1;

    fscanf(finp,"%*s\n%*s\n%*ld\n%*ld\n%*d\n");         /* Read and ignore header */
    for(count1=MAXDATA;count1>0;count1--)
	{
	    outp(CTRLPORT,(ctrlwd|=LOSTROBE));          /* Set strobe low */
	    outp(DATAPORT,getc(finp));                  /* Read and output data byte */
	    outp(CTRLPORT,(ctrlwd&=HISTROBE));          /* Set strobe high (causing bit-map RAM to read byte) */
	}
    ctrlwd |= LOSTROBE;                                 /* Prepare control byte with low strobe */
    ctrlwd ^= BANKSEL;                                  /* and other bank of RAM selected */
    outp(CTRLPORT,ctrlwd);                              /* Switch the message */
    return;
}

