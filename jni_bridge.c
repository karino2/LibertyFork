#include <jni.h>

/*
 Similar to gtp.c and play_gtp.c
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <time.h>
#include <stdlib.h>

#include "liberty.h"

#define EMPTY        0
#define WHITE        1
#define BLACK        2

static int gtp_boardsize = -1;

static char g_output_line[1024];

static void clear_board()
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
}

void
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_initNative (
	JNIEnv*	env,
	jclass clasz
	)
{
  // verbose = 1;
  verbose = 0;
  showstack=0;

  board_size = DEFAULT_BOARD_SIZE;
  srand(time (0));
  clear_board();
}

void
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_setKomi (
	JNIEnv*	env,
	jclass clasz,
	jfloat komi_
	)
{
  // komi 使って無さそう？
  // komi = komi_;
}


void
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_clearBoard (
	JNIEnv*	env,
	jclass clasz
	)
{
  clear_board();
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

/*
  x, y is start from upperleft, 0 origin.

  A3 means
  x = 0
  y = BOARD_SIZE-3
*/
jboolean
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_doMove (
	JNIEnv*	env,
	jclass clasz,
  jint x, 
  jint y,
  jboolean isBlack
	)
{
  int color = isBlack ? BLACK : WHITE;
  play_internal(color, y, x);
  return JNI_TRUE;
}

void
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_doPass (
	JNIEnv*	env,
	jclass clasz,
  jboolean isBlack
	)
{
  int color = isBlack ? BLACK : WHITE;
  mymove = color;
  umove = OTHER_COLOR(mymove);  
}

/*
  x | (y<<16),
  pass is -1.
*/
jint
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_genMoveInternal (
	JNIEnv*	env,
	jclass clasz,
  jboolean isBlack
	)
{
  int color = isBlack ? BLACK : WHITE;
  int i, j;

  mymove = color;
  umove = OTHER_COLOR(mymove);
  genmove(&i, &j);
  play_internal( color, i, j );

  if (i >= 0) {
    return j | (i<<16);
  }
  else {
    // PASS
    return -1;
  }
}


void
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_setBoardSize (
	JNIEnv*	env,
	jclass clasz,
	jint boardSize
	)
{
  board_size = boardSize;
}

static int
oprintf(const char *format_string, ...)
{
  va_list arguments;
  int result;
  char buf[256];

  va_start(arguments, format_string);
  result = vsprintf(buf, format_string, arguments);
  va_end(arguments);

  strcat(g_output_line, buf);

  return result;
}


jstring
Java_io_github_karino2_paoogo_goengine_liberty_LibertyNative_debugInfo (
	JNIEnv*	env,
	jclass clasz
	)
{
  g_output_line[0] = '\0';
  oprintf("no deb info\n");
  /*
  oprintf("komi=%f\n", get_komi());
  oprintf("boardsize=%d\n", get_boardsize());
  oprintf("movenumber=%d\n", get_movenumber());
  oprintf("depth=%d\n", depth);
  */
  return (*env)->NewStringUTF (env, g_output_line);
}
