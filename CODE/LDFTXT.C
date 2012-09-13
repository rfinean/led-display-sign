/***************************************************************************
 *      ldftxt.c                                                           *
 *                                                                         *
 *      Laser Display Formatter Text Formatting Routines                   *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ldf.h"

/***************************************************************************/

boolean rdtxt(ifp, ofp, afp)
			/* Reads ASCII text from input file, ifp, and     */
			/* translates into a bit-map in output file, ofp. */
			/* Returns OK if successful, FAIL if errors are   */
			/* encountered. Reports errors in acknowledge     */
			/* file, afp.                                     */

			/* 23/11/89, 1/3/90  R J Finean */

FILE *ifp,              /* Input text file pointer */
     *ofp,              /* Output bit-map file pointer */
     *afp;              /* Acknowledge file pointer */

{
  char txt[MAXLINELEN];         /* Input text workspace array */
  int lm,                       /* Left margin width in pixels */
      i,                        /* Centre display margin index */
      line = 0;                 /* Current line counter */
  boolean centre = TRUE,        /* Centre display flag */
	  right = FALSE,        /* Right / left display flag */
	  boldflag = FALSE,     /* Boldface flag */
	  italic = FALSE,       /* Italics flag */
	  uline = FALSE,        /* Underline flag */
	  err = FALSE;          /* Format error flag */

  while (gettxt(txt, ifp))
    {
      if (*txt == '.')
	  dotcom((txt + 1), &boldflag, &italic, &uline, &centre, &right, afp);
      else if (++line > DISPLAYLINES)
	{
	  fputs("\nError - Too many lines for display (max 3 lines):\n\n",
		afp);
	  fputs(txt, afp);
	  while (gettxt(txt, ifp))
	      fputs(txt, afp);  /* Copy to end of message before returning */
	  return(FAIL);
	}
      else
	{
	  if (centre)
	    {
	      fprintf(afp, "%d              ", line);
	      for (i = (40 - strlen(txt)) / 2; i-- > 0; )
		  putc(' ', afp);
	      fputs(txt, afp);
	      putc('\n', afp);
	    }
	  else if (right)
	      fprintf(afp, "%d              %40s\n", line, txt);
	  else
	      fprintf(afp, "%d              %s\n", line, txt);
	  if (!(lm = calclm(txt, boldflag, italic, afp)))
	    {
	      if (boldflag)
		{
		  fputs("          boldface font cancelled to shorten line.",
			afp);
		  if (lm = calclm(txt, FALSE, italic, afp))
		    {
		      boldflag = FALSE;
		      fputs("\n\n", afp);
		    }
		  else
		    {
		      err = TRUE;
		      fputs("Error - Unable to shorten line enough "
			    "simply by cancelling boldface.\n\n", afp);
		    }
		}
	      else
		{
		  fputs("Error - Unable to shorten line.\n\n", afp);
		  err = TRUE;
		}
	    }
	  if (!err)
	    {
	      lm = MINLM + (centre ? lm : (right ? (lm * 2) : 0));
	      leftmarg(lm, line);
	      rightmarg(drawline(lm, line, &boldflag, &italic, &uline, txt),
			line);
	      outline(ofp);
	    }
	}
    }
  if (line == 0)
    {
      fputs("\nError - No message text to display!\n", afp);
      return(FAIL);
    }
  while (line++ < DISPLAYLINES)
    {
      rightmarg(0, line);
      outline(ofp);
    }
  return(!err);
}

/***************************************************************************/

void dotcom(txt, boldflag, italic, uline, centre, right, afp)
			/* Interprets dot command in txt to change font. */
			/* Reports invalid commands only as warnings in  */
			/* acknowledge file, afp.                        */

			/* 15/1/90, 1/3/90  R J Finean */

char *txt;              /* Dot command text */
boolean *boldflag,      /* Pointer to boldface flag */
	*italic,        /* Pointer to italics flag */
	*uline,         /* Pointer to underline flag */
	*centre,        /* Pointer to centre display flag */
	*right;         /* Pointer to right/left display flag */
FILE *afp;              /* Acknowledge file pointer */

{
  char *cmd;            /* Pointer to command token */

  cmd = strtok(txt, " \t\n");
  if (!strcmp(cmd, "TL"))
    {
      *boldflag = TRUE;
      *italic = FALSE;
      *uline = FALSE;
      *centre = TRUE;
      return;
    }
  if (!strcmp(cmd, "SH"))
    {
      *boldflag = TRUE;
      *italic = FALSE;
      *uline = FALSE;
      *centre = FALSE;
      *right = FALSE;
      return;
    }
  if (!strcmp(cmd, "CD") || (*(txt+3)=='C' && !strcmp(cmd, "DS"))
			 || !strcmp(cmd, "DE"))
    {
      *boldflag = FALSE;
      *italic = FALSE;
      *uline = FALSE;
      *centre = TRUE;
      return;
    }
  if (!strcmp(cmd, "LD") || (*(txt+3)=='L' && !strcmp(cmd, "DS")))
    {
      *boldflag = FALSE;
      *italic = FALSE;
      *uline = FALSE;
      *centre = FALSE;
      *right = FALSE;
      return;
    }
  if (!strcmp(cmd, "RD") || (*(txt+3)=='R' && !strcmp(cmd, "DS")))
    {
      *boldflag = FALSE;
      *italic = FALSE;
      *uline = FALSE;
      *centre = FALSE;
      *right = TRUE;
      return;
    }
  if (!strcmp(cmd, "PR") || !strcmp(cmd, "FR")
      || !strcmp(cmd, "TO") || !strcmp(cmd, "FI"))
    {
      fprintf(afp, "\nWarning - .%s command occurred after .TO command:\n",
	      cmd);
      fputs("          displaying as shown above.\n\n", afp);
      return;
    }
  fprintf(afp, "\nWarning - Invalid format dot command .%s ignored\n", cmd);
  fputs("          (only .TL, .SH, .DS, .LD, .CD, .RD and .DE valid).\n\n",
	afp);
}

/***************************************************************************/

int calclm (txt, boldflag, italic, afp)
			/* Calculates width of line and hence left margin */
			/* required for a centred display. Returns centre */
			/* display left margin if line fits on display,   */
			/* or FAIL if it is too long or incorrectly       */
			/* formatted. Reports errors to acknowledge file. */

			/* 23/11/89, 1/3/90  R J Finean */

char *txt;              /* Line of text to format */
boolean boldflag,       /* Boldface flag */
	italic;         /* Italics flag */
FILE *afp;              /* Acknowledge file pointer */

{
  int width;            /* Width of line in pixels */

  width = italic ? IS * LINEPIXELS : 0;
  while (*txt)
    {
      if (isspace(*txt))
	{
	  if (*txt != '\n')
	      width += SP;
	  txt++;
	}
      else if (!isgraph(*txt))
	  fprintf(afp, "\nWarning - Non-ASCII character 0x%x ignored.\n\n",
		  *txt++);
      else if ((*txt == '\\') && (*(txt+1) == 'f'))
	{
	  /* Interpret in-line font changing command */
	  switch (*(txt+2))
	    {
	      case 'B':
	      case '3':
		  boldflag = TRUE;
		  break;
	      case 'I':
	      case '2':
		  width += IS * LINEPIXELS;
	      case 'U':
	      case '4':
		  break;
	      case 'P':
	      case 'R':
	      case '1':
		  boldflag = FALSE;
		  break;
	      default:
		  fprintf(afp, "\nError - Font sequence error: %s\n", txt);
		  fputs("        (only \\f1, \\f2, \\f3 and "
			"\\f4 are defined.\n\n", afp);
		  return(FAIL);
	    }
          txt += 3;
        }
      else
        {
	  if (*txt == '\\')
	    {
	      /* Substitute internal character values for escape codes */
	      if (*(txt+1) == 'E')
		  /* Substitute Elec Eng Dept logo */
		  *(++txt) = 1;
	      else if ((*(txt+1) == '&') || (*(txt+1) == '%'))
		  /* Substitute a zero-width invisible character */
		  *(++txt) = 2;
	      else if (*(txt+1) == '#')
		  /* Substitute a real hash (normally prints as pound) */
		  *(++txt) = 3;
	      else if (*(txt+1) == 'e')
		  /* Substitute backslash character */
		  *(++txt) = '\\';
	      else
		  txt++;
	    }
	  width += LSP + (charwid(*txt) * (boldflag ? BOLDEXPN : ROMEXPN));
	  txt++;
        }
    }
  if (USELINWID > width)
      return((USELINWID - width) / 2);
  else
    {
      fprintf(afp, "\nWarning - Above line is %d%% too long for display:\n",
	      (int)(((double)width / USELINWID - 1) * 100));
      return(FAIL);
    }
}
