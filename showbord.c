/* This is LIBERTY, a Go program. LIBERTY is descended from Gnugo 1.2,
   written by Man Lung Li and published by the Free Software Foundation.
   Extensive revisions were made by Daniel Bump (bump@math.stanford.edu) 
   resulting in this program LIBERTY.                              

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation - version 2.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License in
   file COPYING for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc., 59
   Temple Place - Suite 330, Boston, MA 02111, USA 
*/

/* This file contains slightly modified code from Gnugo 1.2 */

/*-------------------------------------------------------------
  showbord.c -- Show current go board and playing information
-------------------------------------------------------------*/

#include <stdio.h>
#include "liberty.h"

void showboard(void)
/* show go board */
{
   int i, j, ii;

   if (!verbose) return;
/* p = 0 for empty ,p = 1 for white piece, p = 2 for black piece */
   fprintf(stderr,"   A B C D E F G H J K L M N O P Q R S T\n");
/* row 19 to 17 */
   for (i = 0; i < 3; i++)
     {
      ii = board_size - i;
      fprintf(stderr,"%2d",ii);

      for (j = 0; j < board_size; j++)
	if (p[i][j] == EMPTY)
	   fprintf(stderr," -");
	else if (p[i][j] == WHITE)
		fprintf(stderr," O");
	     else fprintf(stderr," X");

      fprintf(stderr,"%2d",ii);
      fprintf(stderr,"\n");
     }
/* row 16 */
   fprintf(stderr,"16");

   for (j = 0; j < 3; j++)
     if (p[3][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[3][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[3][3] == 0)
      fprintf(stderr," +");
   else if (p[3][3] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 4; j < 9; j++)
     if (p[3][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[3][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[3][9] == EMPTY)
      fprintf(stderr," +");
   else if (p[3][9] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 10; j < 15; j++)
     if (p[3][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[3][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[3][15] == EMPTY)
      fprintf(stderr," +");
   else if (p[3][15] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 16; j < board_size; j++)
     if (p[3][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[3][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   fprintf(stderr,"16");
   if (umove == 1)
      fprintf(stderr,"     Your color: White O\n");
   else
      if (umove == 2)
	 fprintf(stderr,"     Your color: Black X\n");
      else
	 fprintf(stderr,"\n");
/* row 15 to 11 */
   for (i = 4; i < 9; i++)
     {
      ii = board_size - i;
      fprintf(stderr,"%2d",ii);

      for (j = 0; j < board_size; j++)
	if (p[i][j] == EMPTY)
	   fprintf(stderr," -");
	else if (p[i][j] == WHITE)
		fprintf(stderr," O");
	     else fprintf(stderr," X");

      fprintf(stderr,"%2d",ii);
      if (i == 4)
	{
	 if (mymove == 1)
	    fprintf(stderr,"     My color:   White O\n");
	 else
	    if (mymove == 2)
	       fprintf(stderr,"     My color:   Black X\n");
	    else
	       fprintf(stderr,"\n");
       }
      else
	 if (i != 8)
	    fprintf(stderr,"\n");
	 else
	    fprintf(stderr,"     You have captured %d pieces\n", mk);
     }
/* row 10 */
   fprintf(stderr,"10");

   for (j = 0; j < 3; j++)
     if (p[9][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[9][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[9][3] == EMPTY)
      fprintf(stderr," +");
   else if (p[9][3] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 4; j < 9; j++)
     if (p[9][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[9][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[9][9] == EMPTY)
      fprintf(stderr," +");
   else if (p[9][9] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 10; j < 15; j++)
     if (p[9][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[9][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[9][15] == EMPTY)
      fprintf(stderr," +");
   else if (p[9][15] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 16; j < board_size; j++)
     if (p[9][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[9][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   fprintf(stderr,"10");
   fprintf(stderr,"     I have captured %d pieces\n", uk);
/* row 9 to 5 */
   for (i = 10; i < 15; i++)
     {
      ii = board_size - i;
      fprintf(stderr,"%2d",ii);

      for (j = 0; j < board_size; j++)
	if (p[i][j] == EMPTY)
	   fprintf(stderr," -");
	else if (p[i][j] == WHITE)
		fprintf(stderr," O");
	     else fprintf(stderr," X");

      fprintf(stderr,"%2d",ii);
      fprintf(stderr,"\n");
     }
/* row 4 */
   fprintf(stderr," 4");

   for (j = 0; j < 3; j++)
     if (p[15][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[15][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[15][3] == EMPTY)
      fprintf(stderr," +");
   else if (p[15][3] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 4; j < 9; j++)
     if (p[15][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[15][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[15][9] == EMPTY)
      fprintf(stderr," +");
   else if (p[15][9] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 10; j < 15; j++)
     if (p[15][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[15][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   if (p[15][15] == EMPTY)
      fprintf(stderr," +");
   else if (p[15][15] == WHITE)
	   fprintf(stderr," O");
	else fprintf(stderr," X");

   for (j = 16; j < board_size; j++)
     if (p[15][j] == EMPTY)
	fprintf(stderr," -");
     else if (p[15][j] == WHITE)
	     fprintf(stderr," O");
	  else fprintf(stderr," X");

   fprintf(stderr," 4");
   fprintf(stderr,"\n");
/* row 3 to 1 */
   for (i = 16; i < board_size; i++)
     {
      ii = board_size - i;
      fprintf(stderr,"%2d",ii);

      for (j = 0; j < board_size; j++)
	if (p[i][j] == EMPTY)
	   fprintf(stderr," -");
	else if (p[i][j] == WHITE)
		fprintf(stderr," O");
	     else fprintf(stderr," X");

      fprintf(stderr,"%2d",ii);
      fprintf(stderr,"\n");
     }
   fprintf(stderr,"   A B C D E F G H J K L M N O P Q R S T\n\n");
 }  /* end showboard */
