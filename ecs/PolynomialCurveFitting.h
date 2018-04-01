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

//最小二乘拟合相关函数定义
//double sum(vector<double> Vnum, int n);
//double MutilSum(int Vx[], int Vy[], int n);
double RelatePow(int Vx[], int n, int ex);
double RelateMutiXY(int Vx[], int Vy[], int n, int ex);
void EMatrix(int Vx[], int Vy[], int n, int ex, double coefficient[]);
void CalEquation(int exp, double coefficient[]);
double F(double c[],int l,int m);
double Em[6][4];


////累加
//double sum(vector<double> Vnum, int n)
//{
//    double dsum=0;
//    for (int i=0; i<n; i++)
//    {
//        dsum+=Vnum[i];
//    }
//    return dsum;
//}
//
////乘积和
//double MutilSum(vector<double> Vx, vector<double> Vy, int n)
//{
//    double dMultiSum=0;
//    for (int i=0; i<n; i++)
//    {
//        dMultiSum+=Vx[i] * Vy[i];
//    }
//    return dMultiSum;
//}

//ex次方和
double RelatePow(int Vx[], int n, int ex)
{
    double ReSum=0;
    for (int i=0; i<n; i++)
    {
        ReSum += pow(Vx[i],ex);
    }
    return ReSum;
}

//x的ex次方与y的乘积的累加
double RelateMutiXY(int Vx[], int Vy[], int n, int ex)
{
    double dReMultiSum=0;
    for (int i=0; i<n; i++)
    {
        dReMultiSum+=pow(Vx[i],ex) * Vy[i];
    }
    return dReMultiSum;
}

//计算方程组的增广矩阵
void EMatrix(int Vx[], int Vy[], int n, int ex, double coefficient[])
{
    for (int i=1; i<=ex; i++)
    {
        for (int j=1; j<=ex; j++)
        {
            Em[i][j] = RelatePow(Vx, n, i+j-2);
        }
        Em[i][ex+1] = RelateMutiXY(Vx, Vy, n, i-1);
    }
    Em[1][1] = n;
    CalEquation(ex, coefficient);
}

//求解方程
void CalEquation(int exp, double coefficient[])
{
    for(int k=1; k<exp; k++) { //消元过程
        for(int i=k+1; i<exp+1; i++) {
            double p1=0;
            if(0 != Em[k][k])
                p1 = Em[i][k]/Em[k][k];
            for(int j=k; j<exp+2; j++)
                Em[i][j] = Em[i][j] - Em[k][j]*p1;
        }
    }
    coefficient[exp] = Em[exp][exp+1]/Em[exp][exp];
    for(int l=exp-1; l>=1; l--)   //回代求解
        coefficient[l] = (Em[l][exp+1]-F(coefficient,l+1,exp))/Em[l][l];
}

//供CalEquation函数调用
double F(double c[],int l,int m)
{
    double sum=0;
    for(int i=l;i<=m;i++)
        sum+=Em[l-1][i]*c[i];
    return sum;
}

int PolynomialCurveFitting(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;
    //数据预处理
    int avg = 0;
    for (int i=0; i<data.size(); i++) {
        avg += data[i];
    }
    avg = (int)round((double)avg/n);

    int *arr_x = new int[DataNum];
    int *arr_y = new int[DataNum];
    for(int i=0; i < DataNum; i++) arr_x[i] = i+1;
    for (int i = DataNum - 1; i >= 0; i--) {
        arr_y[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            if(0 != avg && data[tempN] > 5*avg) arr_y[i] += avg;
            else arr_y[i] += data[tempN];
        }
    }
    double coefficient[5];
    memset(coefficient,0,sizeof(double)*5);    //coefficient清零
    EMatrix(arr_x, arr_y, 5, 3, coefficient);
    printf("\n拟合方程为：y = %lf + %lfx + %lfx^2 \n",coefficient[1],coefficient[2],coefficient[3]);
    int x = DataNum + 1;
    double y = coefficient[1] + coefficient[2]*x + coefficient[3]*x*x;
    printf("if x = %d, y=%lf \n\n", x, y);
    delete[] arr_x;
    delete[] arr_y;
    if(y < 0) return 0;
    else return (int)round(y);
}

#endif //ECS_POLYNOMIALCURVEFITTING_H
