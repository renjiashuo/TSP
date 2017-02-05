
#include <iostream>
#include "ls.h"
#include "global.h"
#include "common.h"

using namespace std;

void LocalSearch(int *current)//邻域搜索
{
	int  distance = ComputeDistance(current);//计算当前目标函数值
	int  *next = new int[nbrCities];
	int  delta;
	bool improved = true;

	while( improved )
	{
		improved = false;
	    for(int i = 0; i < nbrCities - 1; i++)
			for(int j = i + 1; j < nbrCities; j++)
		{
			CopySolution(current, next);
			SwapOperator(i, j, next);//邻域交换
			delta = ComputeDistance(next) - distance;
			if( delta < 0 )//如果有改进
			{
				improved = true;
				Accept(current,next);//更新解
				distance = delta + distance;//更新目标函数值
				bestValue = distance;//更新最优值
				CopySolution(current,best);//更新最优解
				cout << distance << endl;//输出当前目标函数值
			}
		}
	}
};