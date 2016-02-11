#include "Label.h"

Graph* Label::graph;
bool Label::directed;

void Label::init(Graph* _g, bool _directed)
{
  graph = _g;
  directed = _directed;
}

long long int Label::updateLabel(int* vsub, int currentVertex, int depth)
{
  long long int label = 0;

  if (directed)
  {
    for (int i = 0; i < depth; i++)
    {
      label |= ((1LL << (2 * i)) * graph->hasEdge(vsub[i], currentVertex));
      label |= ((1LL << (2 * i + 1)) * graph->hasEdge(currentVertex, vsub[i]));
    }
  }
  else
  {
    for (int i = 0; i < depth; i++)
      label |= ((1LL << i) * graph->isConnected(vsub[i], currentVertex));
  }

  return label;
}

int Label::repDigits(int depth)
{
  if (directed)
    return 2 * depth;
  else
    return depth;
}

void Label::fillNautyMatrix(char* sadjM, int K, long long int mask)
{
  int i, j;
  for (i = 0; i < K * K; i++)
    sadjM[i] = '0';
  sadjM[K * K] = '\0';

  int acc = 0;
  for (i = K - 1; i >= 1; i--)
  {
    int gone = 0;
    for (j = 0; j < i; j++)
    {
      if (directed)
      {
        sadjM[j * K + i] =
          ((mask & (1LL << (2 * j + acc))) > 0) ? '1' : '0';
        sadjM[i * K + j] =
          ((mask & (1LL << ((2 * j + 1) + acc))) > 0) ? '1' : '0';
        gone += 2;
      }
      else
      {
        sadjM[i * K + j] = sadjM[j * K + i] =
          ((mask & (1LL << (j + acc))) > 0) ? '1' : '0';
        gone++;
      }
    }

    acc += gone;
  }
}
