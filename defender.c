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

int defender(int *i,    /* row number of next move */
              int *j,    /* column number of next move */
              int *val)  /* value of next move */
/* find move if any pieces is threaten */
{
  int m, n, minlib, ti, tj, tval, groupsize, mylib;
  unsigned char ll[board_size][board_size];
  char movename[3];

  if (verbose) fprintf(stderr,"Defender is THINKING!\n");
  *i = -1;   *j = -1;	 *val = 0;
  for (m=0;m<board_size;m++) 
    for (n=0;n<board_size;n++) {
      if (p[m][n]==mymove) {
	tval=0;
	countlib(m,n,mymove);
	groupsize=size;
	mylib=lib;
	if (mylib==1) {
	  if (readlad1(m,n,&ti,&tj)) {
	    if (legal(ti,tj,mymove)) {
	      asciimov(ti,tj,movename);
	      if (verbose) 
		fprintf(stderr,"   saving move found at %s!\n",movename);
	      tval=90+groupsize;
	    }
	  }
	}
	if (mylib==2) {
	  if (readlad2(m,n,&ti,&tj)) {
	    if (legal(ti,tj,mymove)) {
	      asciimov(ti,tj,movename);
	      if (verbose) 
		fprintf(stderr,"   %s captures. Can we prevent?\n",movename);
	      if (savestone2(m,n,&ti,&tj)) {
		if (verbose) 
		  fprintf(stderr,"   saving move found!\n",movename);
		tval=89+groupsize;
	      }
	    }
	  }
	}
	if ((tval > *val)||((tval >0)&&(tval==*val)&(rand()%2))) {
	  if (legal(ti,tj,mymove))
	    {
	      *val=tval;
	      *i=ti;
	      *j=tj;
	    }
	}
      }
    }
  if (*val>0) {
    return(1);
  } else {
    return(0);
  }
}



