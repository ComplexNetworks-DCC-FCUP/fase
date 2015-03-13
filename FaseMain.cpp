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
Main File For Testing

---------------------------------------------------- */

#include "Common.h"
#include "GraphMatrix.h"
#include "GraphList.h"
#include "GraphUtils.h"
#include "Fase.h"
#include "Timer.h"
#include "Graph.h"
#include "Isomorphism.h"
#include "Random.h"

using namespace std;

Graph *G;
int K = 0;
bool dir = false, detailed = false, draw = false;
char ifilename [100];
char ofilename [100];
char buf[10000];
FILE *outFile;
time_t t_start, t_end;

void initNauty()
{
  Isomorphism::initNauty(K, dir);
}

void init()
{
  printf("  88888888888           ad88888ba   88888888888  \n"
         "  88                   d8\"     \"8b  88           \n"
         "  88                   Y8,          88           \n"
         "  88aaaaa  ,adPPYYba,  `Y8aaaaa,    88aaaaa      \n"
         "  88\"\"\"\"\"  \"\"     `Y8    `\"\"\"\"\"8b,  88\"\"\"\"\"      \n"
         "  88       ,adPPPPP88          `8b  88           \n"
         "  88       88,    ,88  Y8a     a8P  88           \n"
         "  88       `\"8bbdP\"Y8   \"Y88888P\"   88888888888  \n\n"
         "FaSE - Fast Subgraph Enumeration\n"
         "\n\n\tPedro {Paredes, Ribeiro} - DCC/FCUP\n\n\n\n\n");
  t_start = time(0);
}

void displayHelp()
{
  printf("------------ FaSE Usage --------------\nMain Settings: ./FASE -s <Subgraph Size> -i <input file> [arguments...]\n\n\tAll commands:\n-h : Displays this help information\n-s <Integer> : Subgraph Size\n-i <Filename> : Name of input file (Format in Readme)\n-d : Directed Subgraph (Default undirected)\n-o : Name of output file (Default is stdout)\n-dt : Detailed Result (Displays all subgraph types and occurrences)\n-z : Use 0-based input (Suitable for input files starting at node 0)\n-tm : Use Adjacency Matrix LS-Labeling (Default is Adjacency List Labeling)\n-l : Use Adjacency List Only (Suitable for Large Scale or large networks [>10^5 nodes])\n-q : Ignore arguments and prompt input\n--------------------------------------\n\n---- More Info about running FaSE in the Readme file\n");
}

void read(int argc, char **argv)
{
  int E, V, i, check = 0, itera = 0;
  int zeroBased = 1;
  ofilename[0] = '0';
  ofilename[1] = '\0';
  G = new GraphMatrix();
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
      continue;
    if (argv[i][1] == 'h')
    {
      displayHelp();
      K = 0;
      return;
    }
    if (argv[i][1] == 'd' && argv[i][2] == 't')
      detailed = true;
    else if (argv[i][1] == 'd' && argv[i][2] == 'r')
      draw = true;
    else if (argv[i][1] == 'd')
      dir = true;
    if (argv[i][1] == 'l')
      G = new GraphList();
    if (argv[i][1] == 'z')
      zeroBased = 0;

    if (argv[i][1] == 'i' && argv[i][2] == '\0')
    {
      i++;
      strcpy(ifilename, argv[i]);
      check |= (1 << 0);
      continue;
    }

    if (argv[i][1] == 's')
    {
      i++;
      K = argv[i][0] - '0';
      int j = 1;
      while (argv[i][j] != '\0')
      {
    K *= 10;
    K += argv[i][j] - '0';
    j++;
      }
      check |= (1 << 1);
      continue;
    }

    if (argv[i][1] == 'o')
    {
      i++;
      strcpy(ofilename, argv[i]);
      continue;
    }

    if (argv[i][1] == 'q')
    {
      itera = 1;
      break;
    }
  }

  if (!itera)
  {
    if (check != (1 << 2) - 1)
    {
      K = 0;
      if (check != 0)
    printf("Warning: Incorrect number of necessary arguments provided\n");
      displayHelp();
      return;
    }
    GraphUtils::readFileTxt(G, ifilename, dir, false, zeroBased);
    G->sortNeighbours();
    G->makeArrayNeighbours();
    if (ofilename[0] == '0' && ofilename[1] == '\0')
      outFile = stdout;
    else
      outFile = fopen(ofilename, "w");

    return;
  }

  // Direction
  printf("Directed? (Y/n) ");
  char chdir;
  scanf(" %c", &chdir);
  if (chdir == 'n' || chdir == 'N')
    dir = false;

  // Initial
  printf("Input 0 or 1 based: ");
  scanf("%d", &zeroBased);

  // Input filename
  printf("Insert input file name: ");
  scanf(" %s", ifilename);
  GraphUtils::readFileTxt(G, ifilename, dir, false, zeroBased);
  G->sortNeighbours();
  G->makeArrayNeighbours();

  // Subgraph Size
  printf("Input the value K of the subgraph search: ");
  scanf("%d", &K);

  // Input filename
  printf("Insert output file name or 0 to stdout: ");
  scanf(" %s", ofilename);
  if (ofilename[0] == '0' && ofilename[1] == '\0')
    outFile = stdout;
  else
    outFile = fopen(ofilename, "w");
}

void finishNauty()
{
  Isomorphism::finishNauty();
}

void outputGraphlets()
{
  printf("Finished Calculating\n");
  FILE *f = outFile;
  fprintf(f, "\tOutput:\n");
  fprintf(f, "Network: %s\n", ifilename);
  fprintf(f, "Directed: %s\n", dir ? "Yes" : "No");
  fprintf(f, "Nodes: %d\n", G->numNodes());
  fprintf(f, "Edges: %d\n", G->numEdges() / (dir ? 1 : 2));
  fprintf(f, "Subgraph Size: %d\n", K);

  t_end = time(0);
  struct tm *tm_start = localtime(&t_start);
  fprintf(f, "Start of Computation: %02dh%02dm%02ds %02d/%02d/%02d\n\
", tm_start->tm_hour, tm_start->tm_min, tm_start->tm_sec, tm_start->tm_mday, tm_start->tm_mon+1, 1900+tm_start->tm_year);
  struct tm *tm_end   = localtime(&t_end);
  fprintf(f, "End of Computation: %02dh%02dm%02ds %02d/%02d/%02d\n", tm_end->tm_hour, tm_end->tm_min, tm_end->tm_sec, tm_end->tm_mday, tm_end->tm_mon+1, 1900+tm_end->tm_year);
  
  fprintf(f, "\n\n\tResults:\n");
  fprintf(f, "Subgraph Occurrences: %lld\n", Fase::MotifCount);
  fprintf(f, "Clique-%d Occurrences: %lld\n", K, Fase::cliqueCount);
//  fprintf(f, "Subgraph Types: %lld\n", Fase::getTypes());
  fprintf(f, "Computation Time (ms): %0.4lf\n", Timer::elapsed());
  fprintf(f, "\n");

  for (int i = 0; i < MAXGRAPHS; i++)
    if (Fase::type[i])
      fprintf(f, "Subgraph %d: %lld\n", i, Fase::type[i]);

  for (int i = 0; i <= 29; i++)
      printf("Graphlets %d: %lld\n", i, Fase::graphlets[i]);

  for (int i = 0; i <= 72; i++)
      printf("Orbits %d: %lld\n", i, Fase::orbits[i]);
}

void finish()
{
  Fase::destroy();
  finishNauty();
  delete G;
  fclose(outFile);
}

int main(int argc, char **argv)
{
  init();
  read(argc, argv);
  if (K <= 2)
    return 0;
  initNauty();
  
  //Timer::start();

  Fase::directed = dir;
  Timer::start();

  Fase::GraphletsCount(G, K);

  Timer::stop();
  outputGraphlets();
  //output();
  
  finish();

  return 0;
}
