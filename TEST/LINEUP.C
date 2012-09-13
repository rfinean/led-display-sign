/***************************************************************************
 *	lineup.c							   *
 *									   *
 *	Laser Display Formatter Customisation Utility			   *
 *									   *
 *	1989/90 Group Design Project B,  R J Finean			   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* Constants */

#define CTRLPORT	0x3BE		  /* Parallel printer port control */
#define DATAPORT	0x3BC		  /* and data addresses */
#define MASK		0xF		  /* Mask for control byte */
#define LOSTROBE	1		  /* Strobe : control bit 0 */
#define HISTROBE	(~LOSTROBE & MASK)
#define LASEROFF	(1<<1)		  /* Laser power : control bit 1 */
#define LASERON 	(~LASEROFF & MASK)
#define NOBLANK 	(1<<2)		  /* Blank : control bit 2 */
#define BLANK		(~NOBLANK & MASK)
#define BANKSEL 	(1<<3)		  /* Bank Select : control bit 3 */

/* Externals */

static char ctrlwd;
static int stave[1024][8];	/* Line bit-map */

int lineoffset[3][8]		/* Horizontal offsets applied to eack line */

/***************************************************************************/

void main(void) 		/* Displays a vertical line in centre of */
				/* screen.				 */

				/* 19/4/90  R J Finean */

{
  FILE *fp;
  int i, j, k;

  ctrlwd = (NOBLANK | LOSTROBE | BANKSEL) & LASERON;
					 /* Sets strobe low, blanking off, */
  outp(CTRLPORT,ctrlwd);		 /* laser on, writing to bank 0 */

  while (changes())
    {
      fp = fopen("c:\dump", "wb");
      for (i = 0; i <= 2; i++)
	{
	  for (j = 0; j <= 7; j++)
	    {
	      for (k = 0; k <= 1024; k++)
		  set(k, j, i, 0);
	      set(512, j, i, 1);
	    }
	  outline(fp);
	}
      fclose(fp);
      fp = fopen("c:\dump", "rb");
      output_data(fp);
      fclose(fp);
    }
}

/***************************************************************************/

changes(void)		/* Allows user to alter horizontal offsets */

			/* 19/4/90  R J Finean */

{
  int x,y;

  for ( ; ; )
    {
      printf("Change (99 to try again, -1 to end) ");
      scanf("%d %d", &x, &y);
      if (x == 99)
	{
	  for (x=0; x<=2; x++)
	    {
	      for (y=0; y<=7; y++)
	      printf(" %4d ", lineoffset[x][y]);
	      printf("\n");
	    }
	  return(1);
	}
      if (x == -1)
	  return(0);
      printf(" From %d to ", lineoffset[x][y]);
      scanf("%d", &lineoffset[x][y]);
    }
}

/***************************************************************************/

set(x, y, line, state)	/* Sets pixel (x, y) on stave to state, taking	   */
			/* account of x-axis scanning corrections required */
			/* for given line of the laser display. 	   */

			/* 23/11/89, 19/4/90  R J Finean */

int x, y,		/* Horizontal and vertical pixel positions */
    line;		/* Display line number */
int state;		/* What the pixel is to be set to */

{
  static const int lineorder[3][8]
		       = {{1, 3, 0, 4, 2, 5, 6, 7},
			  {2, 1, 0, 4, 3, 6, 5, 7},
			  {4, 1, 0, 5, 2, 6, 3, 7}};
		      /* Order of lines on each stave of prototype polygon */

  x += lineoffset[line][y];
  if (x > 1024)
      x -= 1024;
  if (x < 0)
      x += 1024;
  y = lineorder[line][y];
  stave[x][y] = state;
}

/***************************************************************************/

outline(ofp)		/* Outputs bit-map on stave to output file, ofp, */
			/* in a binary format consisting of 8-bit bytes. */

			/* 23/11/89, 8/2/90  R J Finean */

FILE *ofp;		/* Output file pointer */

{
  int xprime, yprime;	/* Corrected horizontal & vertical stave co-ords */
  register int bit;	/* Bit index */
  register char pattern;/* 8-bit binary pattern */

  for(yprime = 0; yprime < 8; yprime++)
      for(xprime = 0; xprime < 1024; )
	{
	  pattern = 0;
	  for(bit = 8; bit--; )
	      pattern = (pattern << 1) + stave[xprime++][yprime];
	  putc(pattern, ofp);
	}
}

/***************************************************************************/

output_data(finp)

FILE *finp;

{
int count1;

    for(count1=3072;count1>0;count1--)
	{
	    outp(CTRLPORT,(ctrlwd|=LOSTROBE)); /* Set strobe low */
	    outp(DATAPORT,getc(finp));	       /* Read and output data byte */
	    outp(CTRLPORT,(ctrlwd&=HISTROBE)); /* Set strobe high (byte ready) */
	}
    ctrlwd |= LOSTROBE; 		       /* Prepare control byte with low strobe */
    ctrlwd ^= BANKSEL;			       /* and other bank of RAM selected */
    outp(CTRLPORT,ctrlwd);		       /* Switch the message */
    return;
}

