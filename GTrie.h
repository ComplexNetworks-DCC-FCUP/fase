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
G-Tries implementation

---------------------------------------------------- */

#ifndef _GTRIE_
#define _GTRIE_

#include "Common.h"

/*! This class creates a G-Trie to store the labelings */
class GTrie
{
 public:
  struct trie;
  struct childTrie;
  struct labelTrie;
  static FILE* f;
  static labelTrie** labelList;
  static labelTrie* labelRoot;
  static long long int* canCount;
  static int numLabel;
  static int maxLabel;
  static char globS[MAXS];
  static void init();
  static void destroy();
  static childTrie* initChild();
  static childTrie* searchChild(childTrie* node, char* s);
  static int searchLabel(labelTrie* node, char* s);
  static char* getLabel(int key);
  static void insert(char* s);
  static void setCanonicalLabel(char *s);
  static void jump();
  static void augment();
  static long long int getCanonicalNumber();
  static long long int getClassNumber();
  static long long int getNodeNumber();
  static void listClasses(FILE* out);
  static void listGtrie(FILE* out);
  static int getCurrentLeaf();
 private:
  static long long int count;
  static long long int nodes;
  static void printClass(labelTrie *node, char* label, int sz);
  static void printGtrie(childTrie *node, char* label, int sz);
};

#endif
