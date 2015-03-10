/* -------------------------------------------------

//                                                 
//  88888888888           ad88888ba   88888888888  
//  88                   d8"     "8b  88           
//  88                   Y8,          88           
//  88aaaaa  ,adPPYYba,  `Y8aaaaa,    88aaaaa      
//  88"""""  ""     `Y8    `"""""8b,  88"""""      
//  88       ,adPPPPP88          `8b  88           
//  88       88,    ,88  Y8a     a8P  88           
//  88       `"8bbdP"Y8   "Y88888P"   88888888888  
//                                                 
//

Pedro {Paredes, Ribeiro} - DCC/FCUP
----------------------------------------------------
Isomorphism Utilities

Adapted from gtrieScanner - http://www.dcc.fc.up.pt/gtries/

---------------------------------------------------- */

#include "Isomorphism.h"
#include "GraphUtils.h"

// Static variables
setword Isomorphism::workspace[WORKSPACE_SIZE];
int Isomorphism::n,Isomorphism::m;
set *Isomorphism::gv;
graph Isomorphism::g[MAXN*MAXM];
int Isomorphism::lab[MAXN];
int Isomorphism::ptn[MAXN];
int Isomorphism::orbits[MAXN];
bool Isomorphism::dir;

DEFAULTOPTIONS(options);
statsblk(stats);
graph mm[MAXN*MAXM];


void Isomorphism::initNauty(int size, bool directed) {
  n = size;  
  m = (n + WORDSIZE - 1) / WORDSIZE;
  nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
  
  dir = directed;

  options.getcanon = TRUE;
  options.writeautoms = FALSE;
  if (dir) options.digraph = TRUE;
  else     options.digraph = FALSE;
}

void Isomorphism::finishNauty() {
  nauty_freedyn();
  naugraph_freedyn();
  nautil_freedyn();
}


void Isomorphism::canonicalStrNauty(Graph *myg, int *v, char *s) {
  int i, j, aux;
  
  for (i=0; i<n; i++) {
    gv = GRAPHROW(g,i,m);
    EMPTYSET(gv,m);
    for (j=0; j<n; j++)
      if (myg->hasEdge(v[i], v[j])) ADDELEMENT(gv,j);
  }

  nauty(g,lab,ptn,NULL,orbits,&options,&stats,
        workspace,WORKSPACE_SIZE,m,n,mm);
  
  aux=0;
  for (i=0; i<n; i++) {
    gv = GRAPHROW(mm,i,m); 
    for (j=0; j<n; j++)
      s[aux++] = ISELEMENT(gv,j)?'1':'0';
  }
  s[aux]=0;
}
