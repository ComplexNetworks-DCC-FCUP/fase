#include "Graphlets5.h"
#include "stdio.h"

int**** Graphlets5::graphToOrbitCase;
int** Graphlets5::graphToOrbitNCases;
int Graphlets5::ngraphs;
int Graphlets5::norbits;
int Graphlets5::maxcases;
int Graphlets5::K;

void Graphlets5::init(int ngraphs1)
{
    ngraphs  = ngraphs1;
    norbits  = 14;
    maxcases = 4;
    K        = 4;

    graphToOrbitCase   = new int***[ngraphs+1];
    graphToOrbitNCases = new int*[ngraphs+1];

    for(int i = 0 ; i <= ngraphs ; i++ )
    {
        graphToOrbitCase[i]   = new int**[norbits+1] ;
        graphToOrbitNCases[i] = new int[norbits+1] ;
        for(int j = 0 ; j <= norbits ; j++ )
        {
            graphToOrbitNCases[i][j] = 0;
            graphToOrbitCase[i][j]   = new int*[maxcases] ;
            for(int k = 0 ; k < maxcases ; k++ )
                graphToOrbitCase[i][j][k] = new int[ K ] ;
        }
    }
}


void Graphlets5::buildCases(){
    /** Graphlet 8 **/
    /// Graph 10
    // o - o
    // | X |
    // o - o

    // Orbit 14
    // All 4 cases: [x,a,b,c], [b,x,a,c], [c,x,a,b], [a,x,b,c]
    insertCase(10, 14, 0, new int[4]{0, 1, 2, 3}) ;
    insertCase(10, 14, 1, new int[4]{2, 0, 1, 3}) ;
    insertCase(10, 14, 2, new int[4]{3, 0, 1, 2}) ;
    insertCase(10, 14, 3, new int[4]{1, 0, 2, 3}) ;
    /**  ********  **/


    /** Graphlet 7 **/
    /// Graph 3
    // o - o
    // | \ |
    // o - o

    // Orbit 13
    // 2 Cases: [x,c,b,a], [c,x,b,a]
    insertCase(3, 13, 0, new int[4]{0, 3, 2, 1}) ;
    insertCase(3, 13, 1, new int[4]{3, 0, 2, 1}) ;

    // Orbit 12
    // 2 Cases: [b,x,c,a], [a,x,c,b]
    insertCase(3, 12, 0, new int[4]{2, 0, 3, 1}) ;
    insertCase(3, 12, 1, new int[4]{1, 0, 3, 2}) ;

    /// Graph 9
    // o - o
    // | x
    // o - o

    // Orbit 13
    // 2 Cases: [x,a,c,b], [a,x,c,b]
    insertCase(9, 13, 0, new int[4]{0, 1, 3, 2}) ;
    insertCase(9, 13, 1, new int[4]{1, 0, 3, 2}) ;

    // Orbit 12
    // 2 Cases: [c,x,a,b], [b,x,a,c]
    insertCase(9, 12, 0, new int[4]{3, 0, 1, 2}) ;
    insertCase(9, 12, 1, new int[4]{2, 0, 1, 3}) ;

    /// Graph 11
    // o - o
    // | x |
    // o   o

    // Orbit 13
    // 2 Cases: [x,b,c,a], [b,x,c,a]
    insertCase(11, 13, 0, new int[4]{0, 2, 3, 1}) ;
    insertCase(11, 13, 1, new int[4]{2, 0, 3, 1}) ;

    // Orbit 12
    // 2 Cases: [c,x,b,a], [a,x,b,c]
    insertCase(11, 12, 0, new int[4]{3, 0, 2, 1}) ;
    insertCase(11, 12, 1, new int[4]{1, 0, 2, 3}) ;

    ///Graph 17
    // o   o
    // | x |
    // o - o

    // Orbit 13
    // 2 Cases: [c,a,x,b], [a,c,x,b]
    insertCase(17, 13, 0, new int[4]{3, 1, 0, 2}) ;
    insertCase(17, 13, 1, new int[4]{1, 3, 0, 2}) ;

    // Orbit 12
    // 2 Cases: [x,c,a,b], [b,c,a,x]
    insertCase(17, 12, 0, new int[4]{0, 3, 1, 2}) ;
    insertCase(17, 12, 1, new int[4]{2, 3, 1, 0}) ;
    /**  ********  **/


    /** Graphlet 6 **/
    /**  ********  **/
    ///Graph 2
    // o - o
    // | \
    // o - o

    // Orbit 11
    // 1 Case: [x,c,a,b]
    insertCase(2, 11, 0, new int[4]{0, 3, 1, 2}) ;

    // Orbit 10
    // 2 Cases: [c,a,x,b], [a,c,x,b]
    insertCase(2, 10, 0, new int[4]{3, 1, 0, 2}) ;
    insertCase(2, 10, 1, new int[4]{1, 3, 0, 2}) ;

    // Orbit 9
    // 1 Case: [b,x,c,a]
    insertCase(2, 9, 0, new int[4]{2, 0, 3, 1}) ;

    ///Graph 4
    // o - o
    // | \ |
    // o   o

    // Orbit 11
    // 1 Case: [x,c,b,a]
    insertCase(4, 11, 0, new int[4]{0, 3, 2, 1}) ;

    // Orbit 10
    // 2 Cases: [c,b,x,a], [b,c,x,a]
    insertCase(4, 10, 0, new int[4]{3, 2, 0, 1}) ;
    insertCase(4, 10, 1, new int[4]{2, 3, 0, 1}) ;

    // Orbit 9
    // 1 Case: [a,x,c,b]
    insertCase(4, 9, 0, new int[4]{1, 0, 3, 2}) ;

    ///Graph 8
    // o - o
    // | x
    // o   o

    // Orbit 11
    // 1 Case: [x,b,a,c]
    insertCase(8, 11, 0, new int[4]{0, 2, 1, 3}) ;

    // Orbit 10
    // 2 Cases: [b,a,x,c], [a,b,x,c]
    insertCase(8, 10, 0, new int[4]{2, 1, 0, 3}) ;
    insertCase(8, 10, 1, new int[4]{1, 2, 0, 3}) ;

    // Orbit 9
    // 1 Case: [c,x,b,a]
    insertCase(8, 9, 0, new int[4]{3, 0, 2, 1}) ;

    ///Graph 14
    // o - o
    // | / |
    // o   o

    // Orbit 11
    // 1 Case: [b,x,a,c]
    insertCase(14, 11, 0, new int[4]{2, 0, 1, 3}) ;

    // Orbit 10
    // 2 Cases: [x,a,b,c], [a,x,b,c]
    insertCase(14, 10, 0, new int[4]{0, 1, 2, 3}) ;
    insertCase(14, 10, 1, new int[4]{1, 0, 2, 3}) ;

    // Orbit 9
    // 1 Case: [c,b,x,a]
    insertCase(14, 9, 0, new int[4]{3, 2, 0, 1}) ;

    ///Graph 16
    // o   o
    // | x
    // o - o

    // Orbit 11
    // 1 Case: [a,x,c,b]
    insertCase(16, 11, 0, new int[4]{1, 0, 3, 2}) ;

    // Orbit 10
    // 2 Cases: [x,c,a,b], [c,x,a,b]
    insertCase(16, 10, 0, new int[4]{0, 3, 1, 2}) ;
    insertCase(16, 10, 1, new int[4]{3, 0, 1, 2}) ;

    // Orbit 9
    // 1 Case: [b,a,x,c]
    insertCase(16, 9, 0, new int[4]{2, 1, 0, 3}) ;

    ///Graph 20
    // o   o
    // | / |
    // o - o

    // Orbit 11
    // 1 Case: [a,c,b,x]
    insertCase(20, 11, 0, new int[4]{1, 3, 2, 0}) ;

    // Orbit 10
    // 2 Cases: [c,b,a,x], [b,c,a,x]
    insertCase(20, 10, 0, new int[4]{3, 2, 1, 0}) ;
    insertCase(20, 10, 1, new int[4]{2, 3, 1, 0}) ;

    // Orbit 9
    // 1 Case: [x,a,c,b]
    insertCase(20, 9, 0, new int[4]{0, 1, 3, 2}) ;

    /** Graphlet 5 **/
    /// Graph 18
    // o - o
    // |   |
    // o - o

    // Orbit 8
    // 4 cases:  [x,c,a,b], [c,x,b,a], [b,c,a,x], [a,x,b,c]
    insertCase(18, 8, 0, new int[4]{0, 3, 1, 2}) ;
    insertCase(18, 8, 1, new int[4]{3, 0, 2, 1}) ;
    insertCase(18, 8, 2, new int[4]{2, 3, 1, 0}) ;
    insertCase(18, 8, 3, new int[4]{1, 0, 2, 3}) ;
    /**  ********  **/


    /** Graphlet 4 **/
    /// Graph 1
    // o - o
    // | \
    // o   o

    // Orbit 6
    // 3 Cases: [c,x,b,a], [b,x,c,a], [a,x,c,b]
    insertCase(1, 6, 0, new int[4]{3, 0, 2, 1}) ;
    insertCase(1, 6, 1, new int[4]{2, 0, 3, 1}) ;
    insertCase(1, 6, 2, new int[4]{1, 0, 3, 2}) ;

    // Orbit 7
    // 1 Case: [x,c,b,a]
    insertCase(1, 7, 0, new int[4]{0, 3, 2, 1}) ;

    /// Graph 19
    // o   o
    // | /
    // o - o

    // Orbit 6
    // 3 Cases: [x,a,b,c], [b,a,x,c], [c,a,x,b]
    insertCase(19, 6, 0, new int[4]{0, 1, 2, 3}) ;
    insertCase(19, 6, 1, new int[4]{2, 1, 0, 3}) ;
    insertCase(19, 6, 2, new int[4]{3, 1, 0, 2}) ;

    // Orbit 7
    // 1 Case: [a,x,c,b]
    insertCase(19, 7, 0, new int[4]{1, 0, 3, 2}) ;
    /**  ********  **/


    /** Graphlet 3 **/
    /// Graph 7
    // o - o
    // |   |
    // o   o

    // Orbit 4
    // 2 Cases: [a,x,b,c], [c,b,x,a]
    insertCase(7, 4, 0, new int[4]{1, 0, 2, 3}) ;
    insertCase(7, 4, 1, new int[4]{3, 2, 0, 1}) ;

    //Orbit 5
    // 2 Cases: [x,a,b,c], [b,c,x,a]
    insertCase(7, 5, 0, new int[4]{0, 1, 2, 3}) ;
    insertCase(7, 5, 1, new int[4]{2, 3, 0, 1}) ;

    /// Graph 15
    // o   o
    // | x
    // o   o

    // Orbit 4
    // 2 Cases: [c,x,a,b], [b,a,x,c]
    insertCase(15, 4, 0, new int[4]{3, 0, 1, 2}) ;
    insertCase(15, 4, 1, new int[4]{2, 1, 0, 3}) ;

    // Orbit 5
    // 2 Cases: [x,c,a,b], [a,b,x,c]
    insertCase(15, 5, 0, new int[4]{1, 3, 1, 2}) ;
    insertCase(15, 5, 1, new int[4]{1, 2, 0, 3}) ;

    /// Graph 21
    // o   o
    // | / |
    // o   o

    // Orbit 4
    // 2 Cases: [x,a,b,c], [c,b,a,x]
    insertCase(21, 4, 0, new int[4]{0, 1, 2, 3}) ;
    insertCase(21, 4, 1, new int[4]{3, 2, 1, 0}) ;

    // Orbit 5
    // 2 Cases: [a,x,b,c], [b,c,a,x]
    insertCase(21, 5, 0, new int[4]{1, 0, 2, 3}) ;
    insertCase(21, 5, 1, new int[4]{2, 3, 1, 0}) ;
    /**  ********  **/

}

void Graphlets5::insertCase(int graph, int orbit, int casenum, int map[])
{
  for (int i = 0; i < 4; i++) {
      //printf("%d, ", map[i]);
      graphToOrbitCase[graph][orbit][casenum][i] = map[i];
  }

  graphToOrbitNCases[graph][orbit]++;

  delete [] map;
}

int* Graphlets5::getCase(int graph, int orbit, int casenum)
{
  return graphToOrbitCase[graph][orbit][casenum];
}

int Graphlets5::getNCases(int graph, int orbit)
{
  return graphToOrbitNCases[graph][orbit];
}

void Graphlets5::destroy()
{
    for (int i = 0; i <= ngraphs; ++i) {
        for (int j = 0; j <= norbits; ++j){
          for (int k = 0; k < maxcases; ++k){
            delete [] graphToOrbitCase[i][j][k];
          }
          delete [] graphToOrbitCase[i][j];
        }
        delete [] graphToOrbitCase[i];
        delete [] graphToOrbitNCases[i];
    }
    delete [] graphToOrbitCase;
    delete [] graphToOrbitNCases;
}
