#define MAX_BOARD 19
#define MAXSTACK 80
#define MAXLIBERTIES 240
#define MAXCANDIDATES 60

#define k1 30 /* beyond this depth, 3 liberties is considered alive */
#define k2 15 /* beyond this depth, less searching for tesujis */
#define k3 10  /* below this depth, consider many possibilities */

struct vertex {
  int x;
  int y;
};

int p[MAX_BOARD][MAX_BOARD][MAXSTACK];
int stackp;
short int flags[MAX_BOARD][MAX_BOARD][MAX_BOARD][MAX_BOARD];
short int tflags[MAX_BOARD][MAX_BOARD];
struct vertex attackmove[MAX_BOARD][MAX_BOARD];
struct vertex defendmove[MAX_BOARD][MAX_BOARD];

typedef enum {alive,dead,critical,unknown,empty} stringstatus;
stringstatus status[MAX_BOARD][MAX_BOARD];

void getcandidatesc(int r,int *current,struct vertex *candidates);
void getcandidatesl(int r,int *current,struct vertex *candidates);
int bcountlibrec(int i,int j,int *result);
void removerec(int m,int n,int color);
void pushgo();
void popgo();
