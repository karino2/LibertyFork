/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * This is GNU Go, a Go program. Contact gnugo@gnu.org, or see       *
 * http://www.gnu.org/software/gnugo/ for more information.          *
 *                                                                   *
 * Copyright 1999, 2000, 2001, 2002, 2003 and 2004                   *
 * by the Free Software Foundation.                                  *
 *                                                                   *
 * This program is free software; you can redistribute it and/or     *
 * modify it under the terms of the GNU General Public License as    *
 * published by the Free Software Foundation - version 2             *
 *                                                                   *
 * This program is distributed in the hope that it will be useful,   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 * GNU General Public License in file COPYING for more details.      *
 *                                                                   *
 * You should have received a copy of the GNU General Public         *
 * License along with this program; if not, write to the Free        *
 * Software Foundation, Inc., 59 Temple Place - Suite 330,           *
 * Boston, MA 02111, USA.                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "liberty.h"
#include "gmp.h"
#include "gtp.h"
#include "interface.h"

float komi = 0.0;

#define DECLARE(func) static int func(char *s)

DECLARE(gtp_set_boardsize);
DECLARE(gtp_set_komi);
DECLARE(gtp_reg_genmove);
DECLARE(gtp_genmove);
DECLARE(gtp_play);
DECLARE(gtp_clear_board);
DECLARE(gtp_list_commands);
DECLARE(gtp_debug);
DECLARE(gtp_quit);

static struct gtp_command commands[] = {
  {"boardsize",        	      gtp_set_boardsize},
  {"komi",        	      gtp_set_komi},
  {"clear_board",             gtp_clear_board},
  {"reg_genmove",             gtp_reg_genmove},
  {"genmove",             gtp_genmove},
  {"play",            	      gtp_play},
  {"list_commands",           gtp_list_commands},
  {"debug",            	      gtp_debug},
  {"quit",             	      gtp_quit},
  {NULL,                      NULL}
};


/***************************
 * Setting the board size. *
 ***************************/

/* Function:  Set the board size to NxN and clear the board.
 * Arguments: integer
 * Fails:     board size outside engine's limits
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int
gtp_set_boardsize(char *s)
{
  int boardsize;

  if (sscanf(s, "%d", &boardsize) < 1)
    return gtp_failure("boardsize not an integer");
  
  if (boardsize < MIN_BOARD || boardsize > MAX_BOARD) {
    return gtp_failure("unacceptable size");
  }
  board_size = boardsize;
  gtp_internal_set_boardsize(boardsize);
  return gtp_success("");
}


/***************************
 * Setting komi.           *
 ***************************/

/* Function:  Set the komi.
 * Arguments: float
 * Fails:     incorrect argument
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int
gtp_set_komi(char *s)
{
  if (sscanf(s, "%f", &komi) < 1)
    return gtp_failure("komi not a float");
  
  return gtp_success("");
}


/* Function:  Generate and play the supposedly best move for either color.
 * Arguments: color to move
 * Fails:     invalid color
 * Returns:   a move coordinate or "PASS" (or "resign" if resignation_allowed)
 *
 * Status:    GTP version 2 standard command.
 */
static int
gtp_reg_genmove(char *s)
{
  int i, j;
  int color;
  int n;

  n = gtp_decode_color(s, &color);
  if (!n)
    return gtp_failure("invalid color");

  mymove = color;
  umove = OTHER_COLOR(mymove);
  genmove(&i, &j);

  gtp_start_response(GTP_SUCCESS);
  gtp_print_vertex(i, j);
  return gtp_finish_response();
}

static void
play_internal(int color, int i, int j)
{
  int m, n;
  mymove = color;
  umove = OTHER_COLOR(mymove);
  if(i>=0) {
    for (m=0;m<board_size;m++)
      for (n=0;n<board_size;n++)
        plast[m][n][umove-1]=p[m][n];   /* save position to recognize ko */
    p[i][j] = mymove;
    examboard(umove);   /* remove your dead pieces */
  }

}

/* Function:  Play a stone of the given color at the given vertex.
 * Arguments: color, vertex
 * Fails:     invalid vertex, illegal move
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int
gtp_play(char *s)
{
  int i, j;
  int color;

  if (!gtp_decode_move(s, &color, &i, &j))
    return gtp_failure("invalid color or coordinate");

  play_internal( color, i, j );
  return gtp_success("");
}


/****************************
 * Administrative commands. *
 ****************************/

/* Function:  Quit
 * Arguments: none
 * Fails:     never
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int
gtp_quit(char *s)
{
  UNUSED(s);
  gtp_success("");
  return GTP_QUIT;
}


/* Start playing using the Go Text Protocol. */
void play_gtp()
{
  /* Make sure stdout is unbuffered. (Line buffering is also okay but
   * not necessary. Block buffering breaks GTP mode.)
   */
  setbuf(stdout, NULL);

  /* Inform the GTP utility functions about the board size. */
  gtp_internal_set_boardsize(board_size);

  /* Prepare pattern matcher and reading code. */
  gtp_main_loop(commands, stdin, NULL);
}

/*
  Added by karino2.
*/
#include <string.h>

static int
gtp_clear_board(char *s)
{
  for(int i = 0; i < MAX_BOARD; i++) {
    memset(&p[i][0], 0, sizeof(int)*MAX_BOARD);
    memset(&l[i][0], 0, sizeof(int)*MAX_BOARD);
    memset(&ma[i][0], 0, sizeof(int)*MAX_BOARD);
    memset(&ml[i][0], 0,sizeof(int)* MAX_BOARD);
    memset(&worm[i][0], 0, sizeof(int)*MAX_BOARD);
    memset(&drag[i][0], 0, sizeof(int)*MAX_BOARD);

    for(int j = 0; j < MAX_BOARD; j++) {
      memset(&plast[i][j][0], 0, sizeof(int)*2);
    }
  }

  memset(libi, 0, sizeof(int)*MAXLIBS);
  memset(libj, 0, sizeof(int)*MAXLIBS);
  memset(adji, 0, sizeof(int)*100);
  memset(adjj, 0, sizeof(int)*100);

  for (int i = 0; i < 9; i++)
     opn[i] = 1;
  opn[4] = 0;

  mk = 0;  uk = 0;
  play = 1;
  pass = 0;
  mik = -1; mjk = -1;
  uik = -1; ujk = -1;
  lib = 0;
  openover = 0;

  return gtp_success("");
}

static int
gtp_genmove(char *s)
{
  int i, j;
  int color;
  int n;

  n = gtp_decode_color(s, &color);
  if (!n)
    return gtp_failure("invalid color");

  mymove = color;
  umove = OTHER_COLOR(mymove);
  genmove(&i, &j);

  /* after genmove, play that move. */
  play_internal( color, i, j );

  gtp_start_response(GTP_SUCCESS);
  gtp_print_vertex(i, j);
  return gtp_finish_response();
}

static int
gtp_list_commands(char *s)
{
  gtp_start_response(GTP_SUCCESS);
  for(int i = 0; commands[i].name != NULL; i++) {
      gtp_printf("%s\n", commands[i].name);

  }  
  return gtp_finish_response();
}


static int
gtp_debug(char *s)
{
  gtp_start_response(GTP_SUCCESS);
  for(int row = 0; row < board_size; row++) {    
    for(int col = 0; col < board_size; col++) {
      gtp_printf("%d ", p[row][col]);
    }
    gtp_printf("\n");
  }
  return gtp_finish_response();
}


/*
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 2
 * End:
 */
