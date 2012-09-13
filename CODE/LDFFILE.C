/***************************************************************************
 *      ldffile.c                                                          *
 *                                                                         *
 *      Laser Display Formatter Message Deletion and Access                *
 *                              Restriction routines.                      *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "ldf.h"
#include "dirs.h"

/***************************************************************************/

int delmes(mesid, user) /* Deletes all of user's messages with a message */
			/* ID the same as mesid.  Returns the number of  */
			/* messages deleted.                             */

			/* 29/3/90  R J Finean */

char *mesid,            /* Message identifier */
     *user;             /* User mailbox address */

{
  int nodeleted = 0;            /* Number of messages deleted */
  struct find_t finder;         /* Structure for file finding */
  boolean more;                 /* More files found flag */

  more = !_dos_findfirst(MESSDIR DISPFILE, _A_NORMAL, &finder);
  while (more)
    {
      if (erasemes(finder.name, mesid, user))
	  nodeleted++;
      more = !_dos_findnext(&finder);
    }
  more = !_dos_findfirst(MESSDIR BACKFILE, _A_NORMAL, &finder);
  while (more)
    {
      if (erasemes(finder.name, mesid, user))
	  nodeleted++;
      more = !_dos_findnext(&finder);
    }
  return(nodeleted);
}

/***************************************************************************/

boolean erasemes(fname, mesid, user)
			/* Check if fname in MESSDIR was sent by user and */
			/* has message ID mesid.  If it does, delete and  */
			/* return OK, otherwise return FAIL.  Deletes     */
			/* messages from both MESSDIR and TMPDIR          */
			/* directories.                                   */

			/* 29/3/90 R J Finean */

char *fname,            /* Message display filename */
     *mesid,            /* Message ID to match */
     *user;             /* User mailbox address to match */

{
  FILE *mfp;                    /* Message file pointer */
  char txt[MAXLINELEN],         /* Text from message file */
       fpath[MAXLINELEN];       /* Display message full pathname */

  sprintf(fpath, MESSDIR "%s", fname);
  if (!(mfp = fopen(fpath, "rb")))
    {
      fprintf(stderr, "erasemes: can't open message file %s - deleting.\n\n",
	      fpath);
      remove(fpath);
      sprintf(fpath, TMPDIR "%s." BAKFEXT, strtok(fname, "."));
      remove(fpath);
      return(FAIL);
    }

  if (!strcmp(strtok(fgets(txt, MAXLINELEN, mfp), "\n"), user))
      if (!strcmp(strtok(fgets(txt, MAXLINELEN, mfp), "\n"), mesid))
	{
	  fclose(mfp);
	  remove(fpath);
	  sprintf(fpath, TMPDIR "%s." BAKFEXT, strtok(fname, "."));
	  remove(fpath);
	  return(OK);
	}
  fclose(mfp);
  return(FAIL);
}
