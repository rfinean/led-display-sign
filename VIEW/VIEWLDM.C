/***************************************************************************
 *      viewldm.c                                                          *
 *                                                                         *
 *      Laser Display Message VDU Display Program (stand-alone version)    *
 *                                                                         *
 *      Software for displaying LDM format files to the PC screen.         *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

	static char vers[] = "v1.3,  19/4/90  R J Finean.";

/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <conio.h>
#include <time.h>

/* Constants */

#define OK              1
#define FAIL            0
#define RIGHTEDGE       1024            /* Modulator line width */
#define LINEPIXELS      8               /* Vertical line pixel resolution */
#define DISPLAYLINES    3               /* No of text lines on display */
#define AR              3               /* Aspect ratio factor */
#define MAXLINLEN       80              /* Max header text line length */

/* Declare function returns */

void main(), screen_display(), cleanup(), viewldm(), disattr();
int setup();

/* Externals */

struct videoconfig config;

/***************************************************************************/

void main(argc, argv)   /* Opens input .LDM file and sets up graphics    */
			/* screen.                                       */

			/* 1/3/90  R J Finean */

int argc;               /* Number of arguments on command line */
char **argv;            /* Pointer to command line arguments */

{
  FILE *ifp;            /* Input display message file pointer */

  if (argc != 2)
    {
      fputs("Usage:  viewldm filename.LDM\n", stderr);
      exit(2);
    }
  if (!(ifp = fopen(*++argv, "rb")))
    {
      fprintf(stderr, "File %s not found\n", *argv);
      exit(3);
    }
  if (!setup())
    {
      /* Display only attributes in text mode */
      disattr(ifp);
      fclose(ifp);
      return;
    }
  _settextcolor(2);
  _settextposition(1, 9);
  _outtext("viewldm  ");
  _outtext(vers);
  _setcolor(8);
  _rectangle(_GFILLINTERIOR, 0, -12,  512, 118);
  _setcolor(12);
  _settextcolor(14);
  viewldm(ifp);
  fclose(ifp);
  _settextcolor(10);
  _settextposition(2, 9);
  _outtext("Press any key to continue ...");
  _getch();
  cleanup();
  exit(0);
}

/***************************************************************************/

void viewldm(ifp)       /* Draws display in input file, ifp, to screen. */
			/* N.B. -- Displays lines in same order as      */
			/*         prototype polygon.                   */

			/* 12/2/90,  19/4/90  R J Finean */

FILE *ifp;              /* Input file pointer */

{
  static const int displine[DISPLAYLINES][LINEPIXELS]
		 = {{2, 0, 4, 1, 3, 5, 6, 7},
		    {2, 1, 0, 4, 3, 6, 5, 7},
		    {2, 1, 4, 5, 0, 3, 6, 7}};
			/* Order in which prototype polygon displays lines */
  static const int lineoffset[DISPLAYLINES][LINEPIXELS]
		 = {{  7,  41,  11, -16,  -4,  -6, -22,  -5},
		    { -2,   4,  18,  14,   5,  25,  27,  14},
		    {  7,  19,  -2,  12,   8,  17,  17,  -4}};
			/* Horizontal offsets polygon applies to lines */
  int xprime, yprime,   /* Corrected horizontal & vertical stave co-ords */
      y,                /* Original vertical co-ordinate */
      line,             /* Display line index */
      bit;              /* Bit index */
  char pattern,         /* 8-bit binary pattern */
       txt[MAXLINLEN];  /* Header info text string */
  time_t tim;           /* Holding variable to satisfy ctime's prototype */

  _settextposition(21, 9);
  _outtext("      Sender: ");
  _outtext(fgets(txt, MAXLINLEN, ifp));
  _settextposition(22, 9);
  _outtext("  Message ID: ");
  _outtext(fgets(txt, MAXLINLEN, ifp));
  _settextposition(23, 9);
  _outtext("Display from: ");
  fscanf(ifp, "%ld\n", &tim);
  _outtext(ctime(&tim));
  _settextposition(24, 9);
  _outtext("          to: ");
  fscanf(ifp, "%ld\n", &tim);
  _outtext(ctime(&tim));
  _settextposition(24, 62);
  _outtext("Priority: ");
  _outtext(fgets(txt, MAXLINLEN, ifp));

  for(line = 0; line < DISPLAYLINES; line++)
      for(yprime = 0; yprime < LINEPIXELS; yprime++)
	{
	  y = displine[line][yprime];
	  for(xprime = 0; xprime < RIGHTEDGE / 2; )
	    {
	      pattern = (char)getc(ifp);
	      for(bit = 4; bit--; xprime++)
		{
		  if (pattern & 0xC0)
		      _setpixel(xprime - lineoffset[line][y] / 2,
				(y + line * (8 + 6)) * AR);
		  pattern <<= 2;
		}
	    }
	}
}

/***************************************************************************/

void disattr(ifp)       /* Displays message attributes of display file ifp */
			/* to screen in text format.                       */

			/* 1/3/90  R J Finean */

FILE *ifp;              /* Input file pointer */

{
  char txt[MAXLINLEN];  /* Header info text string */
  time_t tim;           /* Holding variable to satisfy ctime's prototype */

  printf("\n      Sender: %s", fgets(txt, MAXLINLEN, ifp));
  printf("  Message ID: %s", fgets(txt, MAXLINLEN, ifp));
  printf("Display from: ");
  fscanf(ifp, "%ld\n", &tim);
  printf(ctime(&tim));
  printf("          to: ");
  fscanf(ifp, "%ld\n", &tim);
  printf(ctime(&tim));
  printf("    Priority: %s\n", fgets(txt, MAXLINLEN, ifp));
}

/***************************************************************************/

int setup()             /* Sets up a high resolution graphics mode, if */
			/* possible. Returns OK if successful, FAIL if */
			/* graphics support is not provided.           */

			/* 12/2/90, 1/3/90  R J Finean */

{
  if (!_setvideomode(_HRES16COLOR))
      if(!_setvideomode(_HRESBW))
	  return(FAIL);
  _getvideoconfig(&config);
  _setlogorg(64, 35);
  return(OK);
}

/***************************************************************************/

void cleanup()          /* Restores default text mode. */

			/* 12/2/90  R J Finean */

{
  _clearscreen(0);
  _setvideomode(_DEFAULTMODE);
}
