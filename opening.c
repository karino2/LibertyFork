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

/* This file contains exclusively code which is left over from Gnugo 1.2 */

/*-----------------------------------
  opening.c -- Select opening moves
-----------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "liberty.h"

int opening(int *i,
            int *j,
            int *cnd,
            int type)
/* get move for opening from game tree */
{
 struct tnode {
   int i, j, ndct, next[8];
  };

 static struct tnode tree[] = {
  {-1, -1, 8, { 1, 2, 3, 4, 5, 6, 7, 20}},	/* 0 */
  {2, 3, 2, { 8, 9}},
  {2, 4, 1, {10}},
  {3, 2, 2, {11, 12}},
  {3, 3, 6, {14, 15, 16, 17, 18, 19}},
  {3, 4, 1, {10}},  /* 5 */
  {4, 2, 1, {13}},
  {4, 3, 1, {13}},
  {4, 2, 0},
  {4, 3, 0},
  {3, 2, 0},  /* 10 */
  {2, 4, 0},
  {3, 4, 0},
  {2, 3, 0},
  {2, 5, 1, {10}},
  {2, 6, 1, {10}},  /* 15 */
  {3, 5, 1, {10}},
  {5, 2, 1, {13}},
  {5, 3, 1, {13}},
  {6, 2, 1, {13}},
  {2, 2, 0}  /* 20 */
};
int m;

/* get i, j */
 if ((type == 1) || (type == 3))
    *i = board_size-1 - tree[*cnd].i;   /* inverted */
 else
    *i = tree[*cnd].i;
 if ((type == 2) || (type == 3))
    *j = board_size-1 - tree[*cnd].j;   /* reflected */
 else
    *j = tree[*cnd].j;
 if (tree[*cnd].ndct)  /* more move */
   {
    m = rand() % tree[*cnd].ndct;  /* select move */
    *cnd = tree[*cnd].next[m];	/* new	current node */
    return 1;
  }
 else
    return 0;
}  /* end opening */

int fpopen(int *i,    /* row number of next move */
           int *j,        /* column number of next move */
           int *val)      /* value of next move */
/* find pattern to match for next move */
{
 int m, n;
 int ti, tj, tval;
 static int cnd, mtype;  /* game tree node number, move type */
/* mtype = 0, basic; 1, inverted; 2, reflected; 3, inverted & reflected */

/* open game then occupy corners */

 if (opn[4])   /* continue last move */
   {
    opn[4] = 0;  /* clear flag */
    if (opening(i, j, &cnd, mtype)) opn[4] = 1; /* more move then reset flag */
    if (p[*i][*j] == EMPTY)  /* valid move */
      {
       *val = 80;
       return 1;
     }
    else
      opn[4] = 0;
  }

 if (opn[0])   /* Northwest corner */
   {
     opn[0] = 0;  /* clear flag */
     if (openregion(0, 0, 5, 5))
      {
	cnd = 0;
	mtype = 0;
	opening(i, j, &cnd, mtype);  /* get new node for next move */
	if (opening(i, j, &cnd, mtype)) opn[4] = 1;
	*val = 83;
	return 1;
     }
 }
 if (opn[1])   /* Southwest corner */
   {
    opn[1] = 0;
    if (openregion(board_size-6, 0, board_size-1, 5))
      {
       cnd = 0;
       mtype = 1;
       opening(i, j, &cnd, mtype);  /* get new node for next move */
       if (opening(i, j, &cnd, mtype)) opn[4] = 1;
       *val = 83;
       return 1;
     }
  }
 if (opn[2])   /* Northeast corner */
   {
    opn[2] = 0;
    if (openregion(0, board_size-6, 5, board_size-1))
      {
       cnd = 0;
       mtype = 2;
       opening(i, j, &cnd, mtype);  /* get new node for next move */
       if (opening(i, j, &cnd, mtype)) opn[4] = 1;
       *val = 83;
       return 1;
     }
  }

 if (opn[3])   /* Northeast corner */
   {
    opn[3] = 0;
    if (openregion(board_size-6, board_size-6, board_size-1, board_size-1))
      {
       cnd = 0;
       mtype = 3;
       opening(i, j, &cnd, mtype);  /* get new node for next move */
       if (opening(i, j, &cnd, mtype)) opn[4] = 1;
       *val = 83;
       return 1;
     }
  }

/* occupy edges */
 if (opn[5])   /* North edge */
   {
    opn[5] = 0;
    if (board_size==19 && openregion(0, 6, 4, 11))
      {
       *i = 3;
       *j = 9;
       *val = 80;
       return 1;
     }
  }

 if (opn[6])   /* South edge */
   {
    opn[6] = 0;
    if (board_size==19 && openregion(18, 6, 14, 11))
      {
       *i = 15;
       *j = 9;
       *val = 80;
       return 1;
     }
  }

 if (opn[7])   /* West edge */
   {
    opn[7] = 0;
    if (board_size==19 && openregion(6, 0, 11, 4))
      {
       *i = 9;
       *j = 3;
       *val = 80;
       return 1;
     }
  }

 if (opn[8])   /* East edge */
   {
    opn[8] = 0;
    if (board_size==19 && openregion(6, 18, 11, 14))
      {
       *i = 9;
       *j = 15;
       *val = 80;
       return 1;
     }
  }
 return 0;
 } 

int openregion(int i1,
               int j1,
               int i2,
               int j2)
/* check if region from i1, j1 to i2, j2 is open */
{
 int minx, maxx, miny, maxy, x, y;

/* exchange upper and lower limits */

 if (i1 < i2)
   {
    miny = i1;
    maxy = i2;
  }
 else
   {
    miny = i2;
    maxy = i1;
  }

 if (j1 < j2)
   {
    minx = j1;
    maxx = j2;
  }
 else
   {
    minx = j2;
    maxx = j1;
  }

/* check for empty region */
 for (y = miny; y <= maxy; y++)
   for (x = minx; x <= maxx; x++)
     if (p[y][x] != EMPTY) return 0;
 return 1;
}  /* end openregion */

