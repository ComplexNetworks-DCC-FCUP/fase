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
Graphs Implementation with Adj. List

Adapted from gtrieScanner - http://www.dcc.fc.up.pt/gtries/

---------------------------------------------------- */

#include "GraphList.h"
#include "GraphUtils.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>

struct GraphList::l_list
{
  int val;
  l_list *next;
};

GraphList::GraphList() {
  _init();
}

GraphList::~GraphList() {
  _delete();
}

// ------------------------------
// Graph Creation
// ------------------------------

void GraphList::_init() {
  _num_nodes = _num_edges = 0;

  _adjOut           = NULL;
  _adjIn            = NULL;
  _neighbours       = NULL;  
  _in               = NULL;
  _out              = NULL;
  _sq_out           = NULL;
  _num_neighbours   = NULL;
  _array_neighbours = NULL;
}

void GraphList::_delete() {
  int i;

  /* HASH MAP STATISTICS OUTPUT
  if (_out != NULL)
  {
    FILE *fout = fopen("hash_map.txt", "w");
    for (i = 0; i < _num_nodes; i++)
    {
      fprintf(fout, "%d:", i + 1);
      int j;
      for (j = 0; j < _sq_nodes * _out[i]; j++)
      {
        l_list *curr = hash_map[i][j];
        int ct = 0;
        while (curr != NULL)
        {
          ct++;
          curr = curr->next;
        }
        fprintf(fout, " %d", ct);
      }
      fprintf(fout, "\n");
    }
    fclose(fout);
  }
  //*/

  if (_adjIn!=NULL) delete[] _adjIn;
  if (_adjOut!=NULL) delete[] _adjOut;
  if (_neighbours!=NULL) delete[] _neighbours;

  if (_in!=NULL) delete[] _in;
  if (_out!=NULL) delete[] _num_neighbours;
  if (_out!=NULL) delete[] _out;
  if (_sq_out!=NULL) delete[] _sq_out;

  if (_array_neighbours!=NULL) {
    for (i=0; i<_num_nodes; i++)
      if (_array_neighbours[i]!=NULL) delete[] _array_neighbours[i];
    delete[] _array_neighbours;
  }
}

void GraphList::zero() {
  int i,j;
  _num_edges = 0;

  for (i=0; i<_num_nodes;i++) {
    _in[i] = 0;
    _out[i] = 0;
    _sq_out[i] = 0;
    _num_neighbours[i] = 0;
    _adjIn[i].clear();
    _adjOut[i].clear();
    _neighbours[i].clear();
  }
}

void GraphList::createGraph(int n, GraphType t) {
  int i;

  _num_nodes = n;
  _sq_nodes = sqrt(n);
  _type = t;

  _delete();

  _adjIn      = new vector<int>[n];
  _adjOut     = new vector<int>[n];
  _neighbours = new vector<int>[n];

  _in             = new int[n]; 
  _out            = new int[n];
  _sq_out         = new int[n];
  _num_neighbours = new int[n];

  hash_map = new l_list**[n];
  cache_map = new int[n];

  zero();
}

void GraphList::addEdge(int a, int b)
{
  int i;
  for (i = 0; i < _out[a]; i++)
    if (_adjOut[a][i] == b)
      return;

  _adjOut[a].push_back(b);
  _out[a]++;

  _adjIn[b].push_back(a);
  _in[b]++;

  for (i = 0; i < _out[b]; i++)
    if (_adjOut[b][i] == a)
      return;

  _neighbours[a].push_back(b);
  _num_neighbours[a]++;

  _neighbours[b].push_back(a);
  _num_neighbours[b]++;

  if (_type == UNDIRECTED)
  {
    _adjOut[b].push_back(a);
    _out[b]++;

    _adjIn[a].push_back(b);
    _in[a]++;
  }
}

void GraphList::preProcess()
{
  _num_edges = 0;
  int i, j;
  for (i = 0; i < _num_nodes; i++)
  {
    sort(_adjOut[i].begin(), _adjOut[i].begin() + _out[i]);
    _out[i] = unique(_adjOut[i].begin(), _adjOut[i].begin() + _out[i]) - _adjOut[i].begin();
    sort(_adjIn[i].begin(), _adjIn[i].begin() + _in[i]);
    _in[i] = unique(_adjIn[i].begin(), _adjIn[i].begin() + _in[i]) - _adjIn[i].begin();
    sort(_neighbours[i].begin(), _neighbours[i].begin() + _num_neighbours[i]);
    _num_neighbours[i] = unique(_neighbours[i].begin(), _neighbours[i].begin() + _num_neighbours[i]) - _neighbours[i].begin();

    int sz = _out[i];
    int sq_nodes = sqrt(sz) + 1;
    int hs_sz = sq_nodes * sz;
    _sq_out[i] = hs_sz;
    _num_edges += sz;

    hash_map[i] = new l_list*[hs_sz];
    for (j = 0; j < hs_sz; j++)
      hash_map[i][j] = NULL;

    for (j = 0; j < sz; j++)
    {
      cache_map[i] = _adjOut[i][j];
      int pos = _adjOut[i][j] % hs_sz;
      if (hash_map[i][pos] == NULL)
      {
        hash_map[i][pos] = new l_list();
        hash_map[i][pos]->val = _adjOut[i][j];
      }
      else
      {
        l_list *curr = hash_map[i][pos];
        while (curr->next != NULL)
          curr = curr->next;
        curr->next = new l_list();
        curr->next->val = _adjOut[i][j];
      }
    }
  }
}

void GraphList::rmEdge(int a, int b)
{
  fprintf(stderr, "Currently removing edges is deprecated...\n");
}

bool GraphList::hasEdge(int a, int b)
{
  if (cache_map[a] == b)
    return true;

  if (_out[a] == 0)
    return false;

  int pos = b % (_sq_out[a]);
  
  if (hash_map[a][pos] == NULL)
    return false;

  l_list *curr = hash_map[a][pos];
  while (curr != NULL)
  {
    if (curr->val == b)
    {
      cache_map[a] = b;
      return true;
    }
    else if (curr->val > b)
      return false;

    curr = curr->next;
  }

  return false;

/* Old BSearch Code, kept for reference

  int lo = 0, hi = _out[a] - 1, med, vl, vlo, vhi;
  if (hi < 0)
    return false;
  
  while (lo <= hi)
  {
    vlo = _adjOut[a][lo], vhi = _adjOut[a][hi];
    med = (lo + hi) / 2;
    vl = _adjOut[a][med];
    if (vl < b)
      lo = med + 1;
    else if (vl > b)
      hi = med - 1;
    else if (vl == b)
      return true;
  }

  return false;*/
}

bool GraphList::isConnected(int a, int b)
{
  return hasEdge(a, b) || hasEdge(b, a);
}

void GraphList::_removeVector(vector<int> &v, int b) {
  int i, s = v.size();
  for (i=0; i<s; i++)
    if (v[i] == b) break;
  if (i<s) v.erase(v.begin()+i);
}

void GraphList::sortNeighbours() {
  int i;
  for (i=0; i<_num_nodes; i++)
    sort(_neighbours[i].begin(), _neighbours[i].begin()+_neighbours[i].size());
}

void GraphList::sortNeighboursArray() {
  int i;
  for (i=0; i<_num_nodes; i++)
    qsort(_array_neighbours[i], _num_neighbours[i], sizeof(int), GraphUtils::int_compare);
}

void GraphList::makeArrayNeighbours() {
  int i,j;
  vector<int>:: iterator ii;
  _array_neighbours = new int*[_num_nodes];  
  for (i=0; i<_num_nodes; i++) {
    _array_neighbours[i] = new int[_neighbours[i].size()];
    for (ii=_neighbours[i].begin(), j=0; ii!=_neighbours[i].end(); ++ii, j++)
      _array_neighbours[i][j] = *ii;
    _neighbours[i].clear();
  }
}

void GraphList::makeVectorNeighbours() {
  int i,j;
  vector<int>:: iterator ii;

  for (i=0; i<_num_nodes; i++)
    for (j=0; j<_num_neighbours[i]; j++)
      _neighbours[i].push_back(_array_neighbours[i][j]);

  if (_array_neighbours!=NULL) {
    for (i=0; i<_num_nodes; i++)
      if (_array_neighbours[i]!=NULL) delete[] _array_neighbours[i];
    delete[] _array_neighbours;
  }
}
