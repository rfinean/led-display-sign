/***************************************************************************
 *      ldf.c                                                              *
 *                                                                         *
 *      Laser Display Formatter (stand-alone version)                      *
 *                                                                         *
 *      Software for formatting ASCII text files for display by the        *
 *      Laser Display system.                                              *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

	const char vers[] = "v2.4,  19/4/90  R J Finean.";

/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ldf.h"

/***************************************************************************/

void main(argc, argv)   /* Opens input .TXT file and output .LDM files   */
                        /* then translates ASCII text into a display bit */
			/* map which is output to the .LDM file.         */
			/* Acknowledgement is printed directly to the    */
			/* standard output.  If no output filename is    */
			/* given on the command line, the input filename */
			/* root is used with extension .LDM.             */

			/* 22/11/89, 29/3/90  R J Finean */

int argc;               /* No of arguments on command line */
char **argv;            /* Pointer to command line arguments */

{
  FILE *ifp,            /* Input .TXT file pointer */
       *ofp;            /* Output .LDM file pointer */
  boolean rc;           /* ldf return code */

  if ((argc != 2) && (argc != 3))
    {
      fprintf(stderr, "Usage:  ldf infile.TXT [outfile.LDM]\n");
      exit(2);
    }
  if (!(ifp = fopen(*++argv, "rt")))
    {
      fprintf(stderr, "File %s not found\n", *argv);
      exit(3);
    }
  if (!(ofp = fopen(((argc == 3) ? *++argv
				 : strcat(strtok(*argv, "."), ".LDM")),
		    "wb")))
    {
      fprintf(stderr, "Unable to open output %s file\n", *argv);
      fclose(ifp);
      exit(4);
    }
  rc = ldf(ifp, ofp, stdout);
  fclose(ofp);
  fclose(ifp);
  exit(!rc);
}

/***************************************************************************/

boolean ldf(ifp, ofp, afp)
			/* Extracts message header from input file ifp  */
			/* and formats remaining text into bit-map in   */
			/* ofp. Errors are reported to acknowledge file */
			/* afp. Returns FAIL if errors occur, OK        */
			/* otherwise.                                   */

			/* 12/2/90  R J Finean */

FILE *ifp,              /* Input file pointer */
     *ofp,              /* Output file pointer */
     *afp;              /* Acknowledge file pointer */

{
  char txt[MAXLINELEN]; /* Buffer to satisfy fgets */

  fgets(txt, MAXLINELEN, ifp);  /* Skip over "From " line */
  if (!rdhdr(ifp, ofp, afp))
      return(FAIL);
  if (!rdtxt(ifp, ofp, afp))
    {
      fputs("\n       ****   Errors occurred -"
	    " MESSAGE NOT ON DISPLAY   ****\n", afp);
      return(FAIL);
    }
  return(OK);
}
