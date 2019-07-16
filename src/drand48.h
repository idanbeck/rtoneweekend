#ifndef DRAND48_H_
#define DRAND48_H_

#ifdef _WIN32

#include <cstdlib>
#include <time.h>

double drand48() {
	return ((double)(std::rand()) / ((double)(RAND_MAX) + 1.0f));
}

#endif

void InitializeRand() {
	srand(time(NULL));
}

#endif // DRAND48_H_
