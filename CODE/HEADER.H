/***************************************************************************
 * Laser Display : Header File  :  header.h                                *
 *                                                                         *
 * Contains include files, constants for output ports and times etc,       *
 * structure definition and external file declarations.                    *
 *                                                                         *
 *      R S Barker      6/2/90    7/4/90                                   *
 ***************************************************************************/


#include <stdio.h>
#include <dos.h>
#include <direct.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include "dirs.h"                                       /* Directory and file definitions */

#define LSIZE           20                              /* Size of high,low priority lists */
#define MAXFILES        (3*LSIZE)                       /* Max messages for one sequence */
#define FNAMLNG         13                              /* Max filename length */
#define FPATHLNG        40                              /* Max length of filename with path prefix */
#define SEVERAL         12                              /* Threshold for only high files to be displayed */
#define FEW             4                               /* Threshold for adding in standard files */
#define CTRLPORT        0x3BE                           /* Parallel printer port control */
#define DATAPORT        0x3BC                           /* and data addresses */
#define MAXDATA         3072                            /* No of bytes per screen */
#define MASK            0xF                             /* Mask for control byte */
#define LOSTROBE        1                               /* Strobe : control bit 0 */
#define HISTROBE        (~LOSTROBE & MASK)
#define LASEROFF        (1<<1)                          /* Laser power : control bit 1 */
#define LASERON         (~LASEROFF & MASK)
#define NOBLANK         (1<<2)                          /* Blank : control bit 2 */
#define BLANK           (~NOBLANK & MASK)
#define BANKSEL         (1<<3)                          /* Bank Select : control bit 3 */
#define MORNINGHR       8                               /* Hour of morning start */
#define EVENINGHR       18                              /* Hour of evening closedown */
#define CLOSESECS       (2*60)                          /* Duration of closedown message */
#define WARMSECS        (1*60)                          /* Motor and laser warm up time */
#define DISPSECS        8                               /* Duration of one message */
#define LISTSECS        (4*60)                          /* Minimum duration of list and so */
							/* min time between log-ons to VAX */

typedef struct                                          /* Structure for holding filename and */
    {                                                   /* deletion time (seconds since 0:00 GMT 1/1/70) */
	char name[FNAMLNG];
	time_t endtime;
    }F_HEADER;

extern void restore_files();                            /* RSB */
extern int create_list();                               /* RSB */
extern void review_files();                             /* RSB */
extern void process_file();                             /* RSB */
extern void add_file();                                 /* RSB */
extern void display();                                  /* RSB */
extern void close_down();                               /* RSB */
extern void output_data();                              /* RSB */
extern void getmail();                                  /* RJF */
extern void screen_display();                           /* RJF */

extern FILE *fopen();
