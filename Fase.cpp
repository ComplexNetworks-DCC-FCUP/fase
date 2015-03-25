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
#include "Graphlets5.h"

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

  solveEquations();
  calcOrbitFrequency();
  //calcGraphletFrequency();

  //fclose(typeFile);
}

void Fase::GraphletsExtendSubgraph(int extNum, int subSize, int node)
{
  int graph, graphlet;
  int x, a, b, c, xa, xb, xc, ab, ac, bc;

  int ncases;
  int* mycase;

  bool** adjM    = G->adjacencyMatrix();
  int * deg      = G->arrayNumNeighbours();

  int node1   = node;
  int extNum1 = extNum;

  int i1, j1, o1, i2, j2, o2, i3;
  int extCpyNum1, extCpyNum2;
  int exti1, *eExcl1, eExclNum1, exti2, *eExcl2, eExclNum2, exti3, *eExcl3, eExclNum3;

  memcpy(extCpy[1], extCpy[0], extNum1 * sizeof(short));

  // K = 2
  for (i1 = extNum1 - 1; i1 >= 0; i1--)
  {
    extCpyNum1 = i1;
    exti1      = extCpy[0][i1];
    eExcl1     = G->arrayNeighbours(exti1);
    eExclNum1  = G->numNeighbours(exti1);

    for (j1 = 0; j1 < eExclNum1; j1++)
    {
      int eEj1 = eExcl1[j1];
      if (eEj1 <= sub[0]) continue;
      for (o1 = 0; o1 < 1; o1++)
        if (eEj1 == sub[o1] || G->isConnected(eEj1, sub[o1])) break;
      if (o1 == 1)
        extCpy[1][extCpyNum1++] = eEj1;
    }
    sub[1] = exti1;

    // K = 3
    int node2   = (node1 << 1);
    int extNum2 = extCpyNum1;

    memcpy(extCpy[2], extCpy[1], extNum2 * sizeof(short));

    for (i2 = extNum2 - 1; i2 >= 0; i2--)
    {
      extCpyNum2 = i2;
      exti2      = extCpy[1][i2];
      eExcl2     = G->arrayNeighbours(exti2);
      eExclNum2  = G->numNeighbours(exti2);

      for (j2 = 0; j2 < eExclNum2; j2++)
      {
        int eEj2 = eExcl2[j2];
        if (eEj2 <= sub[0])
          continue;
        for (o2 = 0; o2 < 2; o2++)
          if (eEj2 == sub[o2] || G->isConnected(eEj2, sub[o2]))
            break;
        if (o2 == 2)
          extCpy[2][extCpyNum2++] = eEj2;
      }

      int nm = 0;

      bool *p = adjM[exti2];
      for (int j2 = 0; j2 < 2; j2++)
        nm += ((int)(*(p + sub[j2])) << j2);

      int myType = (node2 << 2) + nm;
      type[myType]++;

      sub[2] = exti2;

      // K = 4
      int node3   = myType;
      int extNum3 = extCpyNum2;

      memcpy(extCpy[3], extCpy[2], extNum3 * sizeof(short));

      for (i3 = 0; i3 < extNum3; i3++)
      {
        exti3  = extCpy[2][i3];
        sub[3] = exti3;

        int nm = 0;
        bool *p = adjM[exti3];
        for (int j3 = 0; j3 < 3; j3++)
          nm += ((int)(*(p + sub[j3])) << j3);

        int myType = (node3 << 3) + nm;
        type[myType]++;

        //MotifCount++;

        graph    = mapLabelToGraph[myType];
        graphlet = mapGraphToGraphlet[graph];

        /** Graphlet 3 **/
        if(graphlet == 3){
            // Orbit 4
            ncases = 2; //Graphlets5::getNCases(graph, 4);

            for(int i = 0; i < ncases; i++){
              mycase      = Graphlets5::getCase(graph, 4, i);

              x  = sub[mycase[0]]; a  = sub[mycase[1]]; b  = sub[mycase[2]]; c  = sub[mycase[3]];

              bc = inc[b][c];

              orbits[35] += common_x[a][c] - 1;
              orbits[34] += common_x[x][c];
              orbits[27] += tri[bc];
              orbits[18] += deg[b] - 2;
              orbits[15] += deg[c] - 1;
            }
        }

        /** Graphlet 4 **/
        else if(graphlet == 4){
            // Orbit 6

            // Orbit 7
            mycase      = Graphlets5::getCase(graph, 7, 0);

            x  = sub[mycase[0]];

            orbits[23] += deg[x] - 3;
        }

        /** Graphlet 5 **/
        else if(graphlet == 5){
            // Orbit 8
            ncases = 4; //Graphlets5::getNCases(graph, 8);

            for(int i = 0; i < ncases; i++){
              mycase      = Graphlets5::getCase(graph, 8, i);

              x  = sub[mycase[0]]; a  = sub[mycase[1]]; b  = sub[mycase[2]]; c  = sub[mycase[3]];

              xa = inc[x][a]; xb = inc[x][b];

              orbits[53] += tri[xa] + tri[xb];
              orbits[50] += common_x[x][c] - 2;
            }
        }

        /** Graphlet 6 **/
        else if(graphlet == 6){
            // Orbit 11
            mycase      = Graphlets5::getCase(graph, 11, 0);

            x  = sub[mycase[0]]; c  = sub[mycase[3]];
            xc = inc[x][c];

            orbits[44] += tri[xc];

            // Orbit 10

            // Orbit 9
            mycase      = Graphlets5::getCase(graph, 9, 0);

            a  = sub[mycase[1]]; b  = sub[mycase[2]]; c  = sub[mycase[3]];
            ab = inc[a][b]; ac = inc[a][c];

            orbits[45] += common2_get(PAIR(b,c)) - 1;
            orbits[39] += tri[ab] + tri[ac] - 2;
            orbits[31] += deg[a] - 3;
            orbits[24] += deg[b] + deg[c] - 4;
        }

        /** Graphlet 7 **/
        else if(graphlet == 7){
            // Orbit 13
            ncases = Graphlets5::getNCases(graph, 13);

            for(int i = 0; i < ncases; i++){
              mycase      = Graphlets5::getCase(graph, 13, i);

              x  = sub[mycase[0]]; a  = sub[mycase[1]]; b  = sub[mycase[2]]; c  = sub[mycase[3]];
              xa = inc[x][a]; xb = inc[x][b]; xc = inc[x][c];

              orbits[68] += common3_get(TRIPLE(a,b,c)) - 1;
              orbits[64] += common2_get(PAIR(b,c)) - 2;
              orbits[61] += tri[xb] + tri[xc] - 2;
              orbits[55] += tri[xa] - 2;
            }

            // Orbit 12
        }

        /** Graphlet 8 **/
        else if (graphlet == 8)
        {
          //Calculo Clique-K
          //if(GTrieNode::adjM[a][neigh] && GTrieNode::adjM[b][neigh] && GTrieNode::adjM[c][neigh]) GTrie::orbit_freq[72]++;
          eExcl3    = G->arrayNeighbours(exti3);
          eExclNum3 = G->numNeighbours(exti3);
          for (int j3 = 0; j3 < eExclNum3; j3++)
          {
            int eEj3 = eExcl3[j3];

            bool *p = adjM[eEj3];
            int fl = 1;
            for (int l = 0; fl && l < 3; l++)
              fl &= ((sub[l] >= eEj3) & (int)*(p + sub[l]));

            orbits[72] += fl * 5;
           }
           // Orbit 14
           ncases = 4; //Graphlets5::getNCases(graph, 14);

           for(int i = 0; i < ncases; i++){
             mycase      = Graphlets5::getCase(graph, 14, i);

             x  = sub[mycase[0]]; a  = sub[mycase[1]]; b  = sub[mycase[2]]; c  = sub[mycase[3]];

             xa = inc[x][a]; xb = inc[x][b]; xc = inc[x][c];

             orbits[70] += common3_get(TRIPLE(a, b, c)) - 1;
             orbits[67] += tri[xa] + tri[xb] + tri[xc] - 6;
             orbits[58] += deg[x] - 3;
           }
        }
       }
     }
   }
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
    orbits[70] = (orbits[70]-4*orbits[72]);
    orbits[68] = (orbits[68]-3*orbits[70]);
    orbits[67] = (orbits[67]-12*orbits[72]-6*orbits[70]);
    orbits[64] = (orbits[64]-3*orbits[70]-1*orbits[68]-1*orbits[68]);
    orbits[61] = (orbits[61]-6*orbits[70]-2*orbits[68]-2*orbits[67])/2;
    orbits[58] = (orbits[58]-4*orbits[72]-3*orbits[70]-1*orbits[67]);
    orbits[55] = (orbits[55]-3*orbits[70]-2*orbits[67])/3;
    orbits[53] = (orbits[53]-2*orbits[68]-2*orbits[64]-2*orbits[64]);
    orbits[50] = (orbits[50]-1*orbits[68]-2*orbits[64])/3;
    orbits[45] = (orbits[45]-1*orbits[67]-1*orbits[64]-3*orbits[58]);
    orbits[44] = (orbits[44]-1*orbits[67]-2*orbits[61])/4;
    orbits[39] = (orbits[39]-2*orbits[67]-2*orbits[61]-6*orbits[58])/2;
    orbits[35] = (orbits[35]-2*orbits[61]-1*orbits[53]-2*orbits[45])/2;
    orbits[34] = (orbits[34]-2*orbits[61]-2*orbits[53])/2;
    orbits[31] = (orbits[31]-1*orbits[67]-2*orbits[61]-3*orbits[58]-4*orbits[44]-2*orbits[39]);
    orbits[27] = (orbits[27]-2*orbits[61]-1*orbits[53]-2*orbits[45])/2;
    orbits[24] = (orbits[24]-2*orbits[67]-2*orbits[64]-2*orbits[61]-6*orbits[58]-1*orbits[53]-2*orbits[45]-2*orbits[39]);
    orbits[23] = (orbits[23]-1*orbits[55]-1*orbits[39]-1*orbits[31])/4;
    orbits[18] = (orbits[18]-2*orbits[61]-1*orbits[53]-4*orbits[45]-2*orbits[35]-2*orbits[27]-1*orbits[24])/2;
    orbits[15] = (orbits[15]-2*orbits[61]-2*orbits[53]-2*orbits[45]-2*orbits[35]-2*orbits[34]-2*orbits[27]);
}



void Fase::calcOrbitFrequency(){
    int myType, myGraph, freq, k;

    for(myType = 0; myType < 32; myType++){
        myGraph = mapLabelToGraph[myType];
        if(myGraph == -1) continue; //should remove -1s

        freq = type[myType];

        graphlets[mapGraphToGraphlet[myGraph]] += freq;
        for(k = 0; k < K; k++)
          orbits[mapGraphletToOrbits[myGraph][k]] += freq;
    }

    // G9
    orbits[16] = orbits[15];
    orbits[17] = orbits[15]/2;

    // G10
    orbits[19] = orbits[18] * 2;
    orbits[20] = orbits[18];
    orbits[21] = orbits[18];

    // G11
    orbits[22] = orbits[23] * 4;

    // G12
    orbits[25] = orbits[24]/2;
    orbits[26] = orbits[24];

    // G13
    orbits[28] = orbits[27];
    orbits[29] = orbits[27] * 2;
    orbits[30] = orbits[27];

    // G14
    orbits[32] = orbits[31];
    orbits[33] = orbits[31]/2;

    // G15

    // G16
    orbits[36] = orbits[35];
    orbits[37] = orbits[35] * 2;
    orbits[38] = orbits[35];

    // G17
    orbits[40] = orbits[39] * 2;
    orbits[41] = orbits[39];
    orbits[42] = orbits[39];

    // G18
    orbits[43] = orbits[44] * 4;

    // G19
    orbits[46] = orbits[45];
    orbits[47] = orbits[45];
    orbits[48] = orbits[45] * 2;

    // G20
    orbits[49] = 3*orbits[50]/2;

    // G21
    orbits[51] = orbits[53];
    orbits[52] = orbits[53]/2;

    // G22
    orbits[54] = 3*orbits[55]/2;

    // G23
    orbits[56] = orbits[58];
    orbits[57] = orbits[58] * 3;

    // G24
    orbits[59] = orbits[61] * 2;
    orbits[60] = orbits[61] * 2;

    // G25
    orbits[62] = orbits[64]/2;
    orbits[63] = orbits[64];

    // G26
    orbits[65] = orbits[67]/2;
    orbits[66] = orbits[67];

    // G27
    orbits[69] = orbits[68]/4;

    // G28
    orbits[71] = 3*orbits[70]/2;
}

void Fase::calcGraphletFrequency(){
    graphlets[0]+= orbits[0]/2;
}
