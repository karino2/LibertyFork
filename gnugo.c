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

/* This file contains exclusively code from Gnugo 1.2. This code
   contains minor modifications by Daniel Bump made in November, 1998
   and perhaps subsequently. */


#include <stdio.h>
#include <assert.h>
#include "liberty.h"

void eval(int color)
/* evaluate liberty of color pieces */
 {
  int i, j;

/* find liberty of each piece */
  for (i = 0; i < board_size; i++)
    for (j = 0; j < board_size; j++)
      if (p[i][j] == color)
	{
	  /* lib = 0; */
	 countlib(i, j, color);
	 l[i][j] = lib;
      }
}  /* end eval */

/* like eval, but only marks one stone in each group */

void examboard(int color) /* BLACK or WHITE */
/* examine pieces */
{
   int i, j, n;

/* find liberty of each piece */
   eval(color);

/* initialize piece captured */
   if (color == mymove)
     {
      mik = -1;
      mjk = -1;
    }
   else
     {
      uik = -1;
      ujk = -1;
    }
   n = 0; /* The number of captures this move for Ko purposes */

/* remove all piece of zero liberty */
   for (i = 0; i < board_size; i++)
     for (j = 0; j < board_size; j++)
       if ((p[i][j] == color) && (l[i][j] == 0))
	 {
	  p[i][j] = EMPTY;
/* record piece captured */
	  if (color == mymove)
	    {
	     mik = i;
	     mjk = j;
	     ++mk;
	   }
	  else
	    {
	     uik = i;
	     ujk = j;
	     ++uk;
	   }
	  ++n;  /* increment number of captures on this move */
	}
/* reset to -1 if more than one stone captured since  no Ko possible */
   if (color == mymove && n > 1)
     {
       mik = -1;   
       mjk = -1;
     }
   else if ( n > 1 )
     {
       uik = -1;
       ujk = -1;
     }
}  /* end examboard */


void countlib(int m,     /* row number 0 to 18 */
              int n,     /* column number 0 to 18 */
              int color) /* BLACK or WHITE */
/* count liberty of color piece at m, n */
{
 int i, j;

/* set all piece as unmarked */
 for (i = 0; i < board_size; i++)
   for (j = 0; j < board_size; j++)
     ml[i][j] = 1;
 lib=0;
 size=0;
/* count liberty of current piece */
 count(m, n, color);
}  /* end countlib */

void count(int i,     /* row number 0 to 18 */
           int j,     /* column number 0 to 18 */
           int color) /* BLACK or WHITE */
/* count liberty of color piece at location i, j
   and return value in lib. Return size of connected component
   in size. */
{
/* set current piece as marked */
  ml[i][j] = 0;
  size++;

/* check North neighbor */
  if (i != 0)
    {
      if ((p[i - 1][j] == 0) && ml[i - 1][j])
	{
	  if (lib<MAXLIBS) {
	    libi[lib]=i-1;
	    libj[lib]=j;
	  }
	  ++lib;
	  ml[i - 1][j] = 0;
	} 
      else {
	if ((p[i - 1][j] == color) && ml[i - 1][j]) {
	  count(i - 1, j, color);
	} 
      }
    }
/* check South neighbor */
  if (i != 18)
    {
      if ((p[i + 1][j] == 0) && ml[i + 1][j])
	{
	  if (lib<MAXLIBS) {
	    libi[lib]=i+1;
	    libj[lib]=j;
	  }
	  ++lib;
	  ml[i + 1][j] = 0;
	}
      else {
	if ((p[i + 1][j] == color) && ml[i + 1][j]) {
	  count(i + 1, j, color);
	}
      }
    }
  /* check West neighbor */
  if (j != 0)
    {
      if ((p[i][j - 1] == 0) && ml[i][j - 1])
	{
	  if (lib<MAXLIBS) {
	    libi[lib]=i;
	    libj[lib]=j-1;
	  }
	  ++lib;
	  ml[i][j - 1] = 0;
	}
      else {
	if ((p[i][j - 1] == color) && ml[i][j - 1]) {
	  count(i, j - 1, color);
	}
      }
    }
  /* check East neighbor */
  if (j != 18)
    {
      if ((p[i][j + 1] == 0) && ml[i][j + 1])
	{
	  if (lib<MAXLIBS) {
	    libi[lib]=i;
	    libj[lib]=j+1;
	  }
	  ++lib;
	  ml[i][j + 1] = 0;
	}
      else {
	if ((p[i][j + 1] == color) && ml[i][j + 1]) {
	  count(i, j + 1, color);
	}
      }
    }
}  /* end count */

