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

#ifndef _ISOMORPHISM_
#define _ISOMORPHISM_

#include "Common.h"
#include "Graph.h"

#define MAXN MAX_MOTIF_SIZE
#define WORKSPACE_SIZE MAXN*160

#include "nauty/nauty.h"

class Isomorphism {
 private:  
  static bool dir;
  static setword workspace[WORKSPACE_SIZE];
  static int n,m,lab[MAXN], ptn[MAXN], orbits[MAXN];
  static set *gv;
  static graph g[MAXN*MAXM];

  static void _goCan(int x, int pos, const char *in, 
		     char *perm, char *used,
		     char *current, char *best, int size);

  static void _goCan2(int pos, const char *in, int *perm, bool *used, char *best, int size, char *current);
    
 public:
  static void initNauty(int size, bool dir);
  static void finishNauty();

  static void canonicalStrNauty(Graph *g, int *v, char *s);

  static void canonicalNauty(const char *in, char *out, int size);
  static void canonicalBigger(const char *in, char *out, int size);
  static void canonicalBigger2(const char *in, char *out, int size);
  static void canonicalBasedNauty(const char *in, char *out, int size); // GT Canon
  static void canonicalBasedNautyReverse(const char *in, char *out, int size);
  static void canonicalRandom(const char *in, char *out, int size);
};

#endif


