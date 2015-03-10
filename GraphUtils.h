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
Graph Utilities

Adapted from gtrieScanner - http://www.dcc.fc.up.pt/gtries/

---------------------------------------------------- */

#ifndef _GRAPHUTILS_
#define _GRAPHUTILS_

#include "Graph.h"

class GraphUtils {
 private:
  static int *_degree;
  static int **_ds;
  static int *_neighbours;
  
 public:

  // Compare two integers
  static int int_compare(const void *a, const void *b);

  // Read file 's', with direction 'dir' to graph 'g'
  static void readFileTxt(Graph *g, const char *s, bool dir, bool weight, int base);

  // Convert adjacency matrix to graph of 'size' nodes
  static void strToGraph(Graph *g, const char *s, int size, bool dir);

  static void orderGraph(Graph *old, Graph *g);
  static int _compare_int(const void *a, const void *b);
  static int _compare_int_descending(const void *a, const void *b);
  static int _compare_degree(const void *a, const void *b);
  
};

#endif
