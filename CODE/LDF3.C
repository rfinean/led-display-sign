/***************************************************************************
 *      ldf3.c                                                             *
 *                                                                         *
 *      Laser Display Formatter with Mail Interface (stand-alone version)  *
 *                                                                         *
 *      Software for formatting incomming mail for display by the          *
 *      Laser Display System.                                              *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

	const char vers[] = "v3.4,  19/4/90  R J Finean.";

/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "ldf.h"
#include "dirs.h"

/***************************************************************************/

void main()
{
  getmail();
}

/***************************************************************************/

void getmail()          /* Logs on to mail server to send spooled mail and */
			/* collect any new mail in MAILDIR.  Reads through */
			/* new mail, formatting each message individually, */
			/* storing the output bit-map file in MESSDIR and  */
			/* spooling the acknowledgement file ready for     */
			/* sending next time getmail is called.            */

			/* 20/3/90, 21/3/90  R J Finean */

{
  char txt[MAXLINELEN];         /* Miscellaneous text */
  FILE *mfp;                    /* Mailbox file pointer */

  system(EXEDIR "uuio -x0");   /* Send old acknowledgenents & get new mail */
  remove(SPOOLDIR "logfile");

  if (mfp = fopen(MAILBOX, "rt"))
    {
      fgets(txt, MAXLINELEN, mfp);     /* Ignore first line, only a header */
      while (!feof(mfp))
	  /* Process one message at a time */
	  if (!ldf(mfp))
	      break;
      fclose(mfp);
      remove(MAILBOX);
    }
}

/***************************************************************************/

boolean ldf(ifp)        /* Reads input file ifp up to start of a new mail  */
			/* header or end of file. Extracts header and      */
			/* formats remainder of text into a bitmap file in */
			/* the MESSDIR directory. The sender of the mail   */
			/* is acknowledged by a return message reporting   */
			/* the success, or otherwise, of the formatting    */
			/* operation. Function returns FAIL only if the    */
			/* file system causes problems - otherwise OK.     */
			/* If OK is returned, ifp will be positioned       */
			/* either at the end-of-file or the start of       */
			/* another mail header. If FAIL is returned ifp    */
			/* may be positioned anywhere.                     */

			/* 12/2/90, 20/3/90  R J Finean */

FILE *ifp;              /* Input file */

{
  FILE *afp,                    /* Temporary acknowledgement file pointer */
       *ofp;                    /* Bit-map file pointer */
  char ofname[MAXLINELEN];      /* Bit-map file full pathname */

  if (!(afp = fopen(TACKFILE, "wt")))
    {
      fputs("ldf: can't open temporary acknowledge file " TACKFILE ".\n",
	    stderr);
      return(FAIL);
    }
  ldmname(ofname);
  if (!(ofp = fopen(ofname, "wb")))
    {
      fprintf(stderr, "ldf: can't open message display file %s.\n\n", ofname);
      fclose(afp);
      return(FAIL);
    }
  printf("Formatting %s\n", ofname);
  if (!rdhdr(ifp, ofp, afp))
    {
      fclose(afp);
      fclose(ofp);
      remove(ofname);
      mailback();
      return(OK);
    }
  if (!rdtxt(ifp, ofp, afp))
    {
      fputs("\n       ****   Errors occurred -"
	    " MESSAGE NOT ON DISPLAY   ****\n", afp);
      fclose(ofp);
      remove(ofname);
    }
  else
      fclose(ofp);
  fclose(afp);
  mailback();
  return(OK);
}

/***************************************************************************/

boolean mailback()      /* Adds acknowledge file contents to the mail */
			/* spooler, ready for posting next time uuio  */
			/* is called. Returns OK if successful, FAIL  */
			/* if the file system causes trouble.         */

			/* 20/3/90  R J Finean */

{
  FILE *afp;             /* Acknowledge file pointer */
  char addr[MAXLINELEN], /* Address to return acknowledgement to */
       clin[MAXLINELEN]; /* String in which to make up command line */

  if (!(afp = fopen(TACKFILE, "rt")))
    {
      fputs("mailback: can't open acknowledge file" TACKFILE ":", stderr);
      fputs("          message not acknowledged.\n", stderr);
      remove(TACKFILE);
      return(FAIL);
    }
  getlin(addr, afp);
  fclose(afp);
  sprintf(clin, EXEDIR "MAILER.EXE %s <" TACKFILE, addr);
  system(clin);
  remove(TACKFILE);
  return(OK);
}

/***************************************************************************/

void ldmname(fname)     /* Creates a unique filename in *fname suitable */
			/* for a .LDM file in the MESSDIR directory.    */
			/* Uses the static variable messno to store the */
			/* next name in sequence.  Returns the filename */
			/* in *fname.                                   */

			/* 21/3/90  R J Finean */

char *fname;            /* String in which to place filename */

{
  static int messno = 0;/* Numeric value of variable part of filename */
  struct find_t finder; /* Structure to satisfy _dos_findfirst */

  sprintf(fname, MESSDIR "M%d.*", messno);
  while (!_dos_findfirst(fname, _A_NORMAL, &finder))
    {
      if (messno++ >= MAXLDMFILES)
	  messno = 0;
      sprintf(fname, MESSDIR "M%d.*", messno);
    }
  sprintf(fname, MESSDIR "M%d." DISPFEXT, messno++);
}

