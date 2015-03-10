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

#ifndef _RANDOM_
#define _RANDOM_

#include "Common.h"

class Random
{

 public:
  static void init(unsigned int s);
  static int getInteger(int a, int b);
  static double getDouble();
  static int testProb(double p);
};

#endif
