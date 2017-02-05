
#ifndef COMMON_H
#define COMMON_H

int  ComputeDistance(int *tour);
int  FindCity(int city, int *tour);
void SwapOperator(int city1, int city2, int *tour);
void Accept(int *current, int *newSol);
void CopySolution(int *from, int *to);
void PrintSolution(int *tour, int cost);
void PrintSolution_txt(int *tour, int cost);

#endif /* COMMON_H */