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

/* Code in this file is all copyright 1998 by Daniel Bump. */

#include <stdio.h>
#include "liberty.h"
#include <assert.h>

static int stack[MAXSTACK][MAX_BOARD][MAX_BOARD]; /* stack */
static int stackmk[MAXSTACK],stackuk[MAXSTACK];

void pushgo()
{
  int i,j;
  
  if (showstack)
    fprintf(stderr,"        *** STACK before push: %d\n",stackp); 
  for (i=0;i<board_size;i++)             
    for (j=0;j<board_size;j++) {
      stack[stackp][i][j]=p[i][j];
      stackmk[stackp]=mk;
      stackuk[stackp]=uk;
    }
  stackp++;
}

void popgo()
{
  int i,j;
  
  stackp--;
  if (showstack)
    fprintf(stderr,"        *** STACK  after pop: %d\n",stackp);
  for (i=0;i<board_size;i++)             
    for (j=0;j<board_size;j++) {
      p[i][j]=stack[stackp][i][j];
      mk=stackmk[stackp];
      uk=stackuk[stackp];
    }
}

void asciimov(int i, int j,char *out)
{
  char l;
  int ii;
  if (!verbose) return;
  if (j < 8) {
    l = j + 65;
  }
  else {
    l = j + 66;
  }
  sprintf(out,"%c", l);
  ii = board_size - i;
  if (ii < 10)
    sprintf(out+1,"%1d", ii);
  else
    sprintf(out+1,"%2d", ii);
}

/* If si,sj points to a group with exactly one liberty
   readlad1 determines whether it can be saved by extending.
*/

int readlad1(int si,int sj,int *i,int *j) 
{
  int color;
  int ai,aj,bi,bj;
  int di,dj;
  char movename[3];

  asciimov(si,sj,movename);
  if (verbose) fprintf(stderr,"   check atari on %s.\n",movename);
  color=p[si][sj];
  countlib(si,sj,color);
  assert (lib==1);
  *i=libi[0];
  *j=libj[0];
  if (breakchn(si,sj,&di,&dj)) {
    asciimov(di,dj,movename);
    if (verbose) fprintf(stderr,"      part of chain is in atari at %s\n",movename);
    *i=di;
    *j=dj;
    return(1);
  }
  pushgo();
  asciimov(*i,*j,movename);
  if (verbose) fprintf(stderr,"     extending to %s.\n",movename);
  p[*i][*j]=color;
  countlib(si,sj,color);
  if (lib<2) {
    if (verbose) fprintf(stderr,"     it dies!\n",movename);
    popgo();
    return(0);
  }
  if (breakchn(si,sj,&di,&dj)) {
    asciimov(di,dj,movename);
    if (verbose) fprintf(stderr,"     part of chain is in atari\n");
    popgo();
    return(1);
  }
  examboard(3-color);
  countlib(si,sj,color);
  asciimov(*i,*j,movename);
  if (lib<2) {
    if (verbose) fprintf(stderr,"     it dies!\n");
    popgo();
    return(0);
  }
  if (lib>2) {
    if (verbose) fprintf(stderr,"     it lives at %s!\n",movename);
    popgo();
    return(1);
  }
  assert(lib==2);
  if (readlad2(si,sj,i,j)) {
    if (verbose) fprintf(stderr,"   DEAD!\n");
    popgo();
    return(0);
  } else {
    if (verbose) fprintf(stderr,"   ALIVE at %s!\n",movename);
    popgo();
    return(1);
  }
}

/* If si,sj points to a group with exactly two liberties
   readlad2 determines whether it can be captured in ladder.
   If yes, *i,*j is the killing move. */

int readlad2(int si,int sj,int *i,int *j) 
{
  int color;
  int ai,aj,bi,bj;
  int di,dj;
  char movename[3];

  asciimov(si,sj,movename);
  if (verbose) 
    fprintf(stderr, "   checking attack on %s with 2 liberties\n",movename);
  color=p[si][sj];
  countlib(si,sj,color);
  assert(lib==2);
  ai=libi[0];
  aj=libj[0];
  bi=libi[1];
  bj=libj[1];
  pushgo();
  asciimov(ai,aj,movename);
  p[ai][aj]=3-color;
  examboard(color);
  countlib(ai,aj,3-color);
  if (lib==0) {
    if (verbose) fprintf(stderr, "     %s is not a legal move\n", movename);
  } else {
    if (verbose) fprintf(stderr,"     trying atari at %s\n",movename);
    if (!readlad1(si,sj,&di,&dj)) {
      *i=ai;
      *j=aj;
      if (verbose) fprintf(stderr,"   DEAD at %s!!\n",movename);
      popgo();
      return(1);
    } 
  }
  popgo();
  if (verbose) fprintf(stderr,"     first atari didn't work.\n");
  pushgo();
  asciimov(bi,bj,movename);
  p[bi][bj]=3-color;
  examboard(color);
  countlib(bi,bj,3-color);
  if (lib==0) {
    if (verbose) fprintf(stderr, "     %s is not a legal move\n", movename);
  } else {
    p[bi][bj]=3-color;
    examboard(color);
    if (verbose) fprintf(stderr,"     trying atari at %s\n",movename);
    if (!readlad1(si,sj,&di,&dj)) {
      if (verbose) fprintf(stderr,"   DEAD!!\n");
      *i=bi;
      *j=bj;
      popgo();
      return(1);
    } 
  }
  popgo();
  if (verbose) fprintf(stderr,"   ALIVE!!\n");
  return(0);
}

/* if si,sj points to a group with two liberties, savestone2 determines
   whether the group can be saved by extending, or by capturing part of
   its surrounding chain. A group is considered safe if either part of
   the surrounding chain may be captured, or if it can get 3
   liberties. It is presumed that the opponent could kill if tenuki.
   If both extensions work, it prefers the one which maximizes 
   liberties. */

int savestone2(int si,int sj,int *i,int *j)
{
  int color,ti,tj,ai,aj,bi,bj,firstlib;
  char movename[3];

  color=p[si][sj];
  countlib(si,sj,color);
  assert(lib==2);
  ai=libi[0];
  aj=libj[0];
  bi=libi[1];
  bj=libj[1];
  asciimov(si,sj,movename);
  if (verbose) 
    fprintf(stderr,"   trying to rescue %s\n",movename);
  asciimov(ai,aj,movename);
  if (verbose) 
    fprintf(stderr,"     trying extension to %s\n",movename);
  pushgo();
  p[ai][aj]=color;
  countlib(si,sj,color);
  firstlib=lib;
  if ((lib==2)&&(readlad2(si,sj,&ti,&tj))) firstlib=0;
  asciimov(ai,aj,movename);
  if (verbose) 
    fprintf(stderr,"   %s rescues, but is other move better?\n",movename);
  popgo();
  pushgo();
  p[bi][bj]=color;
  countlib(si,sj,color);
  asciimov(bi,bj,movename);
  if ((firstlib>lib)&&(firstlib>2))
    {
      if (verbose) 
	fprintf(stderr,"   %s is not superior to first move\n",movename);
      *i=ai;
      *j=aj;
      popgo();
      return(1);
    }
  if ((lib>2)||((lib==2)&&(!readlad2(si,sj,&ti,&tj)))) {
    if (verbose) 
      fprintf(stderr,"   %s also rescues, we'll use it\n",movename);
    *i=bi;
    *j=bj;
    popgo();
    return(1);
  }
  if (firstlib>1) {
    if (verbose) 
      fprintf(stderr,"   %s doesn't work, use first move\n",movename);
    *i=ai;
    *j=aj;
    popgo();
    return(1);
  }
  if (verbose) fprintf(stderr,"   failed to find rescuing move.",movename);    
  popgo();
  return(0);
}

/* si,sj points to a group. breakchn(si,sj,*i,*j) returns 1 if
   there is a move which immediately captures part of the surrounding 
   chain. Then *i,*j points to the capturing location. Only one 
   capturing move is tried. */

int breakchn(int si,int sj,int *i,int *j)
{
  int color, adc;
  int ai,aj;
  char movename[3];
  
  asciimov(si,sj,movename);
  if (verbose) 
    fprintf(stderr,"     chain surrounding %s (liberties): ",movename);
  color=p[si][sj];
  chainlinks(si,sj);
  for (adc=0;adc<adj;adc++) {
    ai=adji[adc];
    aj=adjj[adc];
    asciimov(ai,aj,movename);
    countlib(ai,aj,p[ai][aj]);
    if (verbose) fprintf(stderr,"%s(%d) ",movename,lib);
    if (lib==1) {
      *i=libi[0];
      *j=libj[0];
      asciimov(*i,*j,movename);
      if (verbose) fprintf(stderr,"\n");
      if (verbose) fprintf(stderr,"       killing move at %s\n",movename);     
      return(1);
      }
  }
  if (verbose) fprintf(stderr,"\n");
  return(0);
}

/* si,sj points to a group. breakchn2(si,sj,*i,*j) returns 1 if
   there is a link in the surrounding chain having only two
   liberties which can be capture. */

/* chainlinks returns (in adji,adjj arrays) the chain surrounding
   the group at i,j. Adapted from count. Marks only one stone on
   each link */

void chainlinks(int m, int n)
{
 int i, j;

 for (i = 0; i < board_size; i++)
   for (j = 0; j < board_size; j++)
     ma[i][j] = 1;
     ml[i][j] = 1;
   adj=0;
 chain(m, n);
}

void chain(int i,int j)
{
  int color;

  ma[i][j] = 0;
  color=p[i][j];

  if (i != 0)
    {
      if ((p[i - 1][j] == (3-color)) && ma[i - 1][j] && ml[i-1][j])
	{
	  adji[adj]=i-1;
	  adjj[adj]=j;
	  ++adj;
	  ma[i - 1][j] = 0;
	  count(i-1,j,3-color);
	} 
      else {
	if ((p[i - 1][j] == color) && ma[i - 1][j]) {
	  chain(i - 1, j);
	} 
      }
    }
/* check South neighbor */
  if (i != 18)
    {
      if ((p[i + 1][j] == (3-color)) && ma[i + 1][j] && ml[i+1][j])
	{
	  adji[adj]=i+1;
	  adjj[adj]=j;
	  ++adj;
	  ma[i + 1][j] = 0;
	  count(i+1,j,3-color);
	}
      else {
	if ((p[i + 1][j] == color) && ma[i + 1][j]) {
	  chain(i + 1, j);
	}
      }
    }
  /* check West neighbor */
  if (j != 0)
    {
      if ((p[i][j - 1] == (3-color)) && ma[i][j - 1] && ml[i,j-1])
	{
	  adji[adj]=i;
	  adjj[adj]=j-1;
	  ++adj;
	  ma[i][j - 1] = 0;
	  count(i,j-1,3-color);
	}
      else {
	if ((p[i][j - 1] == color) && ma[i][j - 1]) {
	  chain(i, j - 1);
	}
      }
    }
  /* check East neighbor */
  if (j != 18)
    {
      if ((p[i][j + 1] == (3-color)) && ma[i][j + 1] && ml[i][j+1])
	{
	  adji[adj]=i;
	  adjj[adj]=j+1;
	  ++adj;
	  ma[i][j + 1] = 0;
	  count(i,j+1,3-color);
	}
      else {
	if ((p[i][j + 1] == color) && ma[i][j + 1]) {
	  chain(i, j + 1);
	}
      }
    }
} 

/* legal(i,j) determines whether my (color) at (i,j) is legal */

int legal(int i,int j,int color)
{
  int legit=0;
  int m,n;
  char movename[3];

  if (p[i][j]!=EMPTY) return(0);
  pushgo();
  p[i][j]=color;
  examboard(3-color);
  for (m=0;m<board_size;m++)
    for (n=0;n<board_size;n++)
      if (plast[m][n][color-1]!=p[m][n]) {
	legit=1; 
      } 
  asciimov(m,n,movename);
  if ((!legit)&&(verbose))
      fprintf(stderr,"     %s would violate the ko rule\n",movename);
  if (legit) {
    countlib(i,j,color);
    if (lib==0) {
      if (verbose)
	fprintf(stderr,"     %s would be suicide\n",movename);
      legit=0;
    }
  }
  popgo();
  return(legit);
}
  
