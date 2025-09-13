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

/*---------------------------------------------------
  sethand.c -- Set up handicap pieces for black side
----------------------------------------------------*/

#include "liberty.h"

void sethand(int i)
/* set up handicap pieces */
{
  if (i > 1)
    {
      p[3][15] = BLACK;
      p[15][3] = BLACK;
    }
/* This is not where Man Li puts the handicap stones. I moved
   them to traditional location. 2 and 3 stone handicap is 
   affected. */
  if (i > 2)
    {
      p[3][3] = BLACK;
      if (i > 3)
	{
	  p[15][15] = BLACK;
	  if (i == 5)
	    p[9][9] = BLACK;
	  else
	    if (i > 5)
	      {
		p[9][15] = BLACK;
		p[9][3] = BLACK;
		if (i == 7)
		  p[9][9] = BLACK;
		else
		  if (i > 7)
		    {
		      p[15][9] = BLACK;
		      p[3][9] = BLACK;
		      if (i > 8)
			p[9][9] = BLACK;
		      if (i > 9)
			{p[2][2] = 2;
			if (i > 10)
			  {p[16][16] = 2;
			  if (i > 11)
			    {p[2][16] = 2;
			    if (i > 12)
			      {p[16][2] = 2;
			      if (i > 13)
				{p[6][6] = 2;
				if (i > 14)
				  {p[12][12] = 2;
				  if (i > 15)
				    {p[6][12] = 2;
				    if (i > 16) {
				      p[12][6] = 2;
                                      if (i > 17) {
                                        p[3][6] = 2;
                                        if (i > 18) {
                                          p[6][3] = 2;
                                        }
                                      }
                                    }
                                    
				    }
				  }
				}
			      }
			    }
			  }
			}
		    }
	      }
	}
    }
}


