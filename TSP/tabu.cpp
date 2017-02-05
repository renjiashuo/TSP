
#include <iostream>
#include "tabu.h"
#include "common.h"
#include "sort.h"

using namespace std;

int tenure = 15;//禁忌长度
int numberOfIterations = 400;//迭代次数

int nbrOfCanSwaps;//邻域解的表的长度
int tabulist[MaxNumOfCities][MaxNumOfCities];//禁忌表
int canSwap[MaxNumOfCities*MaxNumOfCities][2];//邻域解的表
int canSwapCost[MaxNumOfCities*MaxNumOfCities];//邻域解的目标函数值表
int sortedIndex[MaxNumOfCities*MaxNumOfCities];//邻域解的表的索引

//将此路线添加到禁忌表中
void TabuMove(int city1, int city2)
{ 
	tabulist[city1][city2] += tenure;
    tabulist[city2][city1] += tenure;
}

//禁忌表中所有的数减1
void DecrementTabu()
{
	for(int i = 0; i < nbrCities; i++)
		for(int j = 0; j < nbrCities; j++)
			tabulist[i][j] -= (tabulist[i][j] <= 0 ? 0 : 1);
}

//生成最优邻域解
int *GetBestNeighbour(int* current) 
{
	bool swap = false;
    int city1 = 0, city2 = 0;
	int *best = new int[nbrCities]; 
	CopySolution(current, best);
    int bestValue = ComputeDistance(best);

	int next[MaxNumOfCities];
	int nextValue;

	// 生成邻域 
	nbrOfCanSwaps = 0;
    for(int i = 0; i < nbrCities - 1; i++) 
		for (int j = i + 1; j < nbrCities; j++) 
		{
			CopySolution(current, next);
            SwapOperator(i, j, next);
            nextValue = ComputeDistance(next);
			
			sortedIndex[nbrOfCanSwaps] = nbrOfCanSwaps;
			canSwapCost[nbrOfCanSwaps] = nextValue;
			canSwap[nbrOfCanSwaps][0]  = i;
			canSwap[nbrOfCanSwaps][1]  = j;
			nbrOfCanSwaps++;
		}

	sortNondecreasing(canSwapCost, sortedIndex, 0, nbrOfCanSwaps-1);//从小到大排列

	for(int n = 0; n < nbrOfCanSwaps; n++)
	{
		city1 = canSwap[sortedIndex[n]][0];
        city2 = canSwap[sortedIndex[n]][1];
		//如果目标函数值被改进了，则返回值设为找到的解
		if( (canSwapCost[sortedIndex[n]] < bestValue) && (tabulist[city1][city2] < tenure - 2) )
		{
			CopySolution(current, best);
			SwapOperator(city1, city2, best);
			bestValue = canSwapCost[sortedIndex[n]];
			cout << "C1  " ;
			PrintSolution(best, bestValue);
			swap = true;
			break;
		}
		//如果解不在禁忌表中，则返回值设为找到的解
		else if( 0 == tabulist[city1][city2] )	
		{
			CopySolution(current, best);
			SwapOperator(city1, city2, best);
            bestValue = canSwapCost[sortedIndex[n]];
			cout << "C2  " ;
			PrintSolution(best, bestValue);
			swap = true;
			break;
		}
	}

	//没找到可以改进目标函数值的解并且可行解都在禁忌表中，则返回值设为邻域中最小的函数值对应的解
	if( !swap )									
	{
		city1 = canSwap[sortedIndex[0]][0];
        city2 = canSwap[sortedIndex[0]][1];
		CopySolution(current, best);
		SwapOperator(city1, city2, best);
        bestValue = canSwapCost[sortedIndex[0]];
		cout << "C3  " ;
		PrintSolution(best, bestValue);
	}
			
 	DecrementTabu();
    TabuMove(city1, city2);
	return best;
};

//禁忌搜索主函数
void TabuSearch(int* current)
{
	int *next = new int[nbrCities];					//声明一个指针用于存储新的解

    for(int i = 0; i < nbrCities; i++)				//初始化禁忌表
		for(int j = 0; j < nbrCities; j++)
			tabulist[i][j] = 0;

	CopySolution(current, next);					//复制当前解到next中
    for (int i = 0; i < numberOfIterations; i++)	//控制迭代次数
	{
		next = GetBestNeighbour(next);				//得到最优邻域解
        int nextValue = ComputeDistance(next);		//计算新解的目标函数值
        if (nextValue < bestValue)					//如果解被改进了则更新最优解
		{
			bestValue = nextValue;
			CopySolution(next, best);
        }
    }
};