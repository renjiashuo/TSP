#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>

#include "global.h"
#include "common.h"
#include "random.h"
#include "ls.h"
#include "sa.h"
#include "tabu.h"
#include "ga.h"

using namespace std;

void Data_29();
void Data_EUC_2D_52();
void Data_ATT_48();

int main()
{
	clock_t start, finish;						//时钟用于计算算法的时间
	double  duration;							//算法的时间duration = finish - start;

	cout << "请输入要计算的算例。" << endl;
	cout << "A.29个城市的算例，城市间距离以方阵给出。(bays29.dat)" << endl;
	cout << "B.52个城市的算例，城市以笛卡尔坐标给出。(berlin52.dat)" << endl;
	cout << "C.48个城市的算例，城市以att方式给出。(att48.dat)" << endl;
	char dataName = ' ';
	cin >> dataName;
	while (dataName != 'A' && dataName != 'B' && dataName != 'C')
	{
		cout << "输入有误请重新输入。" << endl;
		cin >> dataName;
	}
	if (dataName == 'A')
		Data_29();								//29个城市的算例，城市间距离以方阵给出
	else if (dataName == 'B')
		Data_EUC_2D_52();							//52个城市的算例，城市以笛卡尔坐标给出
	else if (dataName == 'C')
		Data_ATT_48();							//48个城市的算例，城市以att方式给出

	int current[100] = {0};						//初始解
	for(int i = 0; i < nbrCities; i++)
		current[i] = i;
	
	int distance = ComputeDistance(current);	//初始目标函数值
	
	CopySolution(current, best);				//初始最优设为初始解
	
	bestValue = distance;						//初始最优目标函数值设为初始目标函数值
	
	start = clock();							//开始计时

	cout << "请输入使用的算法。" << endl;
	cout << "1.邻域搜索。" << endl;
	cout << "2.禁忌搜索算法。" << endl;
	cout << "3.模拟退火算法。" << endl;
	cout << "4.遗传算法。" << endl;
	int algorithmName = 0;
	cin >> algorithmName;
	while (algorithmName != 1 && algorithmName != 2 && algorithmName != 3 && algorithmName != 4)
	{
		cout << "输入有误请重新输入。" << endl;
		cin >> algorithmName;
	}
	if (algorithmName == 1)
		LocalSearch(current);					//邻域搜索
	else if (algorithmName == 2)
		TabuSearch(current);					//禁忌搜索算法
	else if (algorithmName == 3)
		SimulatedAnnealing(current);			//模拟退火算法
	else if (algorithmName == 4)
		GeneticAlgorithm();					//遗传算法

	finish = clock();							//终止计时
	PrintSolution(best, bestValue);				//显示最优解
	PrintSolution_txt(best, bestValue);			//最优解输出到txt文件中
	duration = finish - start;					//计算花费时间
	cout << "duration : " << duration << endl;	//显示花费时间
	system("PAUSE");							//停一下我看看
	return 0;
};

void Data_29()
{
	vector<int> dist_vector;
	const char* filename = "bays29.dat";
	ifstream in(filename);
	while (in) 
	{
		int temp;
		in >> temp;
		dist_vector.push_back(temp);
	}
	dist_vector.pop_back();
	nbrCities = sqrt(double(dist_vector.size()));
	for(int i = 0; i < nbrCities; i++)
		for(int j = 0; j < nbrCities; j++)
			dist[i][j] = dist_vector[i * nbrCities + j];
}

void Data_EUC_2D_52()
{
	vector<int> dist_vector;
	const char* filename = "berlin52.dat";
	ifstream in(filename);
	while (in) 
	{
		int temp;
		in >> temp;
		dist_vector.push_back(temp);
	}
	dist_vector.pop_back();
	nbrCities = dist_vector.size()/2;
	for(int i = 0; i < nbrCities; i++)
		for(int j = 0; j < nbrCities; j++)
			dist[i][j] = sqrt(pow(double(dist_vector[i*2] - dist_vector[j*2]),2) + pow(double(dist_vector[i*2+1] - dist_vector[j*2+1]),2));
}

void Data_ATT_48()
{
	vector<int> dist_vector;
	const char* filename = "att48.dat";
	ifstream in(filename);
	while (in) 
	{
		int temp;
		in >> temp;
		dist_vector.push_back(temp);
	}
	dist_vector.pop_back();
	nbrCities = dist_vector.size()/2;
	double r12[48][48];
	for(int i = 0; i < nbrCities; i++)
		for(int j = 0; j < nbrCities; j++)
			r12[i][j] = sqrt((pow(double(dist_vector[i*2] - dist_vector[j*2]),2) + pow(double(dist_vector[i*2+1] - dist_vector[j*2+1]),2))/10);
	int t12[48][48];
	for(int i = 0; i < nbrCities; i++)
		for(int j = 0; j < nbrCities; j++)
			t12[i][j] = r12[i][j];
	int d12[48][48];
	for(int i = 0; i < nbrCities; i++)
		for(int j = 0; j < nbrCities; j++)
		{
			if (double(t12[i][j]) < r12[i][j])
				d12[i][j] = (int) t12[i][j] + 1;
			else
				d12[i][j] = (int) t12[i][j];
		}
	for(int i = 0; i < nbrCities; i++)
		for(int j = 0; j < nbrCities; j++)
			dist[i][j] = d12[i][j];
}

//srand((unsigned)time(NULL));