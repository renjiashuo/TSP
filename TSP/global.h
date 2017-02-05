
#ifndef GLOBAL_H
#define GLOBAL_H

#define MaxNumOfCities  100

extern  int nbrCities;//城市数
extern  int dist[MaxNumOfCities][MaxNumOfCities];//城市间距离

extern  int best[MaxNumOfCities];//最优解
extern  int bestValue;//最优解的值

#endif /* GLOBAL_H */