
#include <iostream>
#include <fstream>

#include "global.h"
#include "common.h"

using namespace std;

int ComputeDistance(int *tour)//计算目标函数的值，即旅行总距离。
{
	int distance = 0;
	for(int i = 0; i < nbrCities - 1; i++)
		distance += dist[tour[i]][tour[i+1]];
	distance += dist[tour[nbrCities-1]][tour[0]];
	return distance;
};

int  FindCity(int city, int *tour)//根据城市编号在解中寻找城市的位置
{
	int j = 0;
	while( tour[j] != city )
		j++;
	return j;
};

void SwapOperator(int city1, int city2, int *tour)//邻域交换
{
	int temp = tour[city1];
    tour[city1] = tour[city2];
    tour[city2] = temp;
};

void Accept(int *current, int *newSol)//用newSol替换当前解
{
    for(int i = 0; i < nbrCities; i++)
		current[i] = newSol[i];
};

void CopySolution(int *from, int *to)//将解from复制到to中
{
    for(int i = 0; i < nbrCities; i++)
		to[i] = from[i];
};

void PrintSolution(int *tour, int cost)//将解输出到屏幕
{
	cout << "cost : " << cost << ", solution :  ";
	for(int i = 0; i < nbrCities; i++)
		cout << tour[i] << "  ";
	cout << endl;
};

void PrintSolution_txt(int *tour, int cost)//将解输出到文本文档
{
	char result[] = "result.txt";
	ofstream fout(result);
	fout << "cost : " << cost << endl << "solution :  " << endl;
	for(int i = 0; i < nbrCities; i++)
		fout << tour[i] << "\t";
	fout << endl;
};