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

//void EMatrix(int Vx[], int Vy[], int n, int ex, double coefficient[]);
//void CalEquation(int exp, double coefficient[]);
//double Em[6][4];
//
////计算方程组的增广矩阵
//void EMatrix(int Vx[], int Vy[], int n, int ex, double coefficient[]) {
//    double ReSum = 0;
//    for (int i=1; i<=ex; i++) {
//        for (int j=1; j<=ex; j++) {
//            ReSum = 0;
//            for(int k=0; k<n; k++) ReSum += pow(Vx[k], i+j-2);
//            Em[i][j] = ReSum;
//        }
//        ReSum = 0;
//        for(int k=0; k<n; k++) ReSum += pow(Vx[k], i-1)*Vy[k];
//        Em[i][ex+1] = ReSum;
//    }
//    Em[1][1] = n;
//    CalEquation(ex, coefficient);
//}
//
////求解方程
//void CalEquation(int exp, double coefficient[]) {
//    for(int k=1; k<exp; k++) { //消元过程
//        for(int i=k+1; i<exp+1; i++) {
//            double p1=0;
//            if(0 != Em[k][k])
//                p1 = Em[i][k]/Em[k][k];
//            for(int j=k; j<exp+2; j++)
//                Em[i][j] = Em[i][j] - Em[k][j]*p1;
//        }
//    }
//    coefficient[exp] = Em[exp][exp+1]/Em[exp][exp];
//    double sum = 0;
//    for(int l=exp-1; l>=1; l--) {   //回代求解
//        sum = 0;
//        for(int i=l+1; i<=exp; i++)
//            sum += Em[l][i]*coefficient[i];
//        coefficient[l] = (Em[l][exp + 1] - sum) / Em[l][l];
//    }
//}
//
//int PolynomialCurveFitting(const vector<int> data, int n, int k) {
//    int DataNum = n / k, tempN = n;
//    //数据预处理
//    int avg = 0;
//    for (unsigned int i=0; i<data.size(); i++) {
//        avg += data[i];
//    }
//    avg = (int)round((double)avg/n);
//
//    int *arr_x = new int[DataNum];
//    int *arr_y = new int[DataNum];
//    for(int i=0; i < DataNum; i++) arr_x[i] = i+1;
//    for (int i = DataNum - 1; i >= 0; i--) {
//        arr_y[i] = 0;
//        for (int j = 0; j < k; j++) {
//            tempN--;
//            if(0 != avg && data[tempN] > 5*avg) arr_y[i] += avg;
//            else arr_y[i] += data[tempN];
//        }
//    }
//    double coefficient[5];
//    memset(coefficient,0,sizeof(double)*5);    //coefficient清零
//    EMatrix(arr_x, arr_y, 5, 3, coefficient);
//
//    int x = DataNum + 1;
//    double y = coefficient[1] + coefficient[2]*x + coefficient[3]*x*x;
//
//#ifdef _DEBUG
//    cout << "\nx: ";
//    for(int i=0; i<DataNum; i++) cout << arr_x[i] << " ";
//    cout << "\ny: ";
//    for(int i=0; i<DataNum; i++) cout << arr_y[i] << " ";
//
//    printf("\n拟合方程为：y = %lf + %lfx + %lfx^2 \n",coefficient[1],coefficient[2],coefficient[3]);
//    printf("if x = %d, y=%lf \n\n", x, y);
//#endif
//
//    delete[] arr_x;
//    delete[] arr_y;
//    if(y < 0) return 0;
//    else return (int)round(y);
//}


int LineFitting(const vector<int> data, int n, int k, int forecase) {
    int DataNum = n / k, tempN = n;
    //数据预处理
    int avg = 0;
    for (unsigned int i=0; i<data.size(); i++) {
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

    int N = DataNum;
    int sumX = (1+N)*N/2;
    int sumY = 0;
    for(int i=0; i<N; i++) sumY += arr_y[i];
    int sumX2 = (N * (N+1) * (2*N+1))/6;
    int sumXY = 0;
    for(int i=0; i<N; i++) sumXY += arr_y[i] * (i+1);
    double a, b;
    b = ((double)sumX*sumY/N - sumXY) / (sumX*sumX/N - sumX2);
    a = ((double)sumY - b*sumX)/N;
    double y = 0;
    for(int i=N+1; i<=N+forecase; i++) y += (a + b*(N+1));
#ifdef _DEBUG
    cout << "\nx: ";
    for(int i=0; i<DataNum; i++) cout << arr_x[i] << " ";
    cout << "\ny: ";
    for(int i=0; i<DataNum; i++) cout << arr_y[i] << " ";

    printf("\n拟合方程为：y = %lf + %lfx \n",a, b);
    printf("if forecase = %d, y=%lf \n\n", forecase, y);
#endif
    delete[] arr_x;
    delete[] arr_y;
    if(y < 0) return 0;
    else return (int)ceil(y);
}

#endif //ECS_POLYNOMIALCURVEFITTING_H
