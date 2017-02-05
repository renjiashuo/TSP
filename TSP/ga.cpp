
#include <iostream>
#include <time.h>
#include "global.h"
#include "ga.h"
#include "random.h"
#include "common.h"

using namespace std;

int    individual[2*PopSize][MaxNumOfCities];				//种群
int    fitness[2*PopSize];									//种群目标函数值
double prob[PopSize];

int    Gen = 3000;											//循环次数
double Pc  = 0.5;											//交叉概率
double Pm  = 0.2;											//变异概率

//生成随机个体
void GenerateRandomTour(int *tour) 
{
	int  numUnVisitedCities = nbrCities - 1;				//未加入解中城市的计数器
	int *unVisitedCities    = new int[nbrCities];			//未加入解中城市列表

	//未加入的解中的城市列表初始化
	for(int i = 0; i < nbrCities; i++) 
    	unVisitedCities[i] = i;

	for(int i = 0; i < nbrCities; i++)						//作总城市个数次循环
    {
	    int index = randomInteger(0, numUnVisitedCities--);	//随机生成一个城市
      	tour[i] = unVisitedCities[index];					//将该城市添加到当前解的当前位置
      	for(int j = index; j <= numUnVisitedCities; j++)	//在未加入的城市列表中删除该城市
      		unVisitedCities[j] = unVisitedCities[j+1];      	 
    }
};

//计算目标函数值
void ComputeFitness()
{
	int max = -1;
	for(int i = 0; i < PopSize; i++)
	{
		fitness[i] = ComputeDistance(individual[i]);		//计算每个个体的旅行总距离
		max = fitness[i] > max ? fitness[i] : max;			//找出当前解中的旅行总距离最大值
		if( fitness[i] < bestValue )						//找出当前解中的最优解
		{
			bestValue = fitness[i];
			CopySolution(individual[i],best);
			cout << bestValue << endl;
		}
	}

	max = max + 1;
	for(int i = 0; i < PopSize; i++)						//令fitness等于旅行总距离最大值与当前解旅行总距离的差值
		fitness[i] = max - fitness[i];
};

//制作轮盘赌的轮盘
void BuildWheel()
{
	double sum = 0;
	for(int i = 0; i < PopSize; i++)						//计算种群每个个体目标函数值之和
		sum += fitness[i];
	prob[0] = (double)fitness[0] / (double)sum;				//第一个个体占总和的分数
	for( int i = 1 ; i < PopSize ; i++ )					//计算每个个体占总和的累计分数
		prob[i] = prob[i-1] + fitness[i] / sum;
};

//轮盘赌方式选取一个个体
int  SelectIndividual()
{
	double p = random01();
	if( p < prob[0] )
		return 0;
	if( p >= prob[PopSize - 1] )
		return (PopSize - 1);
	for( int i = 1 ; i < PopSize ; i++ )
		if( (p >= prob[i-1]) && (p < prob[i]) )
			return i;
	return -1;
};

//从lo到hi做交叉
void PMX(int lo, int hi, int *tour1, int *tour2, int *newtour1, int *newtour2)
{
	int j, pos1, pos2;
	CopySolution(tour1, newtour1);
	CopySolution(tour2, newtour2);

	j = lo;
	while( j <= hi )
	{
		pos1  = FindCity(tour2[j], newtour1);
		pos2  = FindCity(tour1[j], newtour2);
		SwapOperator(j, pos1, newtour1);
		SwapOperator(j, pos2, newtour2);
		j++;
	}
};

//变异
void Mutation()
{
	for(int i = 0; i < PopSize; i++)					//对种群中每个个体做循环
	{
		if( random01() < Pm )							//变异概率
		{
			int i1 = randomInteger(0,nbrCities - 1);	//随机生成两个数
			int i2 = randomInteger(0,nbrCities - 1);
			while( i1 == i2)
				i2 = randomInteger(0,nbrCities - 1);

			SwapOperator(i1, i2, individual[i]);		//对解中这两个位置的数交换
		}
	}
};

//初始化
void Initialize()
{
	for(int i = 0; i < PopSize; i++)		//给初始种群生成PopSize个个体
		GenerateRandomTour(individual[i]);
};

//选择和交叉
void SelectionAndCrossover()
{
	BuildWheel();										//制作轮盘赌的轮盘
	for(int i = PopSize; i < 2*PopSize; i = i + 2)		
	{
		int father = SelectIndividual();				//轮盘赌方式选取一个个体
		int mother = SelectIndividual();				//轮盘赌方式选取另一个个体
		
		if( random01() < Pc )							//如果满足交叉概率
		{
			int lo = randomInteger(1, nbrCities - 2);	//随机两个数作为交叉段的起始点和终止点
			int hi = randomInteger(1, nbrCities - 2);
			while( lo == hi)
				hi = randomInteger(1, nbrCities - 2);
			int min = lo < hi ? lo : hi;
			int max = lo < hi ? hi : lo;
			lo = min;
			hi = max;
			//从lo到hi这段长度对father和mother做交叉然后存到indevidual中
			PMX(lo, hi, individual[father], individual[mother], individual[i], individual[i+1]);
		}
		else											//如果不满足交叉概率
		{
			CopySolution(individual[father], individual[i]  );
			CopySolution(individual[mother], individual[i+1]);
		}
	}

	for(int i = 0; i < PopSize; i++)					//更新整个种群
		CopySolution(individual[PopSize+i], individual[i]);
}

//遗传算法主函数
void GeneticAlgorithm()
{
	srand((unsigned)time(NULL));	//播下一颗时间的种子

	Initialize();					//初始化
	ComputeFitness();				//计算初始化所得的种群的目标函数值

	for(int g = 1; g <= Gen; g++)	//循环总次数
	{
		SelectionAndCrossover();	//选择和交叉
		Mutation();					//变异
		ComputeFitness();			//更新新种群对应每个个体的目标函数值
	}
}