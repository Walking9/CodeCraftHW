//
// Created by brunon on 18-4-1.
//

#ifndef ECS_POLYNOMIALCURVEFITTING_H
#define ECS_POLYNOMIALCURVEFITTING_H

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;


/***************二次曲线拟合***************/
double SolveDeterminant(int n, double *arr);    //递归解n阶行列式函数, 行列式用一维数组存放
double HelperAdd(int num, int nx, double *y, int ny);  //求(xi)nx次方 乘 (yi)ny次方的累和, xi = 1, 2, 3, ....
void SolveLinearEquations(int n, double *arr, double *vectorB, double *ans);  //arr是系数矩阵，n是阶数，vectorB，传入右侧向量bn，ans传出解
int TwoTimeFitting(const vector<int> data, int n, int k);

/***************三次曲线拟合***************/
int ThreeTimeFitting(const vector<int> data, int n, int k);
void ThreeTimeFittingDataProcessing(const vector<int> data, int n, int k, double Dividing, vector<int>& output);
/***************一次曲线拟合*************/
int LineFitting(const vector<int> data, int n, int k);

#endif //ECS_POLYNOMIALCURVEFITTING_H
