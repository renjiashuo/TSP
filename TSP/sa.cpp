
#include <iostream>
#include <complex>
#include <time.h>
#include "global.h"
#include "common.h"
#include "random.h"
#include "sa.h"

using namespace std;

double temperature = 100.0;	//初始温度
double alpha       = 0.98;	//冷却系数
double epsilon     = 0.001;	//终止温度
int    L           = 500;	//循环次数


//随机得到一个邻域解
void ComputeNext(int *current, int *next)
{
    for(int i = 0; i < nbrCities; i++)
		next[i] = current[i];

    int i1 = randomInteger(0,nbrCities - 1);
    int i2 = randomInteger(0,nbrCities - 1);
	while( i1 == i2)
		i2 = randomInteger(0,nbrCities - 1);

	SwapOperator(i1, i2, next); 
}

//模拟退火主函数
void SimulatedAnnealing(int *current)
{
	srand((unsigned)time(NULL));						//播下一颗时间的种子

	int    delta;										//目标函数值的改变量
	double proba;										//接受劣解概率变量
	int    *next = new int[nbrCities];					//新解存放变量
	int    distance = ComputeDistance(current);			//计算当前目标函数值

    //当温度没有达到终止温度时进行循环
    while(temperature > epsilon)
    {
		for(int l = 0; l < L; l++)
		{
			ComputeNext(current,next);					//随机得到一个邻域解
			delta = ComputeDistance(next) - distance;	//得到新解的目标函数值与当前目标函数值的差值
			if(delta < 0)								//如果解改进了
			{
				Accept(current,next);					//接受新解
				distance = delta + distance;			//更新当前目标函数值
				if( distance < bestValue )				//如果当前解优于最优解则更新最优解
				{
					bestValue = distance;
					CopySolution(current,best);
				}
				cout << distance << endl;				//输出一下我看看
			}
			else										//如果解没有改进
			{
				proba = random01();						//接受概率
				if( proba < exp(-delta / temperature) )	//如果满足接受概率公式则接受劣解
				{
					Accept(current,next);
					distance = delta + distance;
					cout << distance << endl;			//输出一下我看看
				}
			}
		}
        temperature *=alpha;							//冷却过程
    }
}