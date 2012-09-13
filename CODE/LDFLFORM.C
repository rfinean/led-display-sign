/***************************************************************************
 *      ldflform.c                                                         *
 *                                                                         *
 *      Laser Display Formatter Line Forming Routines                      *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

#include <ctype.h>
#include "ldf.h"

/***************************************************************************/

void leftmarg(x, line)  /* Starts new line's bit pattern by clearing left */
			/* margin on stave.                               */

			/* 23/11/89  R J Finean */

int x,                  /* Width of left margin in pixels */
    line;               /* Display line number */

{
  int y;                /* Vertical position on stave */

  while (x--)
      for (y = LINEPIXELS; y--; )
	  set(x, y, line, OFF);
}

/***************************************************************************/

void rightmarg(x, line) /* Clears right margin on stave, starting at */
			/* column x.                                 */

			/* 23/11/89  R J Finean */

int x,                  /* Horizontal position on stave */
    line;               /* Display line number */

{
  int y;                /* Vertical position on stave */

  for ( ; x < RIGHTEDGE; x++)
      for (y = LINEPIXELS; y--; )
	  set(x, y, line, OFF);
}

/***************************************************************************/

int drawline(col, line, boldflag, italic, uline, txt)
		        /* Draws given line of text onto stave starting */
			/* at column col. Returns starting column of    */
			/* right margin. Boldface, italic and underline */
			/* attributes are altered by reference so that  */
			/* attributes continue onto subsequent lines    */
			/* unless reset with \f1.                       */

			/* 4/12/89, 1/3/90  R J Finean */

int col,                /* Column at which to start drawing */
    line;               /* Display line number */
boolean *boldflag,      /* Pointer to boldface flag */
	*italic,        /* Pointer to italics flag */
	*uline;         /* Pointer to underline flag */
char *txt;              /* Pointer to line of text */

{
  int x, y,             /* Horizontal & vertical indicies used for drawing */
      rep;              /* Repetition index for pixels in character bitmap */

  if (*italic)
      /* Blank leading italics triangle if entering line in italics */
      for (y = LINEPIXELS; y--; )
	  for (x = (LINEPIXELS - y) * IS; x--; )
	      set(col + x, y, line, OFF);
  while (*txt)
    {
      if (isspace(*txt))
          /* Draw a space */
	  for (x = SP; x--; col++)
	      for (y = LINEPIXELS; y--; )
		  set(col + *italic * IS * (LINEPIXELS - y), y, line,
		      *uline && (y == (LINEPIXELS - 1)));
      else if ((*txt == '\\') && (*(txt+1) == 'f'))
          /* Interpret change font escape sequence */
	  switch (*(txt += 2))
            {
	      case 'I':
	      case '2':
		  *italic = TRUE;
		  for (y = LINEPIXELS; y--; )
		      for (x = (LINEPIXELS - y) * IS; x--; )
			  set(col + x, y, line, OFF);
                  break;
	      case 'B':
	      case '3':
		  *boldflag = TRUE;
		  break;
	      case 'U':
	      case '4':
		  *uline = TRUE;
		  break;
              case 'P':
	      case 'R':
	      case '1':
		  if (*italic)
		    {
		      *italic = FALSE;
		      for (y = LINEPIXELS; y--; )
			  for (x = (LINEPIXELS - y) * IS;
			       x < LINEPIXELS * IS; x++)
			      set(col + x, y, line, OFF);
		      col += LINEPIXELS * IS;
		    }
		  *uline = FALSE;
		  *boldflag = FALSE;
            }
      else if (isgraph(*txt))
        {
          /* Draw character */
	  if ((*txt == '\\') && (*(txt+1) != 'f'))
              txt++;
	  for (x = 0; x < charwid(*txt); x++)
	      for (rep = (*boldflag ? BOLDEXPN : ROMEXPN); rep--; col++)
		  for (y = LINEPIXELS; y--; )
		      set(col + *italic * IS * (LINEPIXELS - y),
			  y, line, charpix(*txt, x, y)
				   || (*uline && (y == (LINEPIXELS - 1))));
	  /* Draw inter-letter whitespace */
	  for (x = LSP; x--; col++)
	      for (y = LINEPIXELS; y--; )
		  set(col + *italic * IS * (LINEPIXELS - y), y, line,
		      *uline && (y == (LINEPIXELS - 1)));
	}
      txt++;
    }
  if (*italic)
    {
      /* Blank trailling italics triangle if leaving line in italics */
      for (y = LINEPIXELS; y--; )
	  for (x = (LINEPIXELS - y) * IS; x < (LINEPIXELS * IS); x++)
	      set(col + x, y, line, OFF);
      col += LINEPIXELS * IS;
    }
  return(col);
}
