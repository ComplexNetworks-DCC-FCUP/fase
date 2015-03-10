
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
Timers for runtime measurement

---------------------------------------------------- */

#include "Timer.h"

// Static variables
struct timeval Timer::cstart;
struct timeval Timer::cend;

// Stop the clock of a timer
void Timer::start()
{
  gettimeofday(&cstart, NULL);
}

// Elapsed time of a timer
void Timer::stop()
{
  gettimeofday(&cend, NULL);
}

double Timer::elapsed()
{
  return (cend.tv_sec - cstart.tv_sec) + (double)(cend.tv_usec - cstart.tv_usec)/1000000.0;
}
