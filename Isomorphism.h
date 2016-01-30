#ifndef _ISOMORPHISM_
#define _ISOMORPHISM_

#define MAXS 200

// Limits for subgraph size
#define MIN_MOTIF_SIZE  3
#define MAX_MOTIF_SIZE 50
#define MAXMOTIF 20

#define MAXMAT 10000

//#include "Graph.h"

#define MAXN MAX_MOTIF_SIZE
#define WORKSPACE_SIZE MAXN*160

#include "Common.h"
#include "nauty/tnauty.h"

class Isomorphism {
 private:  
  bool dir;
  setword workspace[WORKSPACE_SIZE];
  int n,m, lab[MAXN], ptn[MAXN], orbits[MAXN];
  set *gv;
  graphnau g[MAXN*MAXM];
    
 public:
  void initNauty(int size, bool dir);
  void finishNauty();

  void canonicalStrNauty(string v, char *s);
};

#endif

