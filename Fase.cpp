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

/*! Generate the next piece of the LSLabel
    \param w the newly added vertex*/
char* Fase::LSLabel(int w, int subSize)
{
  return LSLabeling::Label(sub, subSize, w, typeLabel, globStr, directed);
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

void Fase::GraphletsCount(Graph *_G, int _K, GTrieGraphlet* ggraphlet)
{
  K = _K;
  G = _G;
  sub = new int[K];
  graphSize = G->numNodes();
  LSLabeling::init(G);
  int i, j, extNum = 0;
  extCpy = new short*[K];

  for (i = 0; i < K; i++)
    extCpy[i] = new short[graphSize];

  for (i = 0; i < graphSize; i++)
  {
    sub[0] = i;
    int *nei = G->arrayNeighbours(i);
    int neiNum = G->numNeighbours(i);
    extNum = 0;
    for (j = 0; j < neiNum; j++)
      if (nei[j] > i)
        extCpy[0][extNum++] = nei[j];
    GraphletsExtendSubgraph(extNum, 1, ggraphlet);
  }
}

void Fase::GraphletsExtendSubgraph(int extNum, int subSize, GTrieGraphlet *ggraphlet)
{
  int i;

  bool** adjM = G->adjacencyMatrix();

  if (subSize == K - 1)
  {
    for (i = 0; i < extNum; i++)
    {
      int exti = extCpy[subSize - 1][i];

      //
      int k = 0;

      //int lab = 0;
      for (int j = 0; j < subSize; j++)
        if(adjM[exti][sub[j]])
          globStr[k++] = j + 1;
          //lab += (j + 1) * pow(10, k++);
      globStr[k] = 0;
      //ggraphlet->insert(globStr, 1);
      //ggraphlet->insert(LSLabel(exti, subSize), 1);

      sub[subSize] = exti;
      MotifCount++;

      //ggraphlet->jump();
    }
    return;
  }

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

    //
    int k = 0;

    for (int j = 0; j < subSize; j++)
      if(adjM[exti][sub[j]])
        globStr[k++] = j + 1;
    globStr[k] = 0;
    //ggraphlet->insert(globStr, 1);
    //ggraphlet->insert(LSLabel(exti, subSize), 1);

    sub[subSize] = exti;

    GraphletsExtendSubgraph(extCpyNum, subSize + 1, ggraphlet);
 
    //ggraphlet->jump();
  }
}
