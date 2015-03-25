#ifndef GRAPHLETS5_H
#define GRAPHLETS5_H

class Graphlets5
{
 public:
  static void init(int ngraphs);
  static void buildCases();

  //static void insertCase(int arr[], int size);
  static void insertCase(int graph, int orbit, int casenum, int map[]);
  static int* getCase(int graph, int orbit, int casenum);
  static int getNCases(int graph, int orbit);
  static void destroy();
 private:
  static int**** graphToOrbitCase; // Graph -> Orbit -> Case -> {x,a,b,c}
  static int** graphToOrbitNCases; // Graph -> Orbit -> Ncases (0,1,2,3,4)
  static int ngraphs;
  static int norbits;
  static int maxcases;
  static int K;
};

#endif // GRAPHLETS5_H
