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

#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define MAXLIBS 360
#define MAXDRAGONS 360
#define MAXSTACK 80
#define OTHER_COLOR(color)  (WHITE+BLACK-(color))

/* public functions */

void count(int i,int j,int color);
void countlib(int m,int n,int color);
void eval(int color);
void examboard(int color);
int findopen(int m,int n,int i[],int j[],int color,int minlib, int *ct);
int shapes(int *i,int *j,int *val);
int defender(int *i,int *j,int *val);
int attacker(int *i, int *j, int *val);
void genmove(int *i,int *j);
int matchpat(int m, int n,int *i,int *j, int *val);
int opening(int *i, int *j,int *cnd, int type);
int openregion(int i1, int j1, int i2, int j2);
void showboard(void);
void showinst(void);
void sethand(int i);
void pushgo();
void popgo();
void asciimov(int i,int j, char *movename);
int readlad1(int si,int sj,int *i,int *j);
int readlad2(int si,int sj,int *i,int *j);
int breakchn(int si,int sj,int *i,int *j);
void chainlinks(int m,int n);
void chain(int i,int j);
void dragon();
void markworm(int i,int j,int color);
void amalgamate_dragons();
void trycut(int i,int j,int k,int l);
void trycut1(int i,int j,int k,int l);
void superize(int drag1, int drag2);
int genus(int dragnum);
void makedragons();
void copydragons();
void connectdragons();
void recitedragons();

/* add by karino2 */
int legal(int i,int j,int color);
int savestone2(int si,int sj,int *i,int *j);
int fpopen(int *i, int *j, int *val);


#define MAX_BOARD 19
#define MIN_BOARD 5
#define DEFAULT_BOARD_SIZE 19
int board_size;
int p[MAX_BOARD][MAX_BOARD];  /* go board */
int l[MAX_BOARD][MAX_BOARD];  /* liberty of current color */
int ma[MAX_BOARD][MAX_BOARD]; /* working matrix for marking */
int ml[MAX_BOARD][MAX_BOARD]; /* working matrix for marking */
int mymove, umove;        /* computer color, opponent color */
int play;                 /* game state */
int pass;                 /* pass indicator */
int mik, mjk;             /* location of computer stone captured */
int uik, ujk;             /* location of opponent stone captured */
int mk, uk;               /* no. of stones captured by computer and oppoent */
int opn[9];               /* opening pattern flag */
int lib;                  /* current stone liberty */
int libi[MAXLIBS],libj[MAXLIBS];     /* array of liberties found */
int adj;                   /* number of stones in surrounding chain */
int adji[100],adjj[100];   /* stones in surrounding chain */
int openover;              /* opening is over */
int stackp;                /* stack pointer */
int verbose;               /* bore the opponent */
int showstack;             /* bore the programmer */
int dragondebug;           /* time for little Smaugie's flea bath */
int size;                  /* cardinality of a group */
int plast[MAX_BOARD][MAX_BOARD][2];      /* position before last move for ko */
int worm[MAX_BOARD][MAX_BOARD]; 
int drag[MAX_BOARD][MAX_BOARD]; 
int wsize;
int nextw;
int dragongenus;
int maxdragon;
int miniv;
int maxiv;
int minjv;
int maxjv;

typedef enum {alive,dead,strong,weak,unknown,deleted} dragonstatus;

struct ddata {
  int color;
  int size;
  int genus;
  int origini;
  int originj;
  int mini;
  int maxi;
  int minj;
  int maxj;
  int liberties;
  int bonus;
  dragonstatus status;
};

struct ddata worms[MAXDRAGONS];   
struct ddata dragons[MAXDRAGONS]; 

