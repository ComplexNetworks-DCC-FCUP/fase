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

David Aparicio, Pedro {Paredes, Ribeiro} - DCC/FCUP

----------------------------------------------------
G-Tries Graphlet implementation

---------------------------------------------------- */

#ifndef _GTRIEGRAPHLET_
#define _GTRIEGRAPHLET_

#include "Common.h"

class GTrieGraphlet
{
 public:
  struct trie;
  struct childTrie;
  struct labelTrie;

  void init();
  void destroy();
  void insert(char* s, int ini);
  void insertCensus(char* s, int ini);
  void setCanonicalLabel(char *s);
  void jump();
  long long int getCanonicalNumber();
  long long int getClassNumber();
  long long int getNodeNumber();
  void listClasses(FILE* out);
  void listGtrie(FILE* out, int k);
  int getCurrentLeaf();

  void readTree(char* rep);

 private:
  
  trie* current;
  long long int count;
  long long int nodes;    
  FILE* f;
  labelTrie** labelList;
  labelTrie* labelRoot;
  long long int* canCount;
  int numLabel;
  int maxLabel;
  char globS[MAXS];

  void printClass(labelTrie *node, char* label, int sz);
  void printGtrie(childTrie *node, char* label, int sz, int k, int prev);
  void augment();
  childTrie* initChild();
  childTrie* searchChild(childTrie* node, char* s);
  int searchLabel(labelTrie* node, char* s);
  char* getLabel(int key);
};

#endif
