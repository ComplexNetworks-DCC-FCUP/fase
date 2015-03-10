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
G-Trie implementation

---------------------------------------------------- */

#include "GTrie.h"

FILE* GTrie::f;
long long int GTrie::count;
long long int GTrie::nodes;
GTrie::labelTrie** GTrie::labelList;
long long int* GTrie::canCount;
int GTrie::numLabel;
int GTrie::maxLabel;
char GTrie::globS[MAXS];

struct GTrie::labelTrie
{
  labelTrie* childs[2];
  labelTrie* parent;
  int num;
  char let;
};

struct GTrie::childTrie
{
  //  childTrie* list[1 << (8 * sizeof(char))];
  childTrie* list[MAXMOTIF + 1];
  trie* ref;
};

struct GTrie::trie
{
  trie* parent;
  childTrie* childs;
  int leaf;
  long long int count;
};

GTrie::trie* current;
GTrie::labelTrie* GTrie::labelRoot;

GTrie::childTrie* GTrie::initChild()
{
  childTrie* nn = new childTrie();
  return nn;
}

void GTrie::init()
{
  nodes = 0;
  count = 0;
  numLabel = 1;
  maxLabel = 1000;
  labelList = (labelTrie **)malloc(maxLabel * sizeof(labelTrie *));
  canCount = (long long int *)malloc(maxLabel * sizeof(long long int));
  labelRoot = new labelTrie();
  labelRoot->parent = NULL;
  current = new GTrie::trie();
  current->parent = NULL;
  current->childs = initChild();
  current->count = 0;
}

void GTrie::destroy()
{
  free(labelList);
  free(canCount);
}

void GTrie::augment()
{
  maxLabel *= 2;
  labelList = (labelTrie **)realloc(labelList, maxLabel * sizeof(labelTrie *));
  canCount = (long long int *)realloc(canCount, maxLabel * sizeof(long long int));
}

long long int GTrie::getCanonicalNumber()
{
  return numLabel - 1;
}

long long int GTrie::getNodeNumber()
{
  return nodes;
}

long long int GTrie::getClassNumber()
{
  return count;
}

GTrie::childTrie* GTrie::searchChild(childTrie* node, char* s)
{
  while (1)
  {
    if (*s == '\0')
      return node;
    if (node->list[*s] == NULL)
      node->list[*s] = initChild();
    node = node->list[*s];
    s++;
  }

  return NULL;
}

void GTrie::insert(char* s)
{
  childTrie* temp = searchChild(current->childs, s);
  if (temp->ref != NULL)
  {
    temp->ref->count++;
    canCount[temp->ref->leaf]++;
    current = temp->ref;
  }
  else
  {
    nodes++;
    temp->ref = new GTrie::trie();
    temp->ref->parent = current;
    temp->ref->childs = initChild();
    temp->ref->count = 1;
    temp->ref->leaf = 0;
    current = temp->ref;
  }
}

int GTrie::searchLabel(labelTrie* node, char* s)
{
  while (1)
  {
    if (*s == 0)
    {
      if (node->num == 0)
      {
        node->num = numLabel;
        canCount[numLabel] = 1;
        labelList[numLabel++] = node;
        if (numLabel == maxLabel - 2)
          augment();
      }
      else
        canCount[node->num]++;

      return node->num;
    }
    else
    {
      if (node->childs[*s - '0'] == NULL)
      {
        node->childs[*s - '0'] = new labelTrie();
        node->childs[*s - '0']->num = 0;
        node->childs[*s - '0']->let = *s;
        node->childs[*s - '0']->parent = node;
      }
      node = node->childs[*s - '0'];
      s++;
    }
  }

  return NULL;
}

void GTrie::setCanonicalLabel(char* s)
{
  GTrie::count++;
  int vl = searchLabel(labelRoot, s);
  current->leaf = vl;
}

int GTrie::getCurrentLeaf()
{
  return current->leaf;
}

void GTrie::jump()
{
  current = current->parent;
}

char* GTrie::getLabel(int key)
{
  int size = 0;
  labelTrie* dp = labelList[key];
  while (dp->parent != NULL)
  {
    globS[size++] = dp->let;
    dp = dp->parent;
  }
  globS[size] = 0;
  return globS;
}

void GTrie::printGtrie(GTrie::childTrie *node, char* label, int sz)
{
  if (node->ref != NULL)
  {
    if (sz != 0)
      label[sz++] = '+';
    label[sz] = '\0';
//      printf("Class: %s, Found: %lld, Sample Model: %s\n", label, node->ref->count, getLabel(node->ref->leaf));
    if (!node->ref->leaf)
      printGtrie(node->ref->childs, label, sz);
    else
    {
      label[sz - 1] = '\0';
      fprintf(f, "%s\n", label);
    }
    if (sz != 0)
      label[--sz] = '\0';
  }
  int i;
  for (i = 1; i < MAXMOTIF + 1; i++)
  {
    label[sz] = i + '0' - 1;
    label[sz + 1] = '\0';
    if (node->list[i] != NULL)
      printGtrie(node->list[i], label, sz + 1);
  }
}

void GTrie::listGtrie(FILE* out)
{
  f = out;
  fprintf(f, "=\n");
  while (current->parent != NULL)
    GTrie::jump();
  char tmpStr[MAXS];
  tmpStr[0] = '\0';
  GTrie::printGtrie(current->childs, tmpStr, 0);
  fprintf(f, "done\n");
}


void GTrie::printClass(GTrie::labelTrie *node, char* label, int sz)
{
  if (node->num != 0)
  {
    label[sz] = '\0';
    fprintf(f, "Canonical Class: %s - Occurrences: %lld\n", label, canCount[node->num]);
    return;
  }
  if(node->childs[0] != NULL)
  {
    label[sz] = '0';
    printClass(node->childs[0], label, sz + 1);
  }
  if(node->childs[1] != NULL)
  {
    label[sz] = '1';
    printClass(node->childs[1], label, sz + 1);
  }
}
 
void GTrie::listClasses(FILE* out)
{
  f = out;
  char tmpStr[MAXS];
  tmpStr[0] = '\0';
  GTrie::printClass(labelRoot, tmpStr, 0);
}
