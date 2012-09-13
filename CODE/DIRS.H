/***************************************************************************
 *      dirs.h                                                             *
 *                                                                         *
 *      Laser Display Software Directory and File Definitions              *
 *      (Note that some other directories are used by uuio and mailer)     *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean, R S Barker            *
 ***************************************************************************/


/* Directories */

#define MAILDIR         "B:\\usr\\mail\\"               /* Mailbox directory */
#define MESSDIR         "B:\\mess\\"                    /* .LDM file directory */
#define TMPDIR          "C:\\"                          /* Temp storage for display mess. */
#define EXEDIR          "A:\\bin\\"                     /* Executable program directory */
#define SPOOLDIR        "B:\\usr\\spool\\"              /* Mail spooler directory */
#define STDFDIR         MESSDIR "std\\"                 /* Standard file directory */


/* Special files */

#define MAILBOX         MAILDIR "GDPB"                  /* Mailbox name */
#define TACKFILE        TMPDIR "RETURN.ACK"             /* Temporary acknowledge file */
#define HELPFILE        "A:\\help\\README.TXT"          /* User Guide */
#define BANNERFILE      "B:\\usr\\news\\BANNER.TXT"     /* Topical information file */
#define CLOSEMSG        MESSDIR "close\\CLOSE.LDM"      /* Close-down message */
#define OPENMSG1        MESSDIR "close\\OPEN1.LDM"      /* Re-opening messages */
#define OPENMSG2        MESSDIR "close\\OPEN2.LDM"


/* Display message file search templates */

#define DISPFILE        "M*.LDM"                        /* Display message file */
#define BACKFILE        "M*.BAK"                        /* Backed-up message display file */


/* Display message file extensions */

#define DOT             '.'
#define DISPFEXT        "LDM"                           /* Display message file extension */
#define BAKFEXT         "BAK"                           /* Back-up message file extension */


/* Display message file access attributes */

#define RACCESS         "rb"                            /* Read access for display files */
#define WACCESS         "wb"                            /* Write access for display files */
