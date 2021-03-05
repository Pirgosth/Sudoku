#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <stdlib.h>
#include <time.h>   
#include <random>

static std::mt19937 *mt = nullptr;

void initRand();

void destroyRand();

int randRange(int a, int b);

#endif //RANDOM_H_INCLUDED
