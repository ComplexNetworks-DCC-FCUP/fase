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
Random generator

---------------------------------------------------- */

#include "Random.h"

void Random::init(unsigned int s)
{
  srand(s);
}

int Random::getInteger(int a, int b)
{
  return a + rand() % (b - a + 1);
}

double Random::getDouble()
{
  return rand() / (double) RAND_MAX;
}

int Random::testProb(double p)
{
  if (p > 0.9999999) return 1;
  return (rand() / (double) RAND_MAX) < p;
}


