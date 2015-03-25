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
Base FaSE implementation

---------------------------------------------------- */

#ifndef _FASE_
#define _FASE_

#include "Common.h"
#include "Graph.h"
#include "GTrie.h"
#include "Isomorphism.h"
#include "Random.h"

/*! This class implements the basic FaSE subgraph enumeration algorithm */
class Fase
{
 private:
  static int K;
  static int graphSize;
  static Graph *G;
  static int *sub;
  static int subNum;
  static char globStr[MAXS];
  static char s[20 * 20 + 1];
  static short **extCpy;
  static int clique[6];

  static char* LSLabel(int w, int subSize);
  static void ExtendSubgraph(int extNum, int subSize);
  static void GraphletsExtendSubgraph(int extNum, int node);

 public:
  static int typeLabel;
  static long long int MotifCount;
  static bool directed;
  static int cliqueCount;
  static long long int count[50];
  static int type[MAXGRAPHS];

  static int graphlets[30];
  static long long int orbits[73];

  static long long int getTypes();
  static long long int getLeafs();
  static long long int getNodes();
  static void destroy();
  static void listClasses(FILE* f);
  static void listTree(FILE* f);
  static void EnumerateSubgraphs(Graph *_G, int _K);
  static void GraphletsCount(Graph *_G, int _K);

  static void updateEquations(int graph, int* sub);

  static void calcGraphletFrequency();
  static void calcOrbitFrequency();
  static void solveEquations();
  static void buildCommonNodes();
};

#endif
