/***************************************************************************
 *      ldfbmap.c                                                          *
 *                                                                         *
 *      Laser Display Formatter Bit-map manipulation routines              *
 *                              for use with prototype deflection polygon  *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

#include <stdio.h>
#include "ldf.h"


/* Assign external static workspace */

static boolean stave[RIGHTEDGE][LINEPIXELS];    /* Line bit-map */

/***************************************************************************/

void set(x, y, line, state)
			/* Sets pixel (x, y) on stave to state, taking     */
			/* account of x-axis scanning corrections required */
			/* for given line of the laser display.            */

			/* N.B. -- This function is taylored to the proto- */
			/* type polygon and will need aletration to work   */
			/* effectively with any other deflection system.   */
			/* This function should be replacable by a macro   */
			/* #define set(x, y, line, state)
						       stave[x][y] = state */
			/* for ideal polygons.                             */

			/* 23/11/89, 7/4/90  R J Finean */

int x, y,               /* Horizontal and vertical pixel positions */
    line;               /* Display line number */
boolean state;          /* What the pixel is to be set to */

{
  static const int lineorder[DISPLAYLINES][LINEPIXELS]
		       = {{1, 3, 0, 4, 2, 5, 6, 7},
			  {2, 1, 0, 4, 3, 6, 5, 7},
			  {4, 1, 0, 5, 2, 3, 6, 7}},
		      /* Order of lines on each stave of prototype polygon */
		   lineoffset[DISPLAYLINES][LINEPIXELS]
		       = {{  7,  41,  11, -16,  -4,  -6, -22,  -5},
			  { -2,   4,  18,  14,   5,  25,  27,  14},
			  {  7,  19,  -2,  12,   8,  17,  17,  -4}};
		       /* Horizontal offset applied to each line of pixels */

  x += lineoffset[line - 1][y];
  if (x > RIGHTEDGE)
      x -= RIGHTEDGE;
  if (x < 0)
      x += RIGHTEDGE;
  y = lineorder[line - 1][y];
  if ((line == 3) && (y == 7))
      stave[x][7] = state;		/* = OFF for prototype */
  else
      stave[x][y] = state;
}

/***************************************************************************/

void outline(ofp)       /* Outputs bit-map on stave to output file, ofp, */
			/* in a binary format consisting of 8-bit bytes. */

			/* 23/11/89, 8/2/90  R J Finean */

FILE *ofp;              /* Output file pointer */

{
  int xprime, yprime;   /* Corrected horizontal & vertical stave co-ords */
  register int bit;     /* Bit index */
  register char pattern;/* 8-bit binary pattern */

  for(yprime = 0; yprime < LINEPIXELS; yprime++)
      for(xprime = 0; xprime < RIGHTEDGE; )
	{
	  pattern = 0;
	  for(bit = 8; bit--; )
	      pattern = (pattern << 1) + stave[xprime++][yprime];
	  putc(pattern, ofp);
	}
}
