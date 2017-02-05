
#ifndef TABU_H
#define TABU_H

#include "global.h"

void TabuMove(int city1, int city2);
void DecrementTabu();
int *GetBestNeighbour(int* current);

void TabuSearch(int* current);

#endif /* TABU_H */