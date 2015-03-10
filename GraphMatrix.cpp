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
Graphs Implementation with Adj. Matrix and Adj. List

Adapted from gtrieScanner - http://www.dcc.fc.up.pt/gtries/

---------------------------------------------------- */

#include "GraphMatrix.h"
#include "GraphUtils.h"
#include <stdio.h>
#include <algorithm>

GraphMatrix::GraphMatrix() {
  _init();
}

GraphMatrix::~GraphMatrix() {
  _delete();
}

// ------------------------------
// Graph Creation
// ------------------------------

void GraphMatrix::_init() {
  _num_nodes = _num_edges = 0;

  _adjM             = NULL;
  _adjOut           = NULL;
  _adjIn            = NULL;
  _neighbours       = NULL;  
  _in               = NULL;
  _out              = NULL;
  _num_neighbours   = NULL;
  _array_neighbours = NULL;
}

void GraphMatrix::_delete() {
  int i;

  if (_adjM!=NULL) {
    for (i=0; i<_num_nodes; i++)
      if (_adjM[i]!=NULL) delete[] _adjM[i];
    delete[] _adjM;
  }
  if (_adjIn!=NULL) delete[] _adjIn;
  if (_adjOut!=NULL) delete[] _adjOut;
  if (_neighbours!=NULL) delete[] _neighbours;

  if (_in!=NULL) delete[] _in;
  if (_out!=NULL) delete[] _out;
  if (_out!=NULL) delete[] _num_neighbours;

  if (_array_neighbours!=NULL) {
    for (i=0; i<_num_nodes; i++)
      if (_array_neighbours[i]!=NULL) delete[] _array_neighbours[i];
    delete[] _array_neighbours;
  }
}

void GraphMatrix::zero() {
  int i,j;
  _num_edges = 0;

  for (i=0; i<_num_nodes;i++) {
    _in[i] = 0;
    _out[i] = 0;
    _num_neighbours[i] = 0;
    _adjIn[i].clear();
    _adjOut[i].clear();
    _neighbours[i].clear();
    for (j=0; j<_num_nodes;j++)
      _adjM[i][j]=false;
  }
}

void GraphMatrix::createGraph(int n, GraphType t) {
  int i;

  _num_nodes = n;
  _type = t;

  _delete();

  _adjM = new bool*[n];  
  for (i=0; i<n; i++) _adjM[i] = new bool[n];
  _adjIn      = new vector<int>[n];
  _adjOut     = new vector<int>[n];
  _neighbours = new vector<int>[n];

  _in             = new int[n]; 
  _out            = new int[n];
  _num_neighbours = new int[n];

  zero();
}

void GraphMatrix::addEdge(int a, int b) {

  if (_adjM[a][b]) return;

  _adjM[a][b] = true;

  _adjOut[a].push_back(b);
  _out[a]++;

  _adjIn[b].push_back(a);
  _in[b]++;

  _num_edges++;

  if (!_adjM[b][a])
  {
    _neighbours[a].push_back(b);
    _num_neighbours[a]++;
    _neighbours[b].push_back(a);
    _num_neighbours[b]++;

    if (_type == UNDIRECTED)
      addEdge(b, a);
  }
}

void GraphMatrix::rmEdge(int a, int b) {

  if (!_adjM[a][b]) return;

  _adjM[a][b] = false;

  _removeVector(_adjOut[a], b);
  _out[a]--;

  _removeVector(_adjIn[b], a);
  _in[b]--;

  _num_edges--;

  if (!_adjM[b][a]) {
    _removeVector(_neighbours[a],b);
    _num_neighbours[a]--;
    _removeVector(_neighbours[b],a);
    _num_neighbours[b]--;
  }
}

void GraphMatrix::_removeVector(vector<int> &v, int b) {
  int i, s = v.size();
  for (i=0; i<s; i++)
    if (v[i] == b) break;
  if (i<s) v.erase(v.begin()+i);
}

void GraphMatrix::sortNeighbours() {
  int i;
  for (i=0; i<_num_nodes; i++)
    sort(_neighbours[i].begin(), _neighbours[i].begin()+_neighbours[i].size());
}

void GraphMatrix::sortNeighboursArray() {
  int i;
  for (i=0; i<_num_nodes; i++)
    qsort(_array_neighbours[i], _num_neighbours[i], sizeof(int), GraphUtils::int_compare);
}

void GraphMatrix::makeArrayNeighbours() {
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

void GraphMatrix::makeVectorNeighbours() {
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
