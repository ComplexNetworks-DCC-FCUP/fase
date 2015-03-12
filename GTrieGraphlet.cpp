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
G-Trie Graphlet implementation

---------------------------------------------------- */

#include "GTrieGraphlet.h"

struct GTrieGraphlet::labelTrie
{
  labelTrie* childs[2];
  labelTrie* parent;
  int num;
  char let;
};

struct GTrieGraphlet::childTrie
{
  //  childTrie* list[1 << (8 * sizeof(char))];
  childTrie* list[MAXMOTIF + 1];
  trie* ref;
};

struct GTrieGraphlet::trie
{
  trie* parent;
  childTrie* childs;
  int leaf;
  long long int count;
};

GTrieGraphlet::childTrie* GTrieGraphlet::initChild()
{
  GTrieGraphlet::childTrie* nn = new GTrieGraphlet::childTrie();
  return nn;
}

void GTrieGraphlet::init()
{
  nodes = 0;
  count = 0;
  numLabel = 1;
  maxLabel = 1000;
  labelList = (GTrieGraphlet::labelTrie **)malloc(maxLabel * sizeof(GTrieGraphlet::labelTrie *));
  canCount = (long long int *)malloc(maxLabel * sizeof(long long int));
  labelRoot = new GTrieGraphlet::labelTrie();
  labelRoot->parent = NULL;
  current = new GTrieGraphlet::trie();
  current->parent = NULL;
  current->childs = initChild();
  current->count = 0;
}

void GTrieGraphlet::destroy()
{
  free(labelList);
  free(canCount);
}

void GTrieGraphlet::augment()
{
  maxLabel *= 2;
  labelList = (labelTrie **)realloc(labelList, maxLabel * sizeof(labelTrie *));
  canCount = (long long int *)realloc(canCount, maxLabel * sizeof(long long int));
}

long long int GTrieGraphlet::getCanonicalNumber()
{
  return numLabel - 1;
}

long long int GTrieGraphlet::getNodeNumber()
{
  return nodes;
}

long long int GTrieGraphlet::getClassNumber()
{
  return count;
}

GTrieGraphlet::childTrie* GTrieGraphlet::searchChild(childTrie* node, char* s)
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

void GTrieGraphlet::insert(char* s, int ini)
{
  childTrie* temp = searchChild(current->childs, s);
  if (temp->ref != NULL)
  {
    temp->ref->count += ini;
    canCount[temp->ref->leaf] += ini;
    current = temp->ref;
  }
  else
  {
    nodes++;
    temp->ref = new GTrieGraphlet::trie();
    temp->ref->parent = current;
    temp->ref->childs = initChild();
    temp->ref->count = ini;
    temp->ref->leaf = 0;
    current = temp->ref;
  }
}

void GTrieGraphlet::insertCensus(char* s, int ini)
{
    childTrie* temp = searchChild(current->childs, s);
    if (temp->ref != NULL)
    {
      temp->ref->count += ini;
      canCount[temp->ref->leaf] += ini;
      current = temp->ref;
    }
    else
    {
      nodes++; printf("!%d\n", ini);
      temp->ref = new GTrieGraphlet::trie();
      temp->ref->parent = current;
      temp->ref->childs = initChild();
      temp->ref->count = ini;
      temp->ref->leaf = 0;
      current = temp->ref;
    }
}

int GTrieGraphlet::searchLabel(labelTrie* node, char* s)
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
        node->childs[*s - '0'] = new GTrieGraphlet::labelTrie();
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

void GTrieGraphlet::setCanonicalLabel(char* s)
{
  count++;
  int vl = searchLabel(labelRoot, s);
  current->leaf = vl;
}

int GTrieGraphlet::getCurrentLeaf()
{
  return current->leaf;
}

void GTrieGraphlet::jump()
{
  current = current->parent;
}

char* GTrieGraphlet::getLabel(int key)
{
  int size = 0;
  GTrieGraphlet::labelTrie* dp = labelList[key];
  while (dp->parent != NULL)
  {
    globS[size++] = dp->let;
    dp = dp->parent;
  }
  globS[size] = 0;
  return globS;
}

void GTrieGraphlet::printGtrie(childTrie *node, char* label, int sz, int k, int prev)
{
  if (node->ref != NULL)
  {
    label[sz] = '\0';
    fprintf(f, "+%s", label + prev);

    if (k - 1)
      printGtrie(node->ref->childs, label, sz, k - 1, sz);

    fprintf(f, "-");
  }
  
  int i;
  for (i = 1; i < MAXMOTIF + 1; i++)
  {
    label[sz] = i + '0' - 1;
    if (node->list[i] != NULL)
      printGtrie(node->list[i], label, sz + 1, k, prev);
  }
}

void GTrieGraphlet::listGtrie(FILE* out, int k)
{
  f = out;
  fprintf(f, "=\n");
  while (current->parent != NULL)
    jump();
  char tmpStr[MAXS];
  tmpStr[0] = '\0';
  printGtrie(current->childs, tmpStr, 0, k - 1, 0);
  fprintf(f, "\ndone\n");
}


void GTrieGraphlet::printClass(labelTrie *node, char* label, int sz)
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
 
void GTrieGraphlet::listClasses(FILE* out)
{
  f = out;
  char tmpStr[MAXS];
  tmpStr[0] = '\0';
  printClass(labelRoot, tmpStr, 0);
}

void GTrieGraphlet::readTree(char* rep)
{
  destroy();
  init();

  char buf[200];
  int i;
  for (i = 0; rep[i]; i++)
  {
    if (rep[i] == '+')
    {
      int sz = 0;
      while (rep[++i] >= '0')
        buf[sz++] = rep[i] - '0' + 1;
      buf[sz] = '\0';
      i--;
      
      insert(buf, 0);
    }
    else if (rep[i] == '-')
      jump();
  }
}
