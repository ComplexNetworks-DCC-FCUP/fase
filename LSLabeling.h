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

#ifndef _LSLABELING_
#define _LSLABELING_

#include "Common.h"
#include "Graph.h"

/*! This class performs the step of generating an LSLabeling */
class LSLabeling
{
 private:
  static Graph *G;

  static char* UPickLabel(int *sub, int subSize, int w, char* label);
  static char* DPickLabel(int *sub, int subSize, int w, char* label);
  static char* UMatrixLabel(int *sub, int subSize, int w, char* label);
  static char* DMatrixLabel(int *sub, int subSize, int w, char* label);

 public:
  static const int TYPE_PICK = 314; // Use the Pick-Order setup
  static const int TYPE_MATRIX = 73; // Use the AdjMatrix-Order setup

  static char* Label(int *sub, int subSize, int w, int type, char* label, bool directed);
  static void init(Graph *_G);
};

#endif
