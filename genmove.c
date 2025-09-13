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

#include <stdio.h>
#include <stdlib.h>
#include "liberty.h"

#define MAXTRY 400

void genmove(int *i,
             int *j)
/* generate computer move */
{
   int ti, tj, tval;
   char a;
   char movename[3];
   int ii, m, n, val;
   int try = 0;

   *i = -1;  *j = -1;  val = -1;

   if (attacker(&ti, &tj, &tval))
       if (tval > val)
	 {
	   asciimov(ti,tj,movename);
	   if (verbose) fprintf(stderr,"Attacker likes %s with value %d\n",movename,tval);
	   val = tval;
	   *i = ti;
	   *j = tj;
	}

/* save any piece if threaten */
   eval(mymove);
   if (defender(&ti, &tj, &tval))
       if ((tval > val)||((tval==val)&&(rand()%2)))
	 {
	   asciimov(ti,tj,movename);
	   if (verbose) fprintf(stderr,"Defender likes %s with value %d\n",movename,tval);
	   val = tval;
	   *i = ti;
	   *j = tj;
	}

/* try match local play pattern for new move */
   if (shapes(&ti, &tj, &tval))
       if ((tval > val)||((tval==val)&&(rand()%2)))
	 {
	   asciimov(ti,tj,movename);
	   if (verbose) fprintf(stderr,"Shape Seer likes %s with value %d\n",movename,tval);
	   val = tval;
	   *i = ti;
	   *j = tj;
	 }

/* no move found then pass */
   if (val < 0)
     {
       pass++;
       fprintf(stderr,"I pass.\n");
       *i = -1;
     }
}  /* end genmove */

