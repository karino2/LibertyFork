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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "liberty.h"

int attacker(int *i,    /* row number of next move */
             int *j,    /* column number of next move */
             int *val)  /* value of next move */
/* find opponent piece to capture or attack */
{
  int tval,ti,tj,m,n,ulib,groupsize,ui,uj;
  unsigned char ll[board_size][board_size];
  char movename[3];

  *val=0;
  if (verbose) fprintf(stderr,"Attacker is THINKING!\n");
  for (m=0;m<board_size;m++) 
    for (n=0;n<board_size;n++) {
      if (p[m][n]==umove) {
	countlib(m,n,umove);
	ulib=lib;
	groupsize=size;
	tval=0;
	if (ulib==1) {
	  ti=libi[0];
	  tj=libj[0];
	  if (legal(ti,tj,mymove)) {
	    asciimov(ti,tj,movename);
	    if (verbose)
	      fprintf(stderr,"    capturing move found at %s\n",movename);
	    if (verbose)
	      fprintf(stderr,"    try tenuki ...\n");
	    pushgo();
	    asciimov(ti,tj,movename);
	    if (verbose)
	      fprintf(stderr,"    ... opponent extends to %s\n",movename);
	    if (!legal(ti,tj,umove)) {
	      if (verbose) fprintf(stderr,"       opponent cannot legally extend.\n");
	    } else {
	      p[ti][tj]=umove;
	      countlib(m,n,umove);
	      if ((lib==1)||((lib==2)&&(readlad2(m,n,&ui,&uj)))) {
		if (verbose) fprintf(stderr,"    capturing move at %s is unnecessary\n",movename);
		tval=0;
	      } else {
		tval=90+groupsize;
		if (verbose) fprintf(stderr,"    capturing move at %s has value %d\n",movename,tval);
	      }
	    }
	    popgo();	    
	  }
	}
	if (ulib==2) {
	  if (readlad2(m,n,&ti,&tj)) {
	    if (legal(ti,tj,mymove)) {
	      asciimov(ti,tj,movename);
	      if (verbose)
		fprintf(stderr,"    capturing move found at %s ...\n",movename);
	      if (verbose)
		fprintf(stderr,"    ... try tenuki\n");
	      pushgo();
	      p[ti][tj]=umove;
	      countlib(m,n,umove);
	      if ((lib>2)||((lib==2)&&(savestone2(m,n,&ui,&uj)))) {
		tval=88+groupsize;
		if (verbose) fprintf(stderr,"   capturing move at %s with value %d\n",movename,tval);
		
	      } else {
		if (verbose) fprintf(stderr,"   capturing at %s is not needed\n",movename);
		tval=0;
	      }
	      popgo();
	    }
	  }
	}
	if ((tval > *val)||((tval == *val)&&(tval>0)&&(rand()%2))) {
	  *val=tval;
	  *i=ti;
	  *j=tj;
	}
      }
    }  
  if (*val>0) {
    return(1);
  } else {
    return(0);
  }
}

