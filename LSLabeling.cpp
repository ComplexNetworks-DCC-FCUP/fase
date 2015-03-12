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
LS-Labeling implementation

---------------------------------------------------- */

#include "LSLabeling.h"

Graph* LSLabeling::G;

void LSLabeling::init(Graph *_G)
{
  G = _G;
}

/* Old versions of Matrix Labeling (used one byte per edge) */
char* LSLabeling::DMatrixLabel(int *sub, int subSize, int w, char* label)
{
  int j, s = 0, tmp;
  for (j = 0; j < subSize; j++)
    if (G->hasEdge(w, sub[j]))
      label[s++] = 2;
    else
      label[s++] = 1;
  for (j = 0; j < subSize; j++)
    if (G->hasEdge(sub[j], w))
      label[s++] = 2;
    else
      label[s++] = 2;
  label[s++] = 0;
  return label;
}

char* LSLabeling::UMatrixLabel(int *sub, int subSize, int w, char* label)
{
  int j, s = 0, tmp;
  for (j = 0; j < subSize; j++)
    if (G->isConnected(w, sub[j]))
      label[s++] = 2;
    else
      label[s++] = 1;
  label[s++] = 0;
  return label;
}


/*char* LSLabeling::DMatrixLabel(int *sub, int subSize, int w, char* label)
{
  int j, s = 0, bitCount = 7;
  label[s] = 1;
  for (j = 0; j < subSize; j++)
  {
    if (G->hasEdge(w, sub[j]))
      label[s] |= (1 << bitCount);
    bitCount--;
    if (bitCount == 0)
    {
      bitCount = 7;
      s++;
      label[s] = 1;
    }
  }
  for (j = 0; j < subSize; j++)
  {
    if (G->hasEdge(sub[j], w))
      label[s] |= (1 << bitCount);
    bitCount--;
    if (bitCount == 0)
    {
      bitCount = 7;
      s++;
      label[s] = 1;
    }
  }
  if (bitCount < 7)
    s++;
  label[s] = 0;
  return label;
}

char* LSLabeling::UMatrixLabel(int *sub, int subSize, int w, char* label)
{ 
  int j, s = 0, bitCount = 1;
  label[s] = 1;
  for (j = 0; j < subSize; j++)
  {
    if (G->isConnected(w, sub[j]))
      label[s] |= (1 << bitCount);
    bitCount++;
    if (bitCount == 8)
    {
      bitCount = 1;
      s++;
      label[s] = 1;
    }
  }
  if (bitCount != 8)
    s++;
  label[s] = 0;
  return label;
}
*/

char* LSLabeling::DPickLabel(int *sub, int subSize, int w, char* label)
{
  int j, s = 0, tmp;
  for (j = 0; j < subSize; j++)
    if (G->hasEdge(w, sub[j]))
      label[s++] = j + 1;
  label[s++] = subSize + 1;
  for (j = 0; j < subSize; j++)
    if (G->hasEdge(sub[j], w))
      label[s++] = j + 1;
  label[s++] = 0;
  return label;
}

char* LSLabeling::UPickLabel(int *sub, int subSize, int w, char* label)
{
  int j, s = 0;

  for (j = 0; j < subSize; j++)
    //if (G->isConnected(w, sub[j]))
    if(G->adjacencyMatrix()[w][sub[j]])
      label[s++] = j + 1;
  label[s] = 0;


  /*
  char* a = &label[0];
  for (j = 0; j < subSize; j++)
    //if (G->isConnected(w, sub[j]))
    if(G->adjacencyMatrix()[w][sub[j]])
      *(a + s++) = j + 1;
  *(a + s) = 0;
  return label;
  */


  /*bool *b = &(G->adjacencyMatrix()[w][0]);
  for (j = 0; j < subSize; j++)
    if ((*G->outEdges(w))[j] == *(b + sub[j]))
      label[s++] = j + 1;
  label[s] = 0;
  return label;*/


  /*bool *b = &adjM[i][0];
  for (j=0; j<glk; j++)
    if (out[j] != *(b+mymap[j]))*/
}

char* LSLabeling::Label(int *sub, int subSize, int w, int type, char* label, bool directed)
{
  return LSLabeling::UPickLabel(sub, subSize, w, label);
  /*if (directed)
  {
    if (type == TYPE_PICK)
      return LSLabeling::DPickLabel(sub, subSize, w, label);
    else if (type == TYPE_MATRIX)
      return LSLabeling::DMatrixLabel(sub, subSize, w, label);
    else
      return "";
  }
  else
  {
    if (type == TYPE_PICK)
      return LSLabeling::UPickLabel(sub, subSize, w, label);
    else if (type == TYPE_MATRIX)
      return LSLabeling::UMatrixLabel(sub, subSize, w, label);
    else
      return "";
  }*/
}
