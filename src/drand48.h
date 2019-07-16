#ifndef DRAND48_H_
#define DRAND48_H_

#include <cstdlib>
#include <time.h>

#ifndef drand48() 
double drand48() {
	return ((double)(std::rand()) / ((double)(RAND_MAX) + 1.0f));
}
#endif

void InitializeRand() {
	srand(time(NULL));
}

#endif // DRAND48_H_