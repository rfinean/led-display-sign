/***************************************************************************
 * Laser Display : Restore Backup Message Files Subroutine  :  restore8.c  *
 *                                                                         *
 * Returns any M*.BAK files to M*.LDM form                                 *
 *                                                                         *
 *      R S Barker      27/2/90    7/4/90                                  *
 ***************************************************************************/


#include "header.h"

void restore_files()

{
int count,count1;
struct find_t find;
char tempname[FNAMLNG],                                 /* Filename with .BAK extension */
     messname[FPATHLNG],mbakname[FPATHLNG];             /* Filenames with paths prefixed */

    printf("Restoring filenames\n");
    system("dir b:\\mess /w");
    if (!_dos_findfirst(MESSDIR BACKFILE,_A_NORMAL,&find))      /* Search for first M*.BAK file */
	{
	    for (count=0; (tempname[count]=find.name[count])!=DOT; count++);
	    for (count1=0; count1<4; tempname[++count]=DISPFEXT[count1++]);
	    sprintf(messname,"%s%s",MESSDIR,find.name);
	    sprintf(mbakname,"%s%s",MESSDIR,tempname);
	    rename (messname,mbakname);

	    while (!_dos_findnext(&find))                       /* Search for subsequent M*.BAK files */
		{
		    for (count=0; (tempname[count]=find.name[count])!=DOT; count++);
		    for (count1=0; count1<4; tempname[++count]=DISPFEXT[count1++]);
		    sprintf(messname,"%s%s",MESSDIR,find.name);
		    sprintf(mbakname,"%s%s",MESSDIR,tempname);
		    rename (messname,mbakname);
		}
	}

    system ("dir b:\\mess /w");
}
