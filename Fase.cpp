#include "Fase.h"

Fase::Fase(Graph* _g, bool _directed)
{
  directed = _directed;
  graph = _g;
  sampling = false;
  
  vext = new int*[MAXMOTIF];
  for (int i = 1; i < MAXMOTIF; i++)
    vext[i] = new int[graph->numNodes()];

  vextSz = new int[MAXMOTIF];
  vsub = new int[MAXMOTIF];
  sampProb = new double[MAXMOTIF];

  iso = new Isomorphism();
  canonicalNumber = 1;
  totalLabel = 0;

  Label::init(_g, _directed);
}

Fase::~Fase()
{
  for (int i = 1; i < MAXMOTIF; i++)
    delete[] vext[i];

  delete[] vext;
  delete[] vextSz;
  delete[] vsub;
  delete[] sampProb;

  iso->finishNauty();
  igtrie.destroy();
}

void Fase::initSampling(int sz, double* _sampProb)
{
  int i;
  for (i = 0; i < sz; i++)
    sampProb[i] = _sampProb[i];

  sampling = true;
}

void Fase::runCensus(int _K)
{
  iso->initNauty(_K, directed);
  K = _K;
  motifCount = 0;
  igtrie.init(K);

  for (int i = 0; i < graph->numNodes(); i++)
    if (!sampling || Random::testProb(sampProb[0]))
    {
      vsub[0] = i;
      int *nei = graph->arrayNeighbours(i);
      int neiNum = graph->numNeighbours(i);

      vextSz[1] = 0;
      for (int j = 0; j < neiNum; j++)
        if (nei[j] > i)
          vext[1][vextSz[1]++] = nei[j];
    
      expandEnumeration(1, 0, 0LL);
    }
}

void Fase::expandEnumeration(int depth, int labelNode, long long int label)
{
  if (depth == K - 1)
  {
    while (vextSz[depth])
      if (!sampling || Random::testProb(sampProb[depth]))
      {
        int currentVertex = vext[depth][--vextSz[depth]];
        long long int clabel = Label::updateLabel(vsub, currentVertex, depth);
        igtrie.incrementLabel(igtrie.insertLabel(labelNode, clabel, Label::repDigits(depth)), 1);

        motifCount++;
      }

    return;
  }

  int i, j;
  long long int clabel = label;
  int clabelNode = labelNode;

  for (i = 0; i < vextSz[depth]; i++)
    vext[depth + 1][i] = vext[depth][i];

  while (vextSz[depth])
    if (!sampling || Random::testProb(sampProb[depth]))
    {
      int currentVertex = vext[depth][--vextSz[depth]];
      vextSz[depth + 1] = vextSz[depth];
      vsub[depth] = currentVertex;

      int *eExcl = graph->arrayNeighbours(currentVertex);
      int eExclNum = graph->numNeighbours(currentVertex);
      
      for (i = 0; i < eExclNum; i++)
      {
        if (eExcl[i] <= vsub[0])
          continue;

        for (j = 0; j < depth; j++)
          if (eExcl[i] == vsub[j] || graph->isConnected(eExcl[i], vsub[j]))
            break;

        if (j == depth)
          vext[depth + 1][vextSz[depth + 1]++] = eExcl[i];
      }

      if (depth >= 1)
      {
        clabel = Label::updateLabel(vsub, currentVertex, depth);
        clabelNode = igtrie.insertLabel(labelNode, clabel, Label::repDigits(depth));
      }

      expandEnumeration(depth + 1, clabelNode, clabel);
    }
}

void Fase::countAddEdge(int a, int b)
{
  runDouble(a, b, -1, 0);
  runDouble(a, b, 1, 1);
}

void Fase::countRemoveEdge(int a, int b)
{
  runDouble(a, b, -1, 1);
  runDouble(a, b, 1, 0);
}

void Fase::runDouble(int a, int b, int increment, int edgeContent)
{
  vsub[0] = a;
  vsub[1] = b;

  vector<int> neiA = *(graph->neighbours(a));
  int neiNumA = graph->numNeighbours(a);

  vector<int> neiB = *(graph->neighbours(b));
  int neiNumB = graph->numNeighbours(b);

  vextSz[2] = 0;
  for (int j = 0; j < neiNumA; j++)
    if (neiA[j] != b)
      vext[2][vextSz[2]++] = neiA[j];

  int prevSize = vextSz[2];
  sort(vext[2], vext[2] + prevSize);
  
  for (int j = 0; j < neiNumB; j++)
    if (neiB[j] != a && !binary_search(vext[2], vext[2] + prevSize, neiB[j]))
      vext[2][vextSz[2]++] = neiB[j];

  long long int label = 0;

  if (directed)
  {
    label |= edgeContent;
    label |= (2 * (graph->hasEdge(b, a)));
  }
  else
    label |= edgeContent;

  int labelNode = igtrie.insertLabel(0, label, Label::repDigits(1));

  totalLabel = label;
  expandDouble(increment, 2, labelNode, label);
}

void Fase::expandDouble(int increment, int depth, int labelNode, long long int label)
{
  if (depth == K - 1)
  {
    while (vextSz[depth])
    {
      int currentVertex = vext[depth][--vextSz[depth]];

      if (!sampling || Random::testProb(sampProb[depth]))
      {
        long long int clabel = Label::updateLabel(vsub, currentVertex, depth);
        int cnode = igtrie.insertLabel(labelNode, clabel, Label::repDigits(depth));
        igtrie.incrementLabel(cnode, increment);

        totalLabel <<= Label::repDigits(depth);
        totalLabel |= clabel;

        if (!igtrie.markLabel(cnode))
          igtrie.markLabel(cnode, getSubgraphFrequency(make_pair(totalLabel, 0), 1));

        if (igtrie.markLabel(cnode) > 0)
        {
          canonicalTypes[igtrie.markLabel(cnode)] += increment;
          motifCount += increment;
        }

        totalLabel >>= Label::repDigits(depth);
      }
    }

    return;
  }

  int i, j;
  long long int clabel = label;
  int clabelNode = labelNode;

  for (i = 0; i < vextSz[depth]; i++)
    vext[depth + 1][i] = vext[depth][i];

  while (vextSz[depth])
  {
    int currentVertex = vext[depth][--vextSz[depth]];

    if (!sampling || Random::testProb(sampProb[depth]))
    {
      vextSz[depth + 1] = vextSz[depth];
      vsub[depth] = currentVertex;

      vector<int> eExcl = *(graph->neighbours(currentVertex));
      int eExclNum = graph->numNeighbours(currentVertex);
      
      for (i = 0; i < eExclNum; i++)
      {
        for (j = 0; j < depth; j++)
          if (eExcl[i] == vsub[j] || graph->isConnected(eExcl[i], vsub[j]))
            break;

        if (j == depth)
          vext[depth + 1][vextSz[depth + 1]++] = eExcl[i];
      }

      clabel = Label::updateLabel(vsub, currentVertex, depth);
      clabelNode = igtrie.insertLabel(labelNode, clabel, Label::repDigits(depth));

      totalLabel <<= Label::repDigits(depth);
      totalLabel |= clabel;

      expandDouble(increment, depth + 1, clabelNode, clabel);

      totalLabel >>= Label::repDigits(depth);
    }
  }
}

int Fase::getSubgraphFrequency(pair<long long int, int> element, int testConnected)
{
  Label::fillNautyMatrix(sadjM, K, element.first);

  if (testConnected)
  {
    int cpy[K][K];
    for (int i = 0; i < K; i++)
      for (int j = 0; j < K; j++)
        cpy[i][j] = sadjM[i * K + j] == '1' || sadjM[j * K + i] == '1';

    for (int k = 0; k < K; k++)
      for (int i = 0; i < K; i++)
        for (int j = 0; j < K; j++)
          cpy[i][j] = cpy[i][j] | (cpy[i][k] & cpy[k][j]);

    for (int i = 0; i < K; i++)
      for (int j = 0; j < K; j++)
        if (!cpy[i][j])
          return -1;
  }

  nauty_s[0] = '\0';
  iso->canonicalStrNauty(sadjM, nauty_s);
  string str = string(nauty_s);

  if (canonicalIndices.count(str) == 0)
    canonicalIndices[str] = canonicalNumber++;

  canonicalTypes[canonicalIndices[str]] += element.second;
  return canonicalIndices[str];
}

void Fase::reduceCanonicalTypes()
{
  if (!canonicalTypes.empty())
    return;

  for (auto element : igtrie.enumerate(K))
    getSubgraphFrequency(element);
}

int Fase::getTypes()
{
  reduceCanonicalTypes();

  int types = (int)canonicalTypes.size();

  for (auto element : canonicalTypes)
    if (element.second == 0)
      types--;

  return types;
}

vector<pair<int, string> > Fase::subgraphCount()
{
  reduceCanonicalTypes();

  vector<pair<int, string> > subgraphVector;
  for (auto element : canonicalIndices)
    if (canonicalTypes[element.second] > 0)
      subgraphVector.push_back(make_pair(canonicalTypes[element.second], element.first));

  sort(subgraphVector.begin(), subgraphVector.end());
  reverse(subgraphVector.begin(), subgraphVector.end());

  return subgraphVector;
}

map<string, int> Fase::subgraphList()
{
  reduceCanonicalTypes();

  map<string, int> subgraphMap;
  for (auto element : canonicalIndices)
    if (canonicalTypes[element.second] > 0)
      subgraphMap[element.first] = canonicalTypes[element.second];

  return subgraphMap;
}
