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
    {
      int currentVertex = vext[depth][--vextSz[depth]];

      if (!sampling || Random::testProb(sampProb[depth]))
      {
        long long int clabel = Label::updateLabel(vsub, currentVertex, depth);
        igtrie.incrementLabel(igtrie.insertLabel(labelNode, clabel, Label::repDigits(depth)), 1);

        motifCount++;
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
}

void Fase::getSubgraphFrequency(pair<long long int, int> element, Isomorphism* iso)
{
  Label::fillNautyMatrix(sadjM, K, element.first);

  nauty_s[0] = '\0';
  iso->canonicalStrNauty(sadjM, nauty_s);
  string str = string(nauty_s);
  canonicalTypes[str] += element.second;
}

void Fase::reduceCanonicalTypes()
{
  if (!canonicalTypes.empty())
    return;

  Isomorphism *iso = new Isomorphism();
  iso->initNauty(K, directed);
  for (auto element : igtrie.enumerate(K))
    getSubgraphFrequency(element, iso);
  iso->finishNauty();
}

int Fase::getTypes()
{
  reduceCanonicalTypes();
  return (int)canonicalTypes.size();
}

vector<pair<int, string> > Fase::subgraphCount()
{
  reduceCanonicalTypes();

  vector<pair<int, string> > subgraphVector;
  for (auto element : canonicalTypes)
    subgraphVector.push_back(make_pair(element.second, element.first));

  sort(subgraphVector.begin(), subgraphVector.end());
  reverse(subgraphVector.begin(), subgraphVector.end());

  return subgraphVector;
}
