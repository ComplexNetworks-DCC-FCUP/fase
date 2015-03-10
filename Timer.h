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
Timers for runtime mesurement

---------------------------------------------------- */

#ifndef _TIMER_
#define _TIMER_

#include <sys/time.h>
#include <stdlib.h>

class Timer
{
 private:
  static struct timeval cstart; // Start times
  static struct timeval cend; // End times

 public:
  static void start(); // Start the clock of a timer
  static void stop(); // Stop the clock of a timer
  static double elapsed(); // Elapsed time of a timer
};

#endif
