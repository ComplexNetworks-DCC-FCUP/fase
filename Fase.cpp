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

#include "Fase.h"

int Fase::K;
long long int Fase::MotifCount = 0;
int Fase::typeLabel;
bool Fase::directed;
Graph* Fase::G;
int *Fase::sub;
int Fase::subNum;
int Fase::graphSize;
short **Fase::extCpy;
char Fase::globStr[MAXS];
char Fase::s[20 * 20 + 1];
long long int Fase::count[50];
int Fase::type[MAXGRAPHS];
int Fase::clique[6] = {0, 0, 1, 3, 31, 511};
int Fase::cliqueCount;

//int types[MAXGRAPHS];
int Fase::graphlets[30];
long long int Fase::orbits[73];


int mapLabelToGraph[32] = {-1, 5, 12, 6,-1,-1,-1,-1,-1, 1,-1,
                            2, 7, 4,-1, 3,-1,15,19,16,21,
                           18,20,17,-1, 8,-1, 9,14,11,-1,
                           10};

int mapGraphToGraphlet[22] = {-1, 4, 6, 7, 6, 1, 2, 3, 6, 7, 8,
                           7, 1,-1, 6, 3, 6, 7, 5, 4, 6,
                           3};

int mapGraphletToOrbits[22][4] = {{-1,-1,-1,-1},{7,6,6,6}, {11,10,9,10}, {13,12,12,13}, {11,9,10,10}, {1, 2, 1, -1}, {3, 3, 3, -1}, {5,4,5,4},
                                  {11,10,10,9}, {13,13,12,12}, {14,14,14,14}, {13,12,13,12}, {2, 1, 1, -1}, {-1,-1,-1,-1}, {10,10,11,9},
                                  {5,5,4,4}, {10,11,9,10}, {12,13,12,13}, {8,8,8,8}, {6,7,6,6},
                                  {11,9,10,10}, {4,5,5,4}};

FILE* typeFile;

/****** ORCA ******/
#include <unordered_map>

long long **orbit;

int *tri;

typedef long long int64;
typedef pair<int,int> PII;

struct PAIR {
    int a, b;
    PAIR(int a0, int b0) { a=min(a0,b0); b=max(a0,b0); }
};

bool operator<(const PAIR &x, const PAIR &y) {
    if (x.a==y.a) return x.b<y.b;
    else return x.a<y.a;
}

bool operator==(const PAIR &x, const PAIR &y) {
    return x.a==y.a && x.b==y.b;
}

struct hash_PAIR {
    size_t operator()(const PAIR &x) const {
        return (x.a<<8) ^ (x.b<<0);
    }
};

struct TRIPLE {
    int a, b, c;
    TRIPLE(int a0, int b0, int c0) {
        a=a0; b=b0; c=c0;
        if (a>b) swap(a,b);
        if (b>c) swap(b,c);
        if (a>b) swap(a,b);
    }
};

bool operator<(const TRIPLE &x, const TRIPLE &y) {
    if (x.a==y.a) {
        if (x.b==y.b) return x.c<y.c;
        else return x.b<y.b;
    } else return x.a<y.a;
}
bool operator==(const TRIPLE &x, const TRIPLE &y) {
    return x.a==y.a && x.b==y.b && x.c==y.c;
}

struct hash_TRIPLE {
    size_t operator()(const TRIPLE &x) const {
        return (x.a<<16) ^ (x.b<<8) ^ (x.c<<0);
    }
};

unordered_map<PAIR, int, hash_PAIR> common2;
unordered_map<TRIPLE, int, hash_TRIPLE> common3;
unordered_map<PAIR, int, hash_PAIR>::iterator common2_it;
unordered_map<TRIPLE, int, hash_TRIPLE>::iterator common3_it;

#define common3_get(x) (((common3_it=common3.find(x))!=common3.end())?(common3_it->second):0)
#define common2_get(x) (((common2_it=common2.find(x))!=common2.end())?(common2_it->second):0)

int **common_x ;
PAIR *edges;
int **inc;
/****** ORCA ******/


long long int Fase::getTypes()
{
  return GTrie::getCanonicalNumber();
}

long long int Fase::getLeafs()
{
  return GTrie::getClassNumber();
}

long long int Fase::getNodes()
{
  return GTrie::getNodeNumber();
}

void Fase::listTree(FILE* f)
{
  return GTrie::listGtrie(f);
}

void Fase::listClasses(FILE* f)
{
  return GTrie::listClasses(f);
}

void Fase::destroy()
{
  int i;
  GTrie::destroy();
  delete[] sub;
  for (i = 0; i < K; i++)
    delete[] extCpy[i];
  delete [] extCpy;
}

void Fase::GraphletsCount(Graph *_G, int _K)
{
  K = _K;
  G = _G;
  sub = new int[K];
  graphSize = G->numNodes();
  cliqueCount = 0;
  memset(count, 0, sizeof count);
  memset(type, 0, sizeof type);
  int i, j, extNum = 0;
  extCpy = new short*[K];

  for (i = 0; i < K; i++)
    extCpy[i] = new short[graphSize];

  /*
  memset(types, 0, sizeof types);
  typeFile = fopen("types.txt", "w");
  */

  buildCommonNodes();

  for (i = 0; i < graphSize; i++)
  {
    sub[0] = i;
    int *nei = G->arrayNeighbours(i);
    int neiNum = G->numNeighbours(i);

    orbits[0]   += neiNum;

    extNum = 0;
    for (j = 0; j < neiNum; j++)
      if (nei[j] > i)
        extCpy[0][extNum++] = nei[j];
    GraphletsExtendSubgraph(extNum, 1, 0);
  }

  calcOrbitFrequency();
  calcGraphletFrequency();
  solveEquations();

  //fclose(typeFile);
}

void Fase::GraphletsExtendSubgraph(int extNum, int subSize, int node)
{
  int i, graph, graphlet;
  int x, a, b, c, xa, xb, xc, ab, ac, bc;

  bool** adjM    = G->adjacencyMatrix();
  int * deg      = G->arrayNumNeighbours();

  if (subSize + 1 == 4)
  {
    for (i = 0; i < extNum; i++)
    {
      int exti = extCpy[subSize - 1][i];

      x  = sub[0];
      a  = sub[1];
      b  = sub[2];
      c  = exti;

      xa = inc[x][a]; xb = inc[x][b]; xc = inc[x][c];
      ab = inc[a][b]; ac = inc[a][c];
      bc = inc[b][c];

      int nm = 0;
      bool *p = adjM[exti];
      for (int j = 0; j < subSize; j++)
        nm += ((int)(*(p + sub[j])) << j);

      int myType = (node << subSize) + nm;
      type[myType]++;

      graph    = mapLabelToGraph[myType];
      graphlet = mapGraphToGraphlet[graph];

      //(node << subSize) + nm == clique[K - 1]
      if (graphlet == 8)
      {
        //Calculo Clique-K
        //if(GTrieNode::adjM[a][neigh] && GTrieNode::adjM[b][neigh] && GTrieNode::adjM[c][neigh]) GTrie::orbit_freq[72]++;
        int *eExcl = G->arrayNeighbours(exti);
        int eExclNum = G->numNeighbours(exti);
        for (int j = 0; j < eExclNum; j++)
        {
          int eEj = eExcl[j];
          
          bool *p = adjM[eEj];
          int fl = 1;
          for (int l = 0; fl && l < subSize; l++)
            fl &= ((sub[l] >= eEj) & (int)*(p + sub[l]));

          orbits[72] += fl * 5;
        }
      }

      /** Graphlet 8 **/
      if (graph == 10) {// nao fazer isto hard coded! just testing
          // o - o
          // | X |
          // o - o
          // All 4 cases: [x,c,b,a], [b,x,c,a], [a,x,c,b], [c,x,b,a]

          orbits[70] += common3_get(TRIPLE(a, b, c)) - 1 +
                        common3_get(TRIPLE(x, b, c)) - 1 +
                        common3_get(TRIPLE(x, a, c)) - 1 +
                        common3_get(TRIPLE(x, a, b)) - 1;

          orbits[71] += ((tri[xc]>2 && tri[xb]>2)?(common3_get(TRIPLE(x,c,b))-1):0) +
                        ((tri[xc]>2 && tri[xa]>2)?(common3_get(TRIPLE(x,a,c))-1):0) +
                        ((tri[xb]>2 && tri[xa]>2)?(common3_get(TRIPLE(x,b,a))-1):0)
                                                           +
                        ((tri[xb]>2 && tri[bc]>2)?(common3_get(TRIPLE(b,x,c))-1):0) +
                        ((tri[xb]>2 && tri[ab]>2)?(common3_get(TRIPLE(x,a,b))-1):0) +
                        ((tri[bc]>2 && tri[ab]>2)?(common3_get(TRIPLE(b,c,a))-1):0)
                                                           +
                        ((tri[xa]>2 && tri[ac]>2)?(common3_get(TRIPLE(a,x,c))-1):0) +
                        ((tri[xa]>2 && tri[ab]>2)?(common3_get(TRIPLE(x,a,b))-1):0) +
                        ((tri[ac]>2 && tri[ab]>2)?(common3_get(TRIPLE(a,c,b))-1):0)
                                                           +
                        ((tri[xc]>2 && tri[bc]>2)?(common3_get(TRIPLE(c,x,b))-1):0) +
                        ((tri[xc]>2 && tri[ac]>2)?(common3_get(TRIPLE(x,a,c))-1):0) +
                        ((tri[bc]>2 && tri[ac]>2)?(common3_get(TRIPLE(c,b,a))-1):0);

          orbits[67] += tri[xc] + tri[xb] + tri[xa] - 6 +
                        tri[xb] + tri[bc] + tri[ab] - 6 +
                        tri[xa] + tri[ac] + tri[ab] - 6 +
                        tri[xc] + tri[bc] + tri[ac] - 6 ;

          orbits[66] += common2_get(PAIR(c,b)) + common2_get(PAIR(c,a)) + common2_get(PAIR(b,a)) - 6 +
                        common2_get(PAIR(x,c)) + common2_get(PAIR(x,a)) + common2_get(PAIR(c,a)) - 6 +
                        common2_get(PAIR(x,c)) + common2_get(PAIR(x,b)) + common2_get(PAIR(c,b)) - 6 +
                        common2_get(PAIR(x,b)) + common2_get(PAIR(x,a)) + common2_get(PAIR(b,a)) - 6;

          orbits[58] += deg[x] - 3 +
                        deg[b] - 3 +
                        deg[a] - 3 +
                        deg[c] - 3;

          orbits[57] += deg[c] + deg[b] + deg[a] - 9 +
                        deg[x] + deg[c] + deg[a] - 9 +
                        deg[x] + deg[c] + deg[b] - 9 +
                        deg[x] + deg[b] + deg[a] - 9;
      }

      /** Graphlet 7 **/
      else if(graph == 3){
          // o - o
          // | \ |
          // o - o

          // Orbit 13
          // 2 Cases: [x,c,b,a], [c,x,b,a]
          orbits[69] += ((tri[xb]>1 && tri[xa]>1)?(common3_get(TRIPLE(x,b,a))-1):0) +
                        ((tri[bc]>1 && tri[ac]>1)?(common3_get(TRIPLE(c,b,a))-1):0);
          orbits[68] += common3_get(TRIPLE(c,b,a)) - 1 +
                        common3_get(TRIPLE(x,b,a)) - 1;
          orbits[64] += common2_get(PAIR(b,a)) - 2 +
                        common2_get(PAIR(b,a)) - 2;
          orbits[61] += tri[xb] + tri[xa] - 2 +
                        tri[bc] + tri[ac] - 2;
          orbits[60] += common2_get(PAIR(c,b)) + common2_get(PAIR(c,a)) - 2 +
                        common2_get(PAIR(x,b)) + common2_get(PAIR(x,a)) - 2;
          orbits[55] += tri[xc] - 2 +
                        tri[xc] - 2;
          orbits[48] += deg[b] + deg[a] - 4 +
                        deg[b] + deg[a] - 4;
          orbits[42] += deg[x] - 3 +
                        deg[c] - 3;
          orbits[41] += deg[c] - 3 +
                        deg[x] - 3;

          // Orbit 12
          // 2 Cases: [b,x,c,a], [a,x,c,b]
          //printf("3: [%d, %d, %d, %d]\n", x, a, b, c);
          orbits[65] += ((tri[xa]>1)?common3_get(TRIPLE(x,c,a)):0) +
                        ((tri[xb]>1)?common3_get(TRIPLE(x,c,b)):0);
          orbits[63] += common_x[b][a] - 2 +
                        common_x[a][b] - 2;
          orbits[59] += tri[xa]-1+common2_get(PAIR(c,a)) - 1 +
                        tri[xb]-1+common2_get(PAIR(c,b)) - 1;
          orbits[54] += common2_get(PAIR(x,c)) - 2 +
                        common2_get(PAIR(x,c)) - 2;
          orbits[47] += deg[b] - 2 +
                        deg[a] - 2;
          orbits[46] += deg[a] - 2 +
                        deg[b] - 2;
          orbits[40] += deg[x] + deg[c] - 6 +
                        deg[x] + deg[c] - 6;
      }
      else if(graph == 9){
          // o - o
          // | x
          // o - o

          // Orbit 13
          // 2 Cases: [x,a,c,b], [a,x,c,b]
          orbits[69] += ((tri[xc]>1 && tri[xb]>1)?(common3_get(TRIPLE(x,c,b))-1):0) +
                        ((tri[ac]>1 && tri[ab]>1)?(common3_get(TRIPLE(a,c,b))-1):0);
          orbits[68] += common3_get(TRIPLE(a,c,b)) - 1 +
                        common3_get(TRIPLE(x,c,b)) - 1;
          orbits[64] += common2_get(PAIR(c,b)) - 2 +
                        common2_get(PAIR(c,b)) - 2;
          orbits[61] += tri[xc] + tri[xb] - 2 +
                        tri[ac] + tri[ab] - 2;
          orbits[60] += common2_get(PAIR(a,c)) + common2_get(PAIR(a,b)) - 2 +
                        common2_get(PAIR(x,c)) + common2_get(PAIR(x,b)) - 2;
          orbits[55] += tri[xa] - 2 +
                        tri[xa] - 2;
          orbits[48] += deg[c] + deg[b] - 4 +
                        deg[c] + deg[b] - 4;
          orbits[42] += deg[x] - 3 +
                        deg[a] - 3;
          orbits[41] += deg[a] - 3 +
                        deg[x] - 3;

          // Orbit 12
          // 2 Cases: [c,x,a,b], [b,x,a,c]
          //printf("9: [%d, %d, %d, %d]\n", x, a, b, c);
          orbits[65] += ((tri[xb]>1)?common3_get(TRIPLE(x,a,b)):0) +
                        ((tri[xc]>1)?common3_get(TRIPLE(x,a,c)):0);
          orbits[63] += common_x[c][b] - 2 +
                        common_x[b][c] - 2;
          orbits[59] += tri[xb]-1+common2_get(PAIR(a,b)) - 1 +
                        tri[xc]-1+common2_get(PAIR(a,c)) - 1;
          orbits[54] += common2_get(PAIR(x,a)) - 2 +
                        common2_get(PAIR(x,a)) - 2;
          orbits[47] += deg[c] - 2 +
                        deg[b] - 2;
          orbits[46] += deg[b] - 2 +
                        deg[c] - 2;
          orbits[40] += deg[x] + deg[a] - 6 +
                        deg[x] + deg[a] - 6;
      }
      else if(graph == 11){
          // o - o
          // | x |
          // o   o

          // Orbit 13
          // 2 Cases: [x,b,c,a], [b,x,c,a]
          orbits[69] += ((tri[xc]>1 && tri[xa]>1)?(common3_get(TRIPLE(x,c,a))-1):0) +
                        ((tri[bc]>1 && tri[ab]>1)?(common3_get(TRIPLE(b,c,a))-1):0);
          orbits[68] += common3_get(TRIPLE(b,c,a)) - 1 +
                        common3_get(TRIPLE(x,c,a)) - 1;
          orbits[64] += common2_get(PAIR(c,a)) - 2 +
                        common2_get(PAIR(c,a)) - 2;
          orbits[61] += tri[xc] + tri[xa] - 2 +
                        tri[bc] + tri[ab] - 2;
          orbits[60] += common2_get(PAIR(b,c)) + common2_get(PAIR(b,a)) - 2 +
                        common2_get(PAIR(x,c)) + common2_get(PAIR(x,a)) - 2;
          orbits[55] += tri[xb] - 2 +
                        tri[xb] - 2;
          orbits[48] += deg[c] + deg[a] - 4 +
                        deg[c] + deg[a] - 4;
          orbits[42] += deg[x] - 3 +
                        deg[b] - 3;
          orbits[41] += deg[b] - 3 +
                        deg[x] - 3;

          // Orbit 12
          // 2 Cases: [c,x,b,a], [a,x,b,c]
          //printf("11: [%d, %d, %d, %d]\n", x, a, b, c);
          orbits[65] += ((tri[xa]>1)?common3_get(TRIPLE(x,b,a)):0) +
                        ((tri[xc]>1)?common3_get(TRIPLE(x,b,c)):0);
          orbits[63] += common_x[c][a] - 2 +
                        common_x[a][c] - 2;
          orbits[59] += tri[xa]-1+common2_get(PAIR(b,a)) - 1 +
                        tri[xc]-1+common2_get(PAIR(b,c)) - 1;
          orbits[54] += common2_get(PAIR(x,b)) - 2 +
                        common2_get(PAIR(x,b)) - 2;
          orbits[47] += deg[c] - 2 +
                        deg[a] - 2;
          orbits[46] += deg[a] - 2 +
                        deg[c] - 2;
          orbits[40] += deg[x] + deg[b] - 6 +
                        deg[x] + deg[b] - 6;
      }
      else if(graph == 17){
          // o   o
          // | x |
          // o - o

          // Orbit 13
          // 2 Cases: [c,a,x,b], [a,c,x,b]
          orbits[69] += ((tri[xc]>1 && tri[bc]>1)?(common3_get(TRIPLE(c,x,b))-1):0) +
                        ((tri[xa]>1 && tri[ab]>1)?(common3_get(TRIPLE(a,x,b))-1):0);
          orbits[68] += common3_get(TRIPLE(a,x,b)) - 1 +
                        common3_get(TRIPLE(c,x,b)) - 1;
          orbits[64] += common2_get(PAIR(x,b)) - 2 +
                        common2_get(PAIR(x,b)) - 2;
          orbits[61] += tri[xc] + tri[bc] - 2 +
                        tri[xa] + tri[ab] - 2;
          orbits[60] += common2_get(PAIR(a,x)) + common2_get(PAIR(a,b)) - 2 +
                        common2_get(PAIR(c,x)) + common2_get(PAIR(c,b)) - 2;
          orbits[55] += tri[ac] - 2 +
                        tri[ac] - 2;
          orbits[48] += deg[x] + deg[b] - 4 +
                        deg[x] + deg[b] - 4;
          orbits[42] += deg[c] - 3 +
                        deg[a] - 3;
          orbits[41] += deg[a] - 3 +
                        deg[c] - 3;

          // Orbit 12
          // 2 Cases: [x,c,a,b], [b,c,a,x]
          orbits[65] += ((tri[bc]>1)?common3_get(TRIPLE(c,a,b)):0) +
                        ((tri[xc]>1)?common3_get(TRIPLE(c,a,x)):0);
          orbits[63] += common_x[x][b] - 2 +
                        common_x[b][x] - 2;
          orbits[59] += tri[bc]-1+common2_get(PAIR(a,b)) - 1 +
                        tri[xc]-1+common2_get(PAIR(a,x)) - 1;
          orbits[54] += common2_get(PAIR(c,a)) - 2 +
                        common2_get(PAIR(c,a)) - 2;
          orbits[47] += deg[x] - 2 +
                        deg[b] - 2;
          orbits[46] += deg[b] - 2 +
                        deg[x] - 2;
          orbits[40] += deg[c] + deg[a] - 6 +
                        deg[c] + deg[a] - 6;

      }

      /** Graphlet 6 **/
      else if(graph == 2){
          // o - o
          // | \
          // o - o

          // Orbit 11
          // 1 Case: [x,c,a,b]
          orbits[44] += tri[xb];
          orbits[33] += deg[x] - 3;
          orbits[30] += deg[b] - 1;
          orbits[26] += deg[c] + deg[a] - 4;

          // Orbit 10
          // 2 Cases: [c,a,x,b], [a,c,x,b]
          orbits[52] += common_x[a][b] - 1 +
                        common_x[c][b] - 1;
          orbits[43] += tri[xb] +
                        tri[xb];
          orbits[32] += deg[x] - 3 +
                        deg[x] - 3;
          orbits[29] += deg[b] - 1 +
                        deg[b] - 1;
          orbits[25] += deg[a] - 2 +
                        deg[c] - 2;

          // Orbit 9
          // 1 Case: [b,x,c,a]
          orbits[56] += (tri[xc]>1 && tri[xa]>1)?common3_get(TRIPLE(x,c,a)):0;
          orbits[45] += common2_get(PAIR(c,a)) - 1;
          orbits[39] += tri[xc] + tri[xa] - 2;
          orbits[31] += deg[x] - 3;
          orbits[28] += deg[b] - 1;
          orbits[24] += deg[c] + deg[a] - 4;

      }
      else if(graph == 4){
          // o - o
          // | \ |
          // o   o

          // Orbit 11
          // 1 Case: [x,c,b,a]
          orbits[44] += tri[xa];
          orbits[33] += deg[x] - 3;
          orbits[30] += deg[a] - 1;
          orbits[26] += deg[c] + deg[b] - 4;

          // Orbit 10
          // 2 Cases: [c,b,x,a], [b,c,x,a]
          orbits[52] += common_x[b][a] - 1 +
                        common_x[c][a] - 1;
          orbits[43] += tri[xa] +
                        tri[xa];
          orbits[32] += deg[x] - 3 +
                        deg[x] - 3;
          orbits[29] += deg[a] - 1 +
                        deg[a] - 1;
          orbits[25] += deg[b] - 2 +
                        deg[c] - 2;

          // Orbit 9
          // 1 Case: [a,x,c,b]
          orbits[56] += (tri[xc]>1 && tri[xb]>1)?common3_get(TRIPLE(x,c,b)):0;
          orbits[45] += common2_get(PAIR(c,b)) - 1;
          orbits[39] += tri[xc] + tri[xb] - 2;
          orbits[31] += deg[x] - 3;
          orbits[28] += deg[a] - 1;
          orbits[24] += deg[c] + deg[b] - 4;
      }
      else if(graph == 8){
          // o - o
          // | x
          // o   o

          // Orbit 11
          // 1 Case: [x,b,a,c]
          orbits[44] += tri[xc];
          orbits[33] += deg[x] - 3;
          orbits[30] += deg[c] - 1;
          orbits[26] += deg[b] + deg[a] - 4;

          // Orbit 10
          // 2 Cases: [b,a,x,c], [a,b,x,c]
          orbits[52] += common_x[a][c] - 1 +
                        common_x[b][c] - 1;
          orbits[43] += tri[xc] +
                        tri[xc];
          orbits[32] += deg[x] - 3 +
                        deg[x] - 3;
          orbits[29] += deg[c] - 1 +
                        deg[c] - 1;
          orbits[25] += deg[a] - 2 +
                        deg[b] - 2;

          // Orbit 9
          // 1 Case: [c,x,b,a]
          orbits[56] += (tri[xb]>1 && tri[xa]>1)?common3_get(TRIPLE(x,b,a)):0;
          orbits[45] += common2_get(PAIR(b,a)) - 1;
          orbits[39] += tri[xb] + tri[xa] - 2;
          orbits[31] += deg[x] - 3;
          orbits[28] += deg[c] - 1;
          orbits[24] += deg[b] + deg[a] - 4;
      }
      else if(graph == 14){
          // o - o
          // | / |
          // o   o

          // Orbit 11
          // 1 Case: [b,x,a,c]
          orbits[44] += tri[bc];
          orbits[33] += deg[b] - 3;
          orbits[30] += deg[c] - 1;
          orbits[26] += deg[x] + deg[a] - 4;

          // Orbit 10
          // 2 Cases: [x,a,b,c], [c,x,a,b]
          orbits[52] += common_x[a][c] - 1 +
                        common_x[x][c] - 1;
          orbits[43] += tri[bc] +
                        tri[bc];
          orbits[32] += deg[b] - 3 +
                        deg[b] - 3;
          orbits[29] += deg[c] - 1 +
                        deg[c] - 1;
          orbits[25] += deg[a] - 2 +
                        deg[x] - 2;

          // Orbit 9
          // 1 Case: [c,b,x,a]
          orbits[56] += (tri[xb]>1 && tri[ab]>1)?common3_get(TRIPLE(b,x,a)):0;
          orbits[45] += common2_get(PAIR(x,a)) - 1;
          orbits[39] += tri[xb] + tri[ab] - 2;
          orbits[31] += deg[b] - 3;
          orbits[28] += deg[c] - 1;
          orbits[24] += deg[x] + deg[a] - 4;
      }
      else if(graph == 16){
          // o   o
          // | x
          // o - o

          // Orbit 11
          // 1 Case: [a,x,c,b]
          orbits[44] += tri[ab];
          orbits[33] += deg[a] - 3;
          orbits[30] += deg[b] - 1;
          orbits[26] += deg[x] + deg[c] - 4;

          // Orbit 10
          // 2 Cases: [x,c,a,b], [c,x,a,b]
          orbits[52] += common_x[x][b] - 1 +
                        common_x[c][b] - 1;
          orbits[43] += tri[ab] +
                        tri[ab];
          orbits[32] += deg[a] - 3 +
                        deg[a] - 3;
          orbits[29] += deg[b] - 1 +
                        deg[b] - 1;
          orbits[25] += deg[c] - 2 +
                        deg[x] - 2;

          // Orbit 9
          // 1 Case: [b,a,x,c]
          orbits[56] += (tri[xa]>1 && tri[ac]>1)?common3_get(TRIPLE(a,x,c)):0;
          orbits[45] += common2_get(PAIR(x,c)) - 1;
          orbits[39] += tri[xa] + tri[ac] - 2;
          orbits[31] += deg[a] - 3;
          orbits[28] += deg[b] - 1;
          orbits[24] += deg[x] + deg[c] - 4;
      }
      else if(graph == 20){
          // o   o
          // | / |
          // o - o

          // Orbit 11
          // 1 Case: [a,c,b,x]
          orbits[44] += tri[xa];
          orbits[33] += deg[a] - 3;
          orbits[30] += deg[x] - 1;
          orbits[26] += deg[c] + deg[b] - 4;

          // Orbit 10
          // 2 Cases: [c,b,a,x], [b,c,a,x]
          orbits[52] += common_x[b][x] - 1 +
                        common_x[c][x] - 1;
          orbits[43] += tri[xa] +
                        tri[xa];
          orbits[32] += deg[a] - 3 +
                        deg[a] - 3;
          orbits[29] += deg[x] - 1 +
                        deg[x] - 1;
          orbits[25] += deg[b] - 2 +
                        deg[c] - 2;

          // Orbit 9
          // 1 Case: [x,a,c,b]
          orbits[56] += (tri[ac]>1 && tri[ab]>1)?common3_get(TRIPLE(a,c,b)):0;
          orbits[45] += common2_get(PAIR(c,b)) - 1;
          orbits[39] += tri[ac] + tri[ab] - 2;
          orbits[31] += deg[a] - 3;
          orbits[28] += deg[x] - 1;
          orbits[24] += deg[c] + deg[b] - 4;
      }

      /** Graphlet 5 **/

      else if(graph == 18){
          // o - o
          // |   |
          // o - o

          // Orbit 8
          // All 4 Cases: [x,c,a,b], [c,x,b,a], [b,c,a,x], [a,x,b,c]
          orbits[62] += ((tri[bc]>0)?common3_get(TRIPLE(c,a,b)):0) +
                        ((tri[xa]>0)?common3_get(TRIPLE(x,b,a)):0) +
                        ((tri[xc]>0)?common3_get(TRIPLE(c,a,x)):0) +
                        ((tri[xc]>0)?common3_get(TRIPLE(x,b,c)):0);
          orbits[53] += tri[xc] + tri[xa] +
                        tri[xc] + tri[bc] +
                        tri[bc] + tri[ab] +
                        tri[xa] + tri[ab];
          orbits[51] += tri[bc] + common2_get(PAIR(a,b)) +
                        tri[xa] + common2_get(PAIR(b,a)) +
                        tri[xc] + common2_get(PAIR(a,x)) +
                        tri[xc] + common2_get(PAIR(b,c));
          orbits[50] += common_x[x][b] - 2 +
                        common_x[c][a] - 2 +
                        common_x[b][x] - 2 +
                        common_x[a][c] - 2;
          orbits[49] += common_x[c][a] - 2 +
                        common_x[x][b] - 2 +
                        common_x[c][a] - 2 +
                        common_x[x][b] - 2;
          orbits[38] += deg[x] - 2 +
                        deg[c] - 2 +
                        deg[b] - 2 +
                        deg[a] - 2;
          orbits[37] += deg[c] + deg[a] - 4 +
                        deg[x] + deg[b] - 4 +
                        deg[c] + deg[a] - 4 +
                        deg[x] + deg[b] - 4;
          orbits[36] += deg[b] - 2 +
                        deg[a] - 2 +
                        deg[x] - 2 +
                        deg[c] - 2;
      }

      /** Graphlet 4 **/

      else if(graph == 1){
          // Orbit 6
          //printf("[%d, %d, %d, %d]\n", x+1, a+1, b+1, c+1);
          orbits[22] += deg[x] - 3 +
                        deg[x] - 3 +
                        deg[x] - 3;
          orbits[20] += deg[c] - 1 +
                        deg[b] - 1 +
                        deg[a] - 1;
          orbits[19] += deg[b] + deg[a] - 2 +
                        deg[c] + deg[a] - 2 +
                        deg[c] + deg[b] - 2;

          // Orbit 7
          //printf("[%d, %d, %d, %d]\n", x+1, a+1, b+1, c+1);
          orbits[23] += deg[x] - 3;
          orbits[21] += deg[c] + deg[b] + deg[a] - 3;

      }
      else if(graph == 19){
          // Orbit 6
          //
          orbits[22] += deg[a] - 3 +
                        deg[a] - 3 +
                        deg[a] - 3;
          orbits[20] += deg[x] - 1 +
                        deg[b] - 1 +
                        deg[c] - 1;
          orbits[19] += deg[b] + deg[c] - 2 +
                        deg[x] + deg[c] - 2 +
                        deg[x] + deg[b] - 2;

          // Orbit 7
          //printf("[%d, %d, %d, %d]\n", x+1, a+1, b+1, c+1);
          orbits[23] += deg[a] - 3;
          orbits[21] += deg[x] + deg[c] + deg[b] - 3;
      }

      /** Graphlet 3 **/

      else if(graph == 7){
          // o - o
          // |   |
          // o   o

          // Orbit 4
          // 2 Cases: [a,x,b,c], [c,b,x,a]
          orbits[35] += common_x[x][c] - 1 +
                        common_x[b][a] - 1;
          orbits[34] += common_x[a][c] +
                        common_x[c][a];
          orbits[27] += tri[bc] +
                        tri[xa];
          orbits[18] += deg[b] - 2 +
                        deg[x] - 2;
          orbits[16] += deg[a] - 1 +
                        deg[c] - 1;
          orbits[15] += deg[c] - 1 +
                        deg[a] - 1;

          //Orbit 5
          // 2 Cases: [x,a,b,c], [b,c,x,a]
          orbits[17] += deg[a] - 1 +
                        deg[c] - 1;
      }
      else if(graph == 15){
          // o   o
          // | x
          // o   o

          // Orbit 4
          // 2 Cases: [c,x,a,b], [b,a,x,c]
          orbits[35] += common_x[x][b] - 1 +
                        common_x[a][c] - 1;
          orbits[34] += common_x[c][b] +
                        common_x[b][c];
          orbits[27] += tri[ab] +
                        tri[xc];
          orbits[18] += deg[a] - 2 +
                        deg[x] - 2;
          orbits[16] += deg[c] - 1 +
                        deg[b] - 1;
          orbits[15] += deg[b] - 1 +
                        deg[c] - 1;

          // Orbit 5
          // 2 Cases: [x,c,a,b], [a,b,x,c]
          orbits[17] += deg[c] - 1 +
                        deg[b] - 1;
      }
      else if(graph == 21){
          // o   o
          // | / |
          // o   o

          // Orbit 4
          // 2 Cases: [x,a,b,c], [c,b,a,x]
          orbits[35] += common_x[a][c] - 1 +
                        common_x[b][x] - 1;
          orbits[34] += common_x[x][c] +
                        common_x[c][x];
          orbits[27] += tri[bc] +
                        tri[xa];
          orbits[18] += deg[b] - 2 +
                        deg[a] - 2;
          orbits[16] += deg[x] - 1 +
                        deg[c] - 1;
          orbits[15] += deg[c] - 1 +
                        deg[x] - 1;

          // Orbit 5
          // 2 Cases: [a,x,b,c], [b,c,a,x]
          orbits[17] += deg[x] - 1 +
                        deg[c] - 1;
      }

      sub[subSize] = exti;


      /*if(types[myType] == 0){
        for(int k = 0; k <= subSize; k++)
            fprintf(typeFile, "%d ", sub[k]+1);
        fprintf(typeFile, "\n");
        fprintf(typeFile, "Type %d: [\n\t  ", myType);
        for (int j = 0; j <= subSize; j++){
          for (int k = 0; k <= subSize; k++)
            fprintf(typeFile, "%d, ", adjM[sub[j]][sub[k]]);
          fprintf(typeFile, "\n\t  ");
        }
        fprintf(typeFile, "]\n");
        types[myType] = 1;
      }*/
    }
    return;
  }

  else if(subSize + 1 == 3){
      int j, o;
      int extCpyNum;
      int exti, *eExcl, eExclNum;

      memcpy(extCpy[subSize], extCpy[subSize - 1], extNum * sizeof(short));

      for (i = extNum - 1; i >= 0; i--)
      {
        extCpyNum = i;
        exti = extCpy[subSize - 1][i];
        eExcl = G->arrayNeighbours(exti);
        eExclNum = G->numNeighbours(exti);

        for (j = 0; j < eExclNum; j++)
        {
          int eEj = eExcl[j];
          if (eEj <= sub[0])
            continue;
          for (o = 0; o < subSize; o++)
            if (eEj == sub[o] || G->isConnected(eEj, sub[o]))
              break;
          if (o == subSize)
            extCpy[subSize][extCpyNum++] = eEj;
        }

        int nm = 0;

        if (subSize >= 2)
        {
          bool *p = adjM[exti];
          for (int j = 0; j < subSize; j++)
            nm += ((int)(*(p + sub[j])) << j);
        }

        int myType = (node << subSize) + nm;
        type[myType]++;

        sub[subSize] = exti;

        GraphletsExtendSubgraph(extCpyNum, subSize + 1, (node << subSize) + nm);
      }
  }

  else { // k=2
      int j, o;
      int extCpyNum;
      int exti, *eExcl, eExclNum;

      memcpy(extCpy[subSize], extCpy[subSize - 1], extNum * sizeof(short));

      for (i = extNum - 1; i >= 0; i--)
      {
        extCpyNum = i;
        exti = extCpy[subSize - 1][i];
        eExcl = G->arrayNeighbours(exti);
        eExclNum = G->numNeighbours(exti);

        for (j = 0; j < eExclNum; j++)
        {
          int eEj = eExcl[j];
          if (eEj <= sub[0])
            continue;
          for (o = 0; o < subSize; o++)
            if (eEj == sub[o] || G->isConnected(eEj, sub[o]))
              break;
          if (o == subSize)
            extCpy[subSize][extCpyNum++] = eEj;
        }

        int nm = 0;

        if (subSize >= 2)
        {
          bool *p = adjM[exti];
          for (int j = 0; j < subSize; j++)
            nm += ((int)(*(p + sub[j])) << j);
        }

        sub[subSize] = exti;

        GraphletsExtendSubgraph(extCpyNum, subSize + 1, (node << subSize) + nm);
      }
  }
}

void Fase::calcOrbitFrequency(){
    int myType, myGraph, freq, k;

    for(myType = 0; myType < 32; myType++){
        myGraph = mapLabelToGraph[myType];
        if(myGraph == -1) continue; //should remove -1s

        freq = type[myType];

        /*printf("type :    %d\n", myType);
        printf("graph:    %d\n", myGraph);
        printf("graphlet: %d\n", mapGraphToGraphlet[myGraph]);*/
        graphlets[mapGraphToGraphlet[myGraph]] += freq;
        for(k = 0; k < K; k++)
          orbits[mapGraphletToOrbits[myGraph][k]] += freq;
    }
}

void Fase::calcGraphletFrequency(){
    graphlets[0]+= orbits[0]/2;
}

void Fase::buildCommonNodes(){
    bool** adjM    = G->adjacencyMatrix();
    int ** fastnei = G->matrixNeighbours();
    int frac_prev, frac;

    printf("computing common nodes\n");
    frac_prev = -1;
    for (int x = 0; x < graphSize; x++) {
        frac = 100LL*x/graphSize;
        if (frac != frac_prev) {
            printf("%d%%\r",frac);
            fflush(stdout);
            frac_prev=frac;
        }
        for (int n1 = 0; n1 < G->numNeighbours(x); n1++) {
            int a = fastnei[x][n1];
            for (int n2 = n1 + 1; n2 < G->numNeighbours(x); n2++) {
                int b   = fastnei[x][n2];
                PAIR ab = PAIR(a,b);
                common2[ab]++;
                for (int n3 = n2 + 1; n3 < G->numNeighbours(x); n3++) {
                    int c  = fastnei[x][n3];
                    int st = adjM[a][b] + adjM[a][c] + adjM[b][c];
                    if (st < 2) continue;
                        TRIPLE abc = TRIPLE(a,b,c);
                        common3[abc]++;
                }
            }
        }
   }

   //this should be changed vvvv
   int m = G->numEdges();
   edges = (PAIR*)malloc(m/2*sizeof(PAIR));

   inc = (int**)malloc(G->numNodes()*sizeof(int*));
   for (int i=0; i < G->numNodes(); i++) inc[i] = (int*)malloc(G->numNodes()*sizeof(int)); //no need, should be g->numNeighbours(x)

   int edge = 0;
   for (int i = 0; i < G->numNodes(); i++) {
       for(int j = 0; j < G->numNeighbours(i); j++){
           int k = fastnei[i][j];
           if(i < k) {
               edges[edge] = PAIR(i, k);
               inc[i][k]=edge; inc[k][i]=edge;
               edge++;}
        }
    }

    m = m/2;
    tri = (int*)calloc(m,sizeof(int));
    for (int i = 0; i < m; i++) {
        int x=edges[i].a, y=edges[i].b;
        for (int xi = 0, yi = 0; xi < G->numNeighbours(x) && yi < G->numNeighbours(y); ) {
            if (fastnei[x][xi] == fastnei[y][yi]) { tri[i]++; xi++; yi++; }
            else if (fastnei[x][xi] < fastnei[y][yi]) { xi++; }
            else { yi++; }
        }
     }

    common_x = (int**)malloc(G->numNodes()*sizeof(int*));
    for (int i=0; i < G->numNodes(); i++) common_x[i] = (int*)malloc(G->numNodes()*sizeof(int));

    for (int x = 0; x < G->numNodes(); x++) {
        frac = 100LL*x/(G->numNodes());
        if (frac!=frac_prev) {
            printf("%d%%\r",frac);
            fflush(stdout);
            frac_prev=frac;
        }

        for (int nx1 = 0; nx1 < G->numNeighbours(x); nx1++) {
            int a = fastnei[x][nx1];
            for (int na = 0; na < G->numNeighbours(a);na++) {
                int b = fastnei[a][na];
                if (b!=x  && !(adjM[x][b])) {
                    common_x[x][b]++;
                }
            }
       }
    }
}

void Fase::solveEquations(){
    orbits[71] = (orbits[71]-12*orbits[72])/2;
    orbits[70] = (orbits[70]-4*orbits[72]);
    orbits[69] = (orbits[69]-2*orbits[71])/4;
    orbits[68] = (orbits[68]-2*orbits[71]);
    orbits[67] = (orbits[67]-12*orbits[72]-4*orbits[71]);
    orbits[66] = (orbits[66]-12*orbits[72]-2*orbits[71]-3*orbits[70]);
    orbits[65] = (orbits[65]-3*orbits[70])/2;
    orbits[64] = (orbits[64]-2*orbits[71]-4*orbits[69]-1*orbits[68]);
    orbits[63] = (orbits[63]-3*orbits[70]-2*orbits[68]);
    orbits[62] = (orbits[62]-1*orbits[68])/2;
    orbits[61] = (orbits[61]-4*orbits[71]-8*orbits[69]-2*orbits[67])/2;
    orbits[60] = (orbits[60]-4*orbits[71]-2*orbits[68]-2*orbits[67]);
    orbits[59] = (orbits[59]-6*orbits[70]-2*orbits[68]-4*orbits[65]);
    orbits[58] = (orbits[58]-4*orbits[72]-2*orbits[71]-1*orbits[67]);
    orbits[57] = (orbits[57]-12*orbits[72]-4*orbits[71]-3*orbits[70]-1*orbits[67]-2*orbits[66]);
    orbits[56] = (orbits[56]-2*orbits[65])/3;
    orbits[55] = (orbits[55]-2*orbits[71]-2*orbits[67])/3;
    orbits[54] = (orbits[54]-3*orbits[70]-1*orbits[66]-2*orbits[65])/2;
    orbits[53] = (orbits[53]-2*orbits[68]-2*orbits[64]-2*orbits[63]);
    orbits[52] = (orbits[52]-2*orbits[66]-2*orbits[64]-1*orbits[59])/2;
    orbits[51] = (orbits[51]-2*orbits[68]-2*orbits[63]-4*orbits[62]);
    orbits[50] = (orbits[50]-1*orbits[68]-2*orbits[63])/3;
    orbits[49] = (orbits[49]-1*orbits[68]-1*orbits[64]-2*orbits[62])/2;
    orbits[48] = (orbits[48]-4*orbits[71]-8*orbits[69]-2*orbits[68]-2*orbits[67]-2*orbits[64]-2*orbits[61]-1*orbits[60]);
    orbits[47] = (orbits[47]-3*orbits[70]-2*orbits[68]-1*orbits[66]-1*orbits[63]-1*orbits[60]);
    orbits[46] = (orbits[46]-3*orbits[70]-2*orbits[68]-2*orbits[65]-1*orbits[63]-1*orbits[59]);
    orbits[45] = (orbits[45]-2*orbits[65]-2*orbits[62]-3*orbits[56]);
    orbits[44] = (orbits[44]-1*orbits[67]-2*orbits[61])/4;
    orbits[43] = (orbits[43]-2*orbits[66]-1*orbits[60]-1*orbits[59])/2;
    orbits[42] = (orbits[42]-2*orbits[71]-4*orbits[69]-2*orbits[67]-2*orbits[61]-3*orbits[55]);
    orbits[41] = (orbits[41]-2*orbits[71]-1*orbits[68]-2*orbits[67]-1*orbits[60]-3*orbits[55]);
    orbits[40] = (orbits[40]-6*orbits[70]-2*orbits[68]-2*orbits[66]-4*orbits[65]-1*orbits[60]-1*orbits[59]-4*orbits[54]);
    orbits[39] = (orbits[39]-4*orbits[65]-1*orbits[59]-6*orbits[56])/2;
    orbits[38] = (orbits[38]-1*orbits[68]-1*orbits[64]-2*orbits[63]-1*orbits[53]-3*orbits[50]);
    orbits[37] = (orbits[37]-2*orbits[68]-2*orbits[64]-2*orbits[63]-4*orbits[62]-1*orbits[53]-1*orbits[51]-4*orbits[49]);
    orbits[36] = (orbits[36]-1*orbits[68]-2*orbits[63]-2*orbits[62]-1*orbits[51]-3*orbits[50]);
    orbits[35] = (orbits[35]-1*orbits[59]-2*orbits[52]-2*orbits[45])/2;
    orbits[34] = (orbits[34]-1*orbits[59]-2*orbits[52]-1*orbits[51])/2;
    orbits[33] = (orbits[33]-1*orbits[67]-2*orbits[61]-3*orbits[58]-4*orbits[44]-2*orbits[42])/2;
    orbits[32] = (orbits[32]-2*orbits[66]-1*orbits[60]-1*orbits[59]-2*orbits[57]-2*orbits[43]-2*orbits[41]-1*orbits[40])/2;
    orbits[31] = (orbits[31]-2*orbits[65]-1*orbits[59]-3*orbits[56]-1*orbits[43]-2*orbits[39]);
    orbits[30] = (orbits[30]-1*orbits[67]-1*orbits[63]-2*orbits[61]-1*orbits[53]-4*orbits[44]);
    orbits[29] = (orbits[29]-2*orbits[66]-2*orbits[64]-1*orbits[60]-1*orbits[59]-1*orbits[53]-2*orbits[52]-2*orbits[43]);
    orbits[28] = (orbits[28]-2*orbits[65]-2*orbits[62]-1*orbits[59]-1*orbits[51]-1*orbits[43]);
    orbits[27] = (orbits[27]-1*orbits[59]-1*orbits[51]-2*orbits[45])/2;
    orbits[26] = (orbits[26]-2*orbits[67]-2*orbits[63]-2*orbits[61]-6*orbits[58]-1*orbits[53]-2*orbits[47]-2*orbits[42]);
    orbits[25] = (orbits[25]-2*orbits[66]-2*orbits[64]-1*orbits[59]-2*orbits[57]-2*orbits[52]-1*orbits[48]-1*orbits[40])/2;
    orbits[24] = (orbits[24]-4*orbits[65]-4*orbits[62]-1*orbits[59]-6*orbits[56]-1*orbits[51]-2*orbits[45]-2*orbits[39]);
    orbits[23] = (orbits[23]-1*orbits[55]-1*orbits[42]-2*orbits[33])/4;
    orbits[22] = (orbits[22]-2*orbits[54]-1*orbits[40]-1*orbits[39]-1*orbits[32]-2*orbits[31])/3;
    orbits[21] = (orbits[21]-3*orbits[55]-3*orbits[50]-2*orbits[42]-2*orbits[38]-2*orbits[33]);
    orbits[20] = (orbits[20]-2*orbits[54]-2*orbits[49]-1*orbits[40]-1*orbits[37]-1*orbits[32]);
    orbits[19] = (orbits[19]-4*orbits[54]-4*orbits[49]-1*orbits[40]-2*orbits[39]-1*orbits[37]-2*orbits[35]-2*orbits[31]);
    orbits[18] = (orbits[18]-1*orbits[59]-1*orbits[51]-2*orbits[46]-2*orbits[45]-2*orbits[36]-2*orbits[27]-1*orbits[24])/2;
    orbits[17] = (orbits[17]-1*orbits[60]-1*orbits[53]-1*orbits[51]-1*orbits[48]-1*orbits[37]-2*orbits[34]-2*orbits[30])/2;
    orbits[16] = (orbits[16]-1*orbits[59]-2*orbits[52]-1*orbits[51]-2*orbits[46]-2*orbits[36]-2*orbits[34]-1*orbits[29]);
    orbits[15] = (orbits[15]-1*orbits[59]-2*orbits[52]-1*orbits[51]-2*orbits[45]-2*orbits[35]-2*orbits[34]-2*orbits[27]);
}
