/***************************************************************************
 *      ldf.h                                                              *
 *                                                                         *
 *      Laser Display Formatter Definitions                                *
 *                                                                         *
 *      1989/90 Group Design Project B,  R J Finean.                       *
 ***************************************************************************/

/* Declare version number SET AT TOP OF LDF.C */

extern const char vers[];


/* Type definitions */

typedef enum {FALSE, TRUE} boolean;     /* Defines TRUE and FALSE */
typedef struct {int length;
		char *pattern[8];} cdef;/* Struct to hold defn of a char */


/* Constants */

#define OK              (boolean)TRUE
#define FAIL            (boolean)FALSE
#define MAXLINELEN      128             /* Max no of chars on input lines */
#define MAXLDMFILES     32767           /* Max no of .LDM permissible */
#define RIGHTEDGE       1024            /* Modulator line width */
#define USELINWID       924             /* Maximum useable line width */
#define MINLM           ((RIGHTEDGE - USELINWID) / 2)
					/* Minimun left margin in pixels */
#define LINEPIXELS      8               /* Vertical pixel resolution */
#define DISPLAYLINES    3               /* No of text lines on display */
#define SP              18              /* Word spacing in pixels */
#define LSP             8               /* Letter spacing in pixels */
#define ROMEXPN         2               /* Expansion for roman typeface */
#define BOLDEXPN        4               /* Expansion for boldface */
#define IS              3               /* Italics slant ratio */
#define ON              (boolean)TRUE   /* Pixel lit code */
#define OFF             (boolean)FALSE  /* Pixel not lit code */
#define LOW             0               /* Lowest display priority */
extern const cdef font[];               /* Standard ASCII character set */


/* Macros */

#define incid(c)        (char)(isgraph(c) ? ((c) - '!') : ((c) + '~' - '!'))
					/* Internal character identifier */
#define charwid(c)      (int)font[incid(c)].length
					/* Width of character */
#define charpix(c,x,y)  (boolean)(*(font[incid(c)].pattern[y] + (x)) != ' ')
					/* State of pixel in character */

/* Declare ldf's function's return types */

extern void dotcom(), leftmarg(), rightmarg(), set(), outline(), getmail(),
	    ldmname();
extern boolean ldf(), rdhdr(), rdtxt(), gettxt(), getlin(), mailback(),
	       erasemes();
extern int calclm(), drawline(), delmes();
