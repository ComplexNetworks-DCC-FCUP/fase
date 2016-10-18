#ifndef _FASE_
#define _FASE_

#include "Common.h"
#include "Graph.h"
#include "Random.h"
#include "Label.h"
#include "IGtrie.h"
#include "Isomorphism.h"

class Fase
{
 private:
  bool directed;
  bool sampling;
  Graph *graph;
  int K;
  int motifCount;
  IGtrie igtrie;

  int canonicalNumber;
  map<string, int> canonicalIndices;
  map<int, int> canonicalTypes;
  long long int totalLabel;

  int** vext;
  int* vextSz;
  int* vsub;
  double* sampProb;
  char sadjM[MAXMOTIF * MAXMOTIF + 1];
  char nauty_s[MAXMOTIF * MAXMOTIF + 1];

  Isomorphism* iso;

  void reduceCanonicalTypes();
  void expandEnumeration(int depth, int labelNode, long long int label);
  void runDouble(int a, int b, int increment, int edgeContent); // Temporal update
  void expandDouble(int increment, int depth, int labelNode, long long int label); // Temporal update
  int getSubgraphFrequency(pair<long long int, int> element, int testConnected = 0);

 public:
  Fase(Graph* _g, bool _directed);
  ~Fase();

  int getTypes();
  void runCensus(int _K);
  void countAddEdge(int a, int b); // Temporal update
  void countRemoveEdge(int a, int b); // Temporal update
  void initSampling(int sz, double* _sampProb);
  int getMotifCount() {return motifCount;}
  vector<pair<int, string> > subgraphCount();
  map<string, int> subgraphList();
};

#endif
