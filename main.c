/* This is LIBERTY, a Go program. LIBERTY is descended from Gnugo 1.2,
   written by Man Lung Li and published by the Free Software Foundation.
   Revisions were made by Daniel Bump (bump@math.stanford.edu) resulting 
   in this program LIBERTY.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation - version 2.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License in
   file COPYING for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc., 59
   Temple Place - Suite 330, Boston, MA 02111, USA */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "liberty.h"
#include "gmp.h"
#include "interface.h"

int main(int argc,
         char *argv[])
  {
   FILE *fp;
   Gmp  *ge;
   int i, j, m, n;
   char move[10], ans[5];
   time_t tm;
   int seed;
   GmpResult  message;
   const char  *error;
   char movename[3];

   /* verbose=1; */ /* change this to 0 to turn off the yakkity-yak */
   verbose = 0;
   showstack=0;
   fprintf(stderr, "\n\
\n\
                   This is LIBERTY, a Go program\n\
\n\
  Gnugo 1.2 was written by Man Li (manli@cs.uh.edu) and copyrighted\n\
  by the Free Software Foundation. Extensive changes have been made\n\
  by Daniel Bump (bump@math.stanford.edu) resulting in this program\n\
  LIBERTY.\n\
\n\
  This program has ABSOLUTELY NO WARRANTY.  See COPYING for detail.  \n\
  This is free software and you are welcome to redistribute it; see \n\
  COPYING for copying conditions. \n\
\n\
");

#if 0
   ge = gmp_create(0, 1);
   gmp_startGame(ge, -1, -1, 5.5, -1, -1);
   do  {
     message = gmp_check(ge, 1, NULL, NULL, &error);
   } while ((message == gmp_nothing) || (message == gmp_reset));
   if (message == gmp_err)  {
     fprintf(stderr, "gnugo-gmp: Error \"%s\" occurred.\n", error);
     exit(1);
   } else if (message != gmp_newGame)  {
     fprintf(stderr, "gnugo-gmp: Expecting a newGame, got %s\n",
	     gmp_resultString(message));
     exit(1);
   }
#endif

/* init opening pattern numbers to search */

   for (i = 0; i < 9; i++)
     opn[i] = 1;
   opn[4] = 0;

/* init board */
   for (i = 0; i < board_size; i++)
     for (j = 0; j < board_size; j++)
       p[i][j] = EMPTY;
/* init global variables */
   mk = 0;  uk = 0;


/* init global variables */
   play = 1;
   pass = 0;
   mik = -1; mjk = -1;
   uik = -1; ujk = -1;

   board_size = DEFAULT_BOARD_SIZE;

/* start random number seed */
   srand(time (0));

   play_gtp();
   return 0;

   i=gmp_handicap(ge);
   if (verbose) fprintf(stderr,"handicap=%d\n",i);
   sethand(i);
   if (gmp_iAmWhite(ge))
     {
       mymove = 1;   /* computer white */
       umove = 2;   /* human black */
       if (i)
	 {
	   genmove(&i, &j); 
	   gmp_sendMove(ge, j, i);
	   
	   p[i][j] = mymove;
	   asciimov(i,j,movename);
	   if (verbose)
	     fprintf(stderr,"\nmy move: %s\n\n",movename);
	 }
     }
   else
     {
       mymove = 2;   /* computer black */
       umove = 1;   /* human white */
       if (i == 0)
	 {
	   genmove(&i, &j); 
	   gmp_sendMove(ge, j, i);
	   p[i][j] = mymove;
	   asciimov(i,j,movename);
	   if (verbose)
	     fprintf(stderr,"\nmy move: %s\n\n",movename);
	 }
     }
   
   if (verbose) showboard(); /* display board after computer's move  only */

/* main loop */
   while (play > 0)
     {
       for (m=0;m<board_size;m++)
	 for (n=0;n<board_size;n++)
	   plast[m][n][mymove-1]=p[m][n];   /* save position to recognize ko */
       
       message = gmp_check(ge, 1, &j, &i, &error);
       
       if (play > 0)
	 {
	   if (message!=gmp_pass)   /* not pass */
	     {
	       asciimov(i,j,movename);
	       if (verbose) 
		     fprintf(stderr,"\nyour move: %s\n\n",movename);
	       p[i][j] = umove;
	       examboard(mymove);	 /* remove my dead pieces */
	       if (verbose) showboard(); /* display board after opponent's move*/
	     }
	   if (pass != 2)
	     {
	       genmove(&i, &j);  
	       
	       if (i >= 0)   /* not pass */
		 {
		   for (m=0;m<board_size;m++)
		     for (n=0;n<board_size;n++)
		       plast[m][n][umove-1]=p[m][n];   /* save position to recognize ko */
		   gmp_sendMove(ge, j, i);
		   p[i][j] = mymove;
		   asciimov(i,j,movename);
		   if (verbose)
		     fprintf(stderr,"\nmy move: %s\n\n",movename);
		   examboard(umove);   /* remove your dead pieces */
		 } else {
		   gmp_sendPass(ge);   /* pass */
		 }
	     }
	   showboard();
	 }
       if (pass == 2) {
	 play = 0;	/* both pass then stop game */
	 gmp_sendPass(ge);   /* pass  */
	 message = gmp_check(ge, 1, &j, &i, &error);
	 gmp_sendPass(ge);   /* pass again */
       }
     }
   fprintf(stderr,"Thank you for the game!\n");
   return 0;
  }  /* end main */


