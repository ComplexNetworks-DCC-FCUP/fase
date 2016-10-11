#ifndef _IGTRIE_
#define _IGTRIE_

#include "Common.h"

#define LB_WORD_LEN 4
#define LB_WORD_SIZE 16 // 1 << LB_WORD_LEN

class IGtrie
{
 private:
  int maxLabels;
  int numLabels;
  int** labelPaths;
  int* labelLeaf;
  int* labelCount;
  int* labelType;
  vector<pair<long long int, int> > enumeration;

  void expand();
  void enumerateFrom(int currentNode, long long int label, long long int parLabel, int parSize, int remaining);

 public:
  IGtrie();
  ~IGtrie();

  void init(int K);
  void destroy();

  void incrementLabel(int labelNode, int value);
  int insertLabel(int labelNode, long long int label, int digits);
  vector<pair<long long int, int> > enumerate(int K);
  int markLabel(int node, int type = 0);
};

#endif
