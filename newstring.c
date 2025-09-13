#include <assert.h>
#include "newlib.h"

extern int ms[MAX_BOARD][MAX_BOARD];
extern int mc[MAX_BOARD][MAX_BOARD];
extern int mcc[MAX_BOARD][MAX_BOARD];
extern int mcl[MAX_BOARD][MAX_BOARD];
extern int mt[MAX_BOARD][MAX_BOARD];
extern int mp[MAX_BOARD][MAX_BOARD];

int decidestatus()
{
  int i,j;
  assert(stackp==1);
  for (i=1;i<board_size;i++)
    for (j=1;j<board_size;j++)
      ms[i][j]=0;
  for (i=1;i<board_size;i++)
    for (j=1;j<board_size;j++)
      if ((!ms[i][j])&&p[i][j][1]) {
	decidestring(i,j);
      }
  assert(stackp==1);
}

int decidestring(int i,int j)
{
  int m,n,q,r,u,v;
  stringstatus stat;
  for (u=1;u<board_size;u++)
    for (v=1;v<board_size;v++)
      tflags[u][v]=0;
  if (attack(i,j,&m,&n)) {
    if (defend(i,j,&q,&r)) {
      attackmove[i][j].x=m;
      attackmove[i][j].y=n;
      defendmove[i][j].x=q;
      defendmove[i][j].y=r;
      stat=critical;	
    } else {
      stat=dead;
    }
  } else {
    stat=alive;
  }	
  propogatestatus(i,j,stat); 
  for (u=1;u<board_size;u++)
    for (v=1;v<board_size;v++)
      flags[u][v][i][j]=tflags[u][v];
}	

int propogatestatus(int i,int j,stringstatus stat) 
{
  assert(p[i][j][1]!=0);
  status[i][j]=stat;
  ms[i][j]=1;
  if ((i>0)&&(p[i-1][j][1]==p[i][j][1])&&(!ms[i-1][j])) 
    propogatestatus(i-1,j,stat);	
  if ((i<18)&&(p[i+1][j][1]==p[i][j][1])&&(!ms[i+1][j])) 
    propogatestatus(i+1,j,stat);
  if ((j>0)&&(p[i][j-1][1]==p[i][j][1])&&(!ms[i][j-1])) 
    propogatestatus(i,j-1,stat);    
  if ((j<18)&&(p[i][j+1][1]==p[i][j][1])&&(!ms[i][j+1])) 
    propogatestatus(i,j+1,stat);    
}

/* counts liberties of a string, and returns their location in an array */

int countlib(int i,int j,struct vertex *adj)
{
  int result=0;
  assert (p[i][j][stackp]!=0);
  for (i=1;i<board_size;i++)
    for (j=1;j<board_size;j++)
      mc[i][j]=0;
  countlibrec(i,j,mc,&result,adj);
  return result;
}  

int countlibrec(int i,int j,int *result,struct vertex *adj)
{
  mc[i][j]=1;
  if ((i>0)&&(p[i-1][j][stackp]==0)&&(!mc[i-1][j])) {
    (*result)++;
    mc[i-1][j]=1;
    adj[*result].x=i-1;
    adj[*result].y=j;
  }
  if ((i<18)&&(p[i+1][j][stackp]==0)&&(!mc[i+1][j])) {
    (*result)++;
    mc[i+1][j]=1;
    adj[*result].x=i+1;
    adj[*result].y=j;
  }
  if ((j>0)&&(p[i][j-1][stackp]==0)&&(!mc[i][j-1])) {
    (*result)++;
    mc[i][j-1]=1;
    adj[*result].x=i;
    adj[*result].y=j-1;
  }
  if ((j<18)&&(p[i][j+1][stackp]==p[i][j][stackp])&&(!mc[i][j+1])) {
    (*result)++;
    mc[i][j+1]=1;
    adj[*result].x=i;
    adj[*result].y=j+1;
  }
  if ((i>0)&&(p[i-1][j][stackp]==p[i][j][stackp])&&(!mc[i-1][j])) 
    countlibrec(i-1,j,result,adj);	
  if ((i<18)&&(p[i+1][j][stackp]==p[i][j][stackp])&&(!mc[i+1][j])) 
    countlibrec(i+1,j,result,adj);	
  if ((j>0)&&(p[i][j-1][stackp]==p[i][j][stackp])&&(!mc[i][j-1])) 
    countlibrec(i,j-1,result,adj);	
  if ((j<18)&&(p[i][j+1][stackp]==p[i][j][stackp])&&(!mc[i][j+1])) 
    countlibrec(i,j+1,result,adj);	
  return *result;
}

/* same as countlib, but does not track found liberties */

int bcountlib(int i,int j)
{
  int result=0;
  assert (p[i][j][stackp]!=0);
  for (i=1;i<board_size;i++)
    for (j=1;j<board_size;j++)
      mc[i][j]=0;
  bcountlibrec(i,j,&result);
  return result;
}  

int bcountlibrec(int i,int j,int *result)
{
  mc[i][j]=1;
  if ((i>0)&&(p[i-1][j][stackp]==0)&&(!mc[i-1][j])) {
    (*result)++;
    mc[i-1][j]=1;
  }
  if ((i<18)&&(p[i+1][j][stackp]==0)&&(!mc[i+1][j])) {
    (*result)++;
    mc[i+1][j]=1;
  }
  if ((j>0)&&(p[i][j-1][stackp]==0)&&(!mc[i][j-1])) {
    (*result)++;
    mc[i][j-1]=1;
  }
  if ((j<18)&&(p[i][j+1][stackp]==p[i][j][stackp])&&(!mc[i][j+1])) {
    (*result)++;
    mc[i][j+1]=1;
  }
  if ((i>0)&&(p[i-1][j][stackp]==p[i][j][stackp])&&(!mc[i-1][j])) 
    bcountlibrec(i-1,j,result);	
  if ((i<18)&&(p[i+1][j][stackp]==p[i][j][stackp])&&(!mc[i+1][j])) 
    bcountlibrec(i+1,j,result);	
  if ((j>0)&&(p[i][j-1][stackp]==p[i][j][stackp])&&(!mc[i][j-1])) 
    bcountlibrec(i,j-1,result);	
  if ((j<18)&&(p[i][j+1][stackp]==p[i][j][stackp])&&(!mc[i][j+1])) 
    bcountlibrec(i,j+1,result);	
  return *result;
}


int attack(int i,int j,int *m,int *n)
{
  int lib,r,maxcandidate,u,v;
  int maxdist,maxchain,can;
  struct vertex adj[MAXLIBERTIES];
  struct vertex candidates[MAXCANDIDATES];
  lib=countlib(i,j,adj);
  if (lib>3) return(0);
  if ((stackp>k1)&&(lib>2)) return(0);
  if (lib==1) {
    *m=adj[0].x;
    *n=adj[0].y;
    return(1);
  }
  if (lib==2) {
    if (stackp<k2)
      maxdist=3;
    else
      maxdist=2;
    maxchain=1;
  }
  if (lib==3) {
    if (stackp<k3)
      maxdist=4;
    else if (stackp<k2)
      maxdist=3;
    else maxdist=2;
    if (stackp<k2)
      maxchain=2;
  }
  maxcandidate=0;
  markcandidates(i,j,maxdist,mcl,mcc);
  for (r=1;r<=maxchain;r++) {
    getcandidatesc(r,&maxcandidate,candidates);
  }
  for (r=1;r<=maxdist;r++) {
    getcandidatesl(r,&maxcandidate,candidates);
  }
  for (can=0;can<maxcandidate;can++) {
    pushgo(candidates[can].x,candidates[can].y,3-p[i][j][stackp]);
    if (!defend(i,j,&u,&v)) {
      *m=candidates[can].x;
      *n=candidates[can].y;
      popgo();
      return(1);
    } else {
      popgo();
    }
  }
  return(0);
}

int defend(int i,int j,int *m,int *n)
{
  int lib;
  struct vertex adj[MAXLIBERTIES];
  lib=countlib(i,j,adj);
  if (lib>3) return(1);
  if ((stackp>k1)&&(lib>2)) return(1);

}

/* markcandidates fills marks moves which are either empty and at 
distance <= maxdist (in the array mcl) from the string at (i,j), or which
are liberties of a string in the surrounding chain.

Marking is as follows. In mcc, empty space surrounding the group is 
marked with the integer distance to the group. In mcl, liberties 
adjacent to strings of the surrounding chain are marked with the
number of liberties in the element of the surrounding chain having
the smallest number of liberties.
*/

int markcandidates(int i,int j,int maxdist)
{
  int m,n,d;
  for (m=0;m<board_size;m++)
    for (n=0;n<board_size;n++) {
      mt[m][n]=0;
      mcl[m][n]=0;
      mcc[m][n]=0;
    }
  markadjrec(i,j,maxdist);
  for (d=2;d<=maxdist;d++)
    for (m=0;m<board_size;m++)
      for (n=0;n<board_size;n++) 
	if ((p[i][j][stackp]==0)&&(mcl[i][j]==0))
	  if (((i>0)&&(mcl[i-1][j]!=0))||	
	      ((i<18)&&(mcl[i+1][j]!=0))||	
	      ((j>0)&&(mcl[i][j-1]!=0))||	
	      ((j<18)&&(mcl[i][j+1]!=0))) {
	    mcl[i][j]=d;
	    tflags[i][j]=1;
	  }
}	

int markadjrec(int i,int j,int maxdist)
{
  mt[i][j]=1;
  if ((i>0)&&(mt[i-1][j]==0)) {
    if (p[i-1][j][stackp]==p[i][j][stackp]) {
      markadjrec(i-1,j,maxdist);
    } else if (p[i-1][j][stackp]==0) {
      mcl[i-1][j]=1;
      tflags[i-1][j]=1;
    } else {
      assert(p[i-1][j][stackp]==3-p[i][j][stackp]);
      markchain(i-1,j);
    }
  }
  if ((i<18)&&(mt[i+1][j]==0)) {
    if (p[i+1][j][stackp]==p[i][j][stackp]) {
      markadjrec(i+1,j,maxdist);
    } else if (p[i+1][j][stackp]==0) {
      mcl[i+1][j]=1;
      tflags[i+1][j]=1;
    } else {
      assert(p[i+1][j][stackp]==3-p[i][j][stackp]);
      markchain(i+1,j);
    }
  }
  if ((j>0)&&(mt[i][j-1]==0)) {
    if (p[i][j-1][stackp]==p[i][j][stackp]) {
      markadjrec(i,j-1,maxdist);
    } else if (p[i][j-1][stackp]==0) {
      mcl[i][j-1]=1;
      tflags[i][j-1]=1;
    } else {
      assert(p[i][j-1][stackp]==3-p[i][j][stackp]);
      markchain(i,j-1);
    }
  }
  if ((j<18)&&(mt[i][j+1]==0)) {
    if (p[i][j+1][stackp]==p[i][j][stackp]) {
      markadjrec(i,j+1,maxdist);
    } else if (p[i][j+1][stackp]==0) {
      mcl[i][j+1]=1;
      tflags[i][j+1]=1;
    } else {
      assert(p[i][j+1][stackp]==3-p[i][j][stackp]);
      markchain(i,j+1);
    }
  }
}

int markchain(int i,int j)
{
  struct vertex adj[MAXLIBERTIES];
  int n,lib;
  int result=0;
  for (n=0;n<(lib=countlibrec(i,j,&result,adj));n++)
    if ((mcc[adj[n].x][adj[n].y]==0)||(lib<mcc[adj[n].x][adj[n].y]))
      mcc[adj[n].x][adj[n].y]=lib;	
}

void getcandidatesc(int r,int *current,struct vertex *candidates)
{
  int i;int j;
  assert ((*current)<MAXCANDIDATES);
  for (i=0;i<board_size;i++)	
    for (j=0;j<board_size;j++)
      if ((mcc[i][j]==r)&&(legal(i,j))) {
	candidates[*current].x=i;
	candidates[*current].y=j;
	(*current)++;
      }	
}

void getcandidatesl(int r,int *current,struct vertex *candidates)
{
  int i;int j;
  assert ((*current)<MAXCANDIDATES);
  for (i=0;i<board_size;i++)	
    for (j=0;j<board_size;j++)
      if ((mcl[i][j]==r)&&(legal(i,j))) {
	candidates[*current].x=i;
	candidates[*current].y=j;
	(*current)++;
      }	
}

int legal(i,j)
{
  if (!p[i][j][stackp-1]) 
    return (1);
  else return (0);     /* in this case check more carefully for ko violation. */
}                     /* for the time being, assume this case is ko. */
  
void pushgo(int i,int j,int color)
{
  int m,n,lib;
  int result;
  struct vertex adj[MAXLIBERTIES];
  stackp++;
  for (m=0;m<board_size;m++)
    for (n=0;n<board_size;n++) {
      p[m][n][stackp]=p[m][n][stackp-1];
      mp[m][n]=0;
    }
  assert (p[i][j][stackp]==0);
  p[i][j][stackp]=color;
  for (m=0;m<board_size;m++)
    for (n=0;n<board_size;n++)
      if ((p[m][n][stackp])&&(!mp[m][n]))
	{
	  lib=0;
	  bcountlibrec(m,n,&lib);
	  if (lib==0) removerec(m,n,p[i][j][stackp]);
	}
}

/* removes all stones of given color starting at p[i][j][stackp] */

void removerec(int m,int n,int color)
{
  assert(p[m][n][stackp]==color);
  p[m][n][stackp]=0;
  if ((m>0)&&(p[m-1][n][stackp]==color))
    removerec(m-1,n,color);
  if ((m<18)&&(p[m+1][n][stackp]==color))
    removerec(m+1,n,color);
  if ((n>0)&&(p[m][n-1][stackp]==color))
    removerec(m,n-1,color);
  if ((n<18)&&(p[m][n+1][stackp]==color))
    removerec(m,n+1,color);
}  

void popgo()
{
  stackp--;
}
