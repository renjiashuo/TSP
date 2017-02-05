
#ifndef GA_H
#define GA_H

#define PopSize  600		//种群大小

void BuildWheel();
void ComputeFitness();
void GenerateRandomTour(int *tour);
int  SelectIndividual();
void PMX(int lo, int hi, int *tour1, int *tour2, int *newtour1, int *newtour2);
void Mutation();

void Initialize();

void GeneticAlgorithm();

#endif /* GA_H */