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

/* This function is taken over, largely unchanged, from Man Li's 
   original code. */

#include <stdio.h>
#include "liberty.h"
#include "patterns.h"

#define abs(x) ((x) < 0 ? -(x) : (x))
#define line(x) (abs(x - 9))

int matchpat(int m,     /* row origin */
             int n,     /* column origin */
             int *i,    /* row number of next move */
             int *j,    /* column number of next move */
             int *val)  /* next move value */
/* match pattern and get next move */
{
/* transformation matrices */
 static int trf [8][2][2] = {
   {{1, 0}, {0, 1}},   /* linear transformation matrix */
   {{1, 0}, {0, -1}},  /* invert */
   {{0, 1}, {-1, 0}},  /* rotate 90 */
   {{0, -1}, {-1, 0}}, /* rotate 90 and invert */
   {{-1, 0}, {0, 1}},  /* flip left */
   {{-1, 0}, {0, -1}}, /* flip left and invert */
   {{0, 1}, {1, 0}},   /* rotate 90 and flip left */
   {{0, -1}, {1, 0}}   /* rotate 90, flip left and invert */
 };
 int k, my, nx, ll, r, cont;
 int ti, tj, tval;
 char movename[3];

 *i = -1;   *j = -1;   *val = -1;
 for (r = 0; r < PATNO; r++)
/* try each pattern */
    for (ll = 0; ll < pat[r].trfno; ll++)
/* try each orientation transformation */
      {
       k = 0;  cont = 1;
       while ((k != pat[r].patlen) && cont)
/* match each point */
	 {
/* transform pattern real coordinate */
	  nx = n + trf[ll][0][0] * pat[r].patn[k].x
		 + trf[ll][0][1] * pat[r].patn[k].y;
	  my = m + trf[ll][1][0] * pat[r].patn[k].x
		 + trf[ll][1][1] * pat[r].patn[k].y;

/* outside the board */
	  if ((my < 0) || ( my > board_size-1) || (nx < 0) || (nx > board_size-1))
	    {
	     cont = 0;
	     break;
	   }
	  switch (pat[r].patn[k].att) {
	  case 0 : if (p[my][nx] == EMPTY)	/* open */
		      break;
		   else
		     {
		      cont = 0;
		      break;
		    }
	  case 1 : if (p[my][nx] == umove)  /* your piece */
		      break;
		   else
		     {
		      cont = 0;
		      break;
		    }
	  case 2 : if (p[my][nx] == mymove)  /* my piece */
		      break;
		   else
		     {
		      cont = 0;
		      break;
		    }
	  case 3 : if (p[my][nx] == EMPTY)	/* open for new move */
		    {
		      /*  lib=0; */
		     countlib(my, nx, mymove);	/* check liberty */
		     if (lib > 1)  /* move o.k. */
		       {
			ti = my;
			tj = nx;
			break;
		       }
		     else
		       {
			cont = 0;
			break;
		      }
		     }
		   else
		     {
		      cont = 0;
		      break;
		    }
	  case 4 : if ((p[my][nx] == EMPTY)  /* open on edge */
		       && ((my == 0) || (my == board_size-1) || (nx == 0) || (nx == board_size-1)))
		      break;
		   else
		     {
		      cont = 0;
		      break;
		    }
	  case 5 : if ((p[my][nx] == umove)  /* your piece on edge */
		       && ((my == 0) || (my == board_size-1) || (nx == 0) || (nx == board_size-1)))
		      break;
		   else
		     {
		      cont = 0;
		      break;
		    }
	  case 6 : if ((p[my][nx] == mymove)  /* my piece on edge */
		       && ((my == 0) || (my == board_size-1) || (nx == 0) || (nx == board_size-1)))
		      break;
		   else
		     {
		      cont = 0;
		      break;
		    }
		 }
	  ++k;
	 }
	 if (cont)   /* match pattern */
	   {
	    tval = pat[r].patwt;
	    if ((r >= 8) && (r <= 13))	/* patterns for expand region */
	      {
	       if (line(ti) > 7)  /* penalty on line 1, 2 */
		  tval--;
	       else
		  if ((line(ti) == 6) || (line(ti) == 7))
		     tval++;	/* reward on line 3, 4 */

	       if (line(tj) > 7)  /* penalty on line 1, 2 */
		  tval--;
	       else
		  if ((line(tj) == 6) || (line(tj) == 7))
		     tval++;	/* reward on line 3, 4 */
	     }
	    if (tval > *val)
	      {
		asciimov(ti,tj,movename);
		if (verbose) 
		  fprintf(stderr,"   pattern %d found with value %d at %s\n",r,tval,movename);
		*val = tval;
		*i = ti;
		*j = tj;
	      }
	   }
      }
 if (*val > 0)	/* pattern matched */
   return 1;
 else  /* match failed */
    return 0;
}  /* end matchpat */





