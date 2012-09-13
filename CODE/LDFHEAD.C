/***************************************************************************
 *      ldfhead.c                                                          *
 *                                                                         *
 *      Laser Display Formatter Header Deciphering Routine and             *
 *                              Line Input Routine                         *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "ldf.h"
#include "dirs.h"

/***************************************************************************/

boolean rdhdr(ifp, ofp, afp)
			/* Reads mail header from input file ifp and */
			/* writes titles to acknowledge file, afp.   */
			/* Header is deemed to finish when both .FI  */
			/* and .TO lines have been read correctly.   */
			/* Returns FAIL if no .FI starts message, or */
			/* no deletion date is present. Returns OK   */
			/* otherwise.                                */

			/* 15/1/90, 22/2/90  R J Finean */

FILE *ifp,              /* Input text file pointer */
     *ofp,              /* Output file pointer */
     *afp;              /* Acknowledge file pointer */

{
  char txt[MAXLINELEN],         /* Input text */
       *cmd,                    /* Pointer to command token */
       mesid[MAXLINELEN],       /* Message ID */
       sender[MAXLINELEN] = "*** Undefined ***";
				/* Sender's mail address */
  boolean fifound = FALSE,      /* Flag to indicate .FI found */
	  tofound = FALSE;      /* Flag to indicate .TO found */
  int priority = LOW,           /* Message display priority */
      c;                        /* File copy character */
  time_t stime,                 /* Date/time to start displaying message */
	 etime;                 /* Date/time to delete message */
  struct tm *bdstime,           /* Broken down start time pointer */
	    bdetime = {0, 0, 0, 0, 0, 0, 7, 366, 0};
				/* Broken down end time */
  FILE *hfp;                    /* Help and banner file pointer */

  time(&stime);
  bdstime = localtime(&stime);
  while (gettxt(txt, ifp))
    {
      cmd = strtok(txt, " \t");
      if ((!strcmp(cmd, "From:")) && (!strcmp(sender, "*** Undefined ***")))
	{
	  strtok(NULL, "<");
	  strcpy(sender, strtok(NULL, ">"));
	  fprintf(afp, "\n\n%s\n\n", sender);
	  fprintf(afp, "ldf Laser Display Formatter %s\n\n", vers);
	  if (hfp = fopen(BANNERFILE, "rt"))
	    {
	      while ((c = getc(hfp)) != EOF)
		  putc(c, afp);    /* Copy banner file to acknowledge file */
	      fclose(hfp);
	      fputs("\n", afp);
	    }
	}
      else if (!strcmp(cmd, ".ER"))
	{
	  strcpy(mesid, strtok(NULL, " \t\n"));
	  fprintf(afp, "\n%d message(s) with ID %s deleted.\n",
		  delmes(mesid, sender), mesid);
	  while (gettxt(txt, ifp))
	      ;                 /* Read to end of message before returning */
	  return(FAIL);
	}
      else if (!strcmp(cmd, ".HE"))
	{
	  if (hfp = fopen(HELPFILE, "rt"))
	    {
	      while ((c = getc(hfp)) != EOF)
		  putc(c, afp);      /* Copy help file to acknowledge file */
	      fclose(hfp);
	    }
	  else
	      fputs("rdhdr: can't open help file " HELPFILE "\n", stderr);
	  while (gettxt(txt, ifp))
	      ;                 /* Read to end of message before returning */
	  return(FAIL);
	}
      else if (!strcmp(cmd, ".FI"))
	{
	  strcpy(mesid, strtok(NULL, " \t\n"));
	  fprintf(afp, "Message has ID %s ", mesid);
	  fifound = TRUE;
	}
      else if (!strcmp(cmd, ".PR"))
	  priority = atoi(strtok(NULL, " \t\n"));
      else if (!strcmp(cmd, ".FR"))
	{
	  bdstime->tm_mday = atoi(strtok(NULL, "/-\\"));
	  bdstime->tm_mon = atoi(strtok(NULL, "/-\\")) - 1;
	  bdstime->tm_year = atoi(strtok(NULL, ",; \t"));
	  bdstime->tm_hour = atoi(strtok(NULL, ".:"));
	  bdstime->tm_min = atoi(strtok(NULL, ".: \t\n"));
	  bdstime->tm_sec = 0;
	  if ((stime = mktime(bdstime)) == -1)
	    {
	      fputs("\n\nError - Message start time not understood.", afp);
	      fputs("        Correct format is .FR DD/MM/YY HH:MM\n", afp);
	      while (gettxt(txt, ifp))
		  ;             /* Read to end of message before returning */
	      return(FAIL);
	    }
	}
      else if (!strcmp(cmd, ".TO"))
	{
	  bdetime.tm_mday = atoi(strtok(NULL, "/-\\"));
	  bdetime.tm_mon = atoi(strtok(NULL, "/-\\")) - 1;
	  bdetime.tm_year = atoi(strtok(NULL, ",; \t"));
	  bdetime.tm_hour = atoi(strtok(NULL, ".:"));
	  bdetime.tm_min = atoi(strtok(NULL, ".: \t\n"));
	  if ((etime = mktime(&bdetime)) == -1)
	    {
	      fputs("\n\nError - Deletion date not understood.", afp);
	      fputs("        Correct format is .TO DD/MM/YY HH:MM\n", afp);
	      while (gettxt(txt, ifp))
		  ;             /* Read to end of message before returning */
	      return(FAIL);
	    }
	  tofound = TRUE;
	}
      if (fifound && tofound)
	{
	  fprintf(afp, "and is to display at priority %d\n", priority);
	  fprintf(afp, "from %s", ctime(&stime));
	  fprintf(afp, "to %s\n", ctime(&etime));
	  fputs("Message reads as follows:\n\n", afp);
	  fprintf(ofp, "%s\n%s\n%ld\n%ld\n%d\n", sender, mesid,
		  stime, etime, priority);
	  return(OK);
	}
    }
  if (!fifound)
    {
      fputs("Error - Unable to find .FI at start of message text.\n", afp);
      return(FAIL);
    }
  else if (!tofound)
    {
      fputs("\n\nError - Unable to find .TO to get message expiry date.\n",
	    afp);
      return(FAIL);
    }
}

/***************************************************************************/

boolean gettxt(txt, ifp)/* Get text from mail file, returning FAIL for    */
			/* both EOF and the start of a new mail header.   */
			/* Ignores the mail signature, if one is present. */
			/* Removes the '>' from start of lines beginning  */
			/* "From " which the mail system puts in.         */

			/* 21/3/90, 19/4/90  R J Finean */

char *txt;              /* String in which to return line of text */
FILE *ifp;              /* Input text file pointer */

{
  if (!getlin(txt, ifp))
      return(FAIL);
  if (!strncmp(txt, "From ", 5))
      return(FAIL);
  if (!strncmp(txt, "--", 2))
    {
      while (gettxt(txt, ifp))
	  ;                       /* Skip over signature to end of message */
      return(FAIL);
    }
  if (!strncmp(txt, ">From ", 6))
      while (*txt = *(txt+1))
	  txt++;                              /* Skip over '>' of ">From " */
  return(OK);
}

/***************************************************************************/

boolean getlin(txt, ifp)/* Get a line of text from ifp and strip        */
			/* leading and trailing spaces. Return FAIL if  */
			/* end of file or OK if text available in *txt. */

			/* 16/2/90, 12/3/90  R J Finean */

char *txt;              /* String in which to return line of text */
FILE *ifp;              /* Input text file pointer */

{
  register int c = MAXLINELEN;  /* Character count */

  while (isspace(*txt = (char)getc(ifp)))
      ;                                       /* Ignore leading whitespace */
  if (*txt == EOF)
      return(FAIL);
  while (((*++txt = (char)getc(ifp)) != '\n') && c--)
      ;                                             /* Store line up to \n */
  while (isspace(*txt--))
      ;                                /* Go back over trailing whitespace */
  *(txt+2) = '\0';
  return(OK);
}
