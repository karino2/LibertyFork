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

/* the function shapes is essentially the same as Man Li's function findpatn. 
   I renamed it and made only minor changes, with one exception: the pattern
   database itself has been very revamped. */

int shapes(int *i, int *j, int *val)
{
 int m, n;
 int ti, tj, tval, ui, uj;
 int oi,oj, oval;
 char movename[3];

 *i = -1;
 *j = -1;
 *val = -1;

 if (verbose) fprintf(stderr,"Shape Seer is THINKING!\n");
 if (!openover) {
   if (fpopen(&oi,&oj,&oval)) {
     asciimov(oi,oj,movename);
     if (verbose) 
       fprintf(stderr," opening found at %s with value %d\n",movename,oval);
   } else {
     oval=-1;
   }
 }
 for (m = 0; m < board_size; m++)
   for (n = 0; n < board_size; n++)

/* modification from Man Li's code: allow p[m][n]==umove.
   this means that patterns can include {0,0,1} as well
   as {0,0,2}. Potentially slower to search, but this
   flexibility allows us to use patterns in which there
   is no original stone of mycolor. -Daniel Bump 11/12/98.
*/

     if (((p[m][n] == mymove)||(p[m][n] == umove)) &&
         (matchpat(m, n, &ti, &tj, &tval) && ((tval > *val)||(tval == *val)&&(tval>0)&&(rand())%2)))
       {
	 pushgo();
	 p[ti][tj]=mymove;
	 countlib(ti,tj,mymove);
	 if ((lib<2)||((lib==2)&&(readlad2(ti,tj,&ui,&uj)))) {
	   if (verbose) fprintf(stderr,"   pattern rejected - move can be captured.\n");
	   popgo();
	 } else {
	   if (tval>30) tval+=rand()%5;
	   *val = tval;
	   *i = ti;
	   *j = tj;
	   popgo();
	 }
       }
 if (!openover &&(oval > *val)) {
   *val=oval;
   *i=oi;
   *j=oj;
 } else {
   openover=1;
 }
 if (*val > 0) {
   return 1;
 }
 else 
    return 0;
} 









