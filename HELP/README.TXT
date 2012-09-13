		 ldf LASER DISPLAY FORMATTER INSTRUCTIONS

			      Robert J Finean


1.  System_Specification

The deflection mechanism limits the display to three text lines of approxi-
mately	30 characters each.  The ldf formatter uses a proportionally spaced
font which means that 30 characters of roman text is only a  guideline:  it
is  not  guaranteed that any line less than 30 characters will not overflow
the physical line length when formatted, especially where  capital  letters
are used.


2.  Sending_Messages_to_the_Display

The laser message display has a mailbox on the departmental e-mail  system,
lena.	Thus  messages	can be placed on the display by sending text of the
correct format (see below) to the following mailbox:


			      gdpb@lena.uucp


Messages are usually processed within ten minutes.  Within  twenty  minutes
the  laser display formatter will mail the sender back with an acknowledge-
ment of the message text on display, or a list of formatting errors if	any
occurred.   The  format  of the messages is similar in many respects to the
format which troff expects.  In all cases the formatter is sensitive to the
case of commands.  Messages start with a header describing when the message
is to be displayed.


3.  Message_Header

Messages start with a header of the form

.FI rjf/gdp/headex
.PR 1
.FR 3/4/90 9:30
.TO 3/4/90 18:00

which gives the message a unique identifier, its display priority, "display
from" time and date and "display until" time and date.


3.1.  Message_Identifier

The input file must begin with a "FIle" command

.FI  message_id

where message_id is a unique identifier that the author of the message	can
use  to refer to the message at a later date.  Since this identifier can be
used to delete a message prematurely, it is vital  that  it  is  unique  to
avoid accidental deletion of the wrong message.


3.2.  Message_Priority

Messages are assigned a priority which governs the frequency at which  they
are  displayed.   Priorities  vary  from 0 upwards, 0 being the lowest (and
default) priority.  A priority of 1 (or higher) will display 50% more often
than priority 0.  The priority is set using the "PRiority" command

.PR  priority

where priority is a number indicating the desired priority.  If .PR is	not
used, a priority of 0 is assumed.


3.3.  Message_Display_Start_Date_and_Time

The date and time that the message is to display from  is  specified  using
the "FRom" command

.FR  dd/mm/yy  HH:MM

where dd, mm and yy are the day in the month (1-31), the month	(1-12)	and
the year (since 1900).	Hence for the year 1995 yy=95, but for 2002 yy=102.
HH:MM represents the time in 24-hour format:  HH is the  hours	since  mid-
night, and MM is the minutes.  The HH:MM may be omitted, in which case mid-
night (00:00) is assumed.  For example, April 3rd, 1990,  4:30pm  would  be
represented as

.FR 3/4/90 16:30

If the .FR command is omitted, then  the  message  will  begin	to  display
immediately.


3.4.  Message_Expiry_Date_and_Time

The message expiry date is specified similarly to the start date using	the
"TO" command

.TO  dd/mm/yy  HH:MM

where the parameters are as described above.  Unlike the .FR and  .PR  com-
mands,	.TO  is  not optional.	Following the .TO command, the message text
should start.


4.  Message_Format

A message consists of up to three lines of  display  text  (which  are	not
filled	or  wrapped),  interspersed with commands to alter the way in which
they are displayed.

The ldf formatter only understands the display environments .DS L,  .DS  C,
.DS  R, .TL and .SH.  By default, the formatter assumes .DS C since left or
right justified text can look a little strange on  such  a  small  display.
For  short messages, .TL may be appropriate since this will use boldface if
there is sufficient space.

4.1.  .DS_X

The "DiSplay" request generates output in roman typeface  without  wrapping
or  filling  out  the  line.  X may be L (Left) where output is against the
left margin, C (Centre) where each line is centred or R (Right) where  each
line  is  justified  against  the right margin.  The display environment is
conventionally terminated by a .DE (Display End) command.

e.g.

.DS C
Hello, World
.DE


4.2.  .TL_and_.SH

The "TitLe" request generates centred output  in  boldface,  again  without
wrapping  or  filling  out  the line.  The "Side Heading" request generates
left-justified output in boldface.  Boldface requested in this	manner	can
be cancelled by the formatter if lines become too long for display in bold-
face, in an attempt to fit the text on the line.  In this respect, request-
ing boldface in this manner is preferable to using \f3 as described below.


4.3.  In-Line_Font_Changes

The font can be changed using \f commands as  described  in  the  following
table:
 _________________________________________________________________________
|	       |		|					  |
|Preferred Form|Alternative Form|Effect 				  |
|______________|________________|_________________________________________|
|	       |		|					  |
|     \f1      |      \fR	|Roman font cancels italics, bold & uline |
|	       |		|					  |
|     \f2      |      \fI	|Sets italics font			  |
|	       |		|					  |
|     \f3      |      \fB	|Sets bold font 			  |
|	       |		|					  |
|     \f4      |      \fU	|Sets underlined font			  |
|______________|________________|_________________________________________|

Fonts 1 to 3 are as defined in troff.  troff leaves font 4 undefined, hence
\fU  is  not a valid command in troff, since troff cannot underline text in
this way.  In addition, unlike troff, a \f3 request (for example) does	not
cancel (say) a \f2 font.  Hence it is possible to mix italics, boldface and
underlining in any combination.  The only way to cancel a font is by speci-
fying	 roman,    or	 using	  \fP	 which	  also	  selects    roman,
even in .TL and .SH modes.

e.g.

.DS C
Start in roman, select \f2italics,
\f3bold italics\f1, even \f2\f3\f4bold,
underlined italics\f1.
.DE


4.4.  Special_Characters

The character set appears on screen as defined in the ASCII  character	set
with  the  exception of  the  hash  sign  which  appears as a  pound  sign,
just as with troff using a HP Laserjet+ printer.  In order to obtain a real
hash sign, ldf uses \#.  Other "escape" codes are used as shown in the fol-
lowing table:
	      ______________________________________________
	     |		       |			    |
	     | Escape sequence |    Appearance on screen    |
	     |_________________|____________________________|
	     |		       |			    |
	     |	     \#        |	     #		    |
	     |		       |			    |
	     |	     \e        |	     \		    |
	     |		       |			    |
	     |	     \E        |     Departmental logo	    |
	     |		       |			    |
	     |	     \&        | Zero-width, NULL character |
	     |_________________|____________________________|

The NULL character can be used where a new line is to be started with the .
character, for example:

.DS L
"This is ...
.DE
.DS R
\&... a quotation"
.DE

or for leaving completely blank lines.

Any other character following a \ will be displayed as if  the	\  was	not
present.


4.5.  Whitespace

The ldf formatter ignores whitespace at the beginning of  lines  (including
blank  lines)  allowing  more  flexibility in the layout of the input file.
Hence the following two files produce identical results:

.FI	rjf/gdp/whiteex1
.PR	0
.TO	7/3/90, 12:30.

.DS C
	Department of
	Electronic and
	Electrical Engineering
.DE


.FI rjf/gdp/whiteex2
.TO 7/3/90 12:30
Department of
Electronic and
Electrical Engineering

In order to leave a completely blank line on the display, the NULL  charac-
ter  must  be  used to ensure that ldf does not ignore the line completely.
Blankspace within a line is displayed character  for  character:  thus,  to
improve  the  appearance  of ends of sentences, two spaces can be put after
the full-stop.	Tab characters are reduced to spaces.


5.  Deleting_Messages_Prematurely

It may be necessary to remove a message from display before the message has
reached  its  expiry  date.  In these cases all that is required is to mail
gdpb with the "ERase" command

.ER  message_id

where message_id is the identity of the message to be deleted,	as  defined
by  the  .FI  line in the original message.  Note that messages can only be
deleted by the user that originally sent the message - i.e. the command  to
delete	the message must come from the same mailbox as the original message
did.   Note also that the case of the  identifier used with the .ER command
must exactly match that originally used with the .FI command.	The success
(or otherwise) of the deletion is  acknowledged in the usual way,  normally
in ten to twenty minutes.

As a final example, here is how to delete the  message	rjf/gdp/delex  from
lena:

$ mail gdpb
To: gdpb@lena.uucp
Subject:

.ER rjf/gdp/delex
^Z
Cc:
send, continue editing, discard [s,c,d]? s


6.  Obtaining_Help

To obtain a copy of these instructions, mail `gdpb' with the "HElp" command

.HE

The instructions will be sent in an acknowledgement file, usually within 20
minutes.


gdpb/laser_progs/help/readme
