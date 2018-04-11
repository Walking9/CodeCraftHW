//
// Created by brunon on 18-4-8.
//
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include "PolynomialCurveFitting.h"
#include "ExponentialSmooth.h"

using namespace std;


/***************二次曲线拟合***************/
double SolveDeterminant(int n, double *arr)    //递归解n阶行列式函数, 行列式用一维数组存放
{
    if (n == 1)
        return arr[0];
    double *arr_son = new double[(n - 1)*(n - 1)];//创建n-1阶的代数余子式阵
    int move = 0;                                  //判断行是否移动
    double sum = 0.0;                             //sum为行列式的值
    for (int arow = 0; arow<n; arow++) {
        for (int brow = 0; brow<n - 1; brow++) {
            move = arow > brow ? 0 : 1;
            for (int j = 0; j<n - 1; j++) {
                arr_son[brow*(n - 1) + j] = arr[(brow + move)*n + j + 1];
            }
        }
        int flag = (arow % 2 == 0 ? 1: -1);
        sum += flag* arr[arow*n] * SolveDeterminant(n - 1, arr_son);
    }
    delete[] arr_son;
    return sum;
}

double HelperAdd(int num, int nx, double *y, int ny) {  //求(xi)nx次方 乘 (yi)ny次方的累和, xi = 1, 2, 3, ....
    double sum = 0;
    for(int i=1; i<=num; i++) {
        if(0 != ny) sum += pow(i, nx) * pow(y[i-1], ny);
        else sum += pow(i, nx);
    }
    return sum;
}

void SolveLinearEquations(int n, double *arr, double *vectorB, double *ans) {   //arr是系数矩阵，n是阶数，vectorB，传入右侧向量bn，ans传出解
    double D = SolveDeterminant(n, arr);
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {      //一列替换,交换值
            arr[n*j+i] += vectorB[j];
            vectorB[j] = arr[n*j+i] - vectorB[j];
            arr[n*j+i] -= vectorB[j];
        }
        ans[i] = SolveDeterminant(n, arr)/D;    //求解，即D1, D2...Dn/D
        for(int j=0; j<n; j++) {      //计算后返回原值
            arr[n*j+i] += vectorB[j];
            vectorB[j] = arr[n*j+i] - vectorB[j];
            arr[n*j+i] -= vectorB[j];
        }
    }
}

int TwoTimeFitting(const vector<int> data, int n, int k, int forecase) {
    int DataNum = n / k;

    double *arr_x = new double[DataNum];
    double *arr_y = new double[DataNum];
    for(int i=0; i < DataNum; i++) arr_x[i] = i+1;
//    for (int i = DataNum - 1; i >= 0; i--) {     //数据不加处理
//        arr_y[i] = 0;
//        for (int j = 0; j < k; j++) {
//            tempN--;
//            arr_y[i] += data[tempN];
//        }
//    }
    vector<double> vector_y;
    ExponentialSmooth2(data, n, k, forecase, vector_y);
    for(unsigned int i=0; i<vector_y.size(); i++) {       //对数据用指数平滑处理
        arr_y[i] = vector_y[i];
    }

    double *CoefficientMatrix = new double[9];  //根据公式求3*3系数矩阵
    CoefficientMatrix[0] = DataNum;
    CoefficientMatrix[1] = HelperAdd(DataNum, 1, arr_y, 0);
    CoefficientMatrix[2] = HelperAdd(DataNum, 2, arr_y, 0);
    CoefficientMatrix[3] = CoefficientMatrix[1];
    CoefficientMatrix[4] = CoefficientMatrix[2];
    CoefficientMatrix[5] = HelperAdd(DataNum, 3, arr_y, 0);
    CoefficientMatrix[6] = CoefficientMatrix[2];
    CoefficientMatrix[7] = CoefficientMatrix[5];
    CoefficientMatrix[8] = HelperAdd(DataNum, 4, arr_y, 0);

    double *vectorB = new double[3];
    for(int i=0; i<3; i++) vectorB[i] = HelperAdd(DataNum, i, arr_y, 1);
    double *ans = new double[3];
    SolveLinearEquations(3, CoefficientMatrix, vectorB, ans);
    double y = ans[0] + ans[1]*(DataNum+1) + ans[2]*(DataNum+1)*(DataNum+1);

#ifdef  _DEBUG
    cout << "\nx: ";
    for(int i=0; i<DataNum; i++) cout << arr_x[i] << " ";
    cout << "\ny: ";
    for(int i=0; i<DataNum; i++) cout << arr_y[i] << " ";
    printf("\n拟合方程为：y = %lf + %lfx + %lfx2 \n",ans[0], ans[1], ans[2]);
    printf("if forecase = %d, y=%lf \n\n", forecase, y);
#endif

    delete[] ans;
    delete[] vectorB;
    delete[] CoefficientMatrix;
    if(y < 0) return 0;
    else return (int)ceil(y);
}


int ThreeTimeFitting(const vector<int> data, int n, int k, int forecase) {
    int DataNum = n / k;

    double *arr_x = new double[DataNum];
    double *arr_y = new double[DataNum];
    for(int i=0; i < DataNum; i++) arr_x[i] = i+1;
//    for (int i = DataNum - 1; i >= 0; i--) {     //数据不加处理
//        arr_y[i] = 0;
//        for (int j = 0; j < k; j++) {
//            tempN--;
//            arr_y[i] += data[tempN];
//        }
//    }
    vector<double> vector_y;
    ExponentialSmooth2(data, n, k, forecase, vector_y);
    for(unsigned int i=0; i<vector_y.size(); i++) {       //对数据用二次指数平滑处理
        arr_y[i] = vector_y[i];
    }

    double *CoefficientMatrix = new double[16];  //根据公式求4*4系数矩阵
    CoefficientMatrix[0] = DataNum;
    CoefficientMatrix[1] = HelperAdd(DataNum, 1, arr_y, 0);
    CoefficientMatrix[2] = HelperAdd(DataNum, 2, arr_y, 0);
    CoefficientMatrix[3] = HelperAdd(DataNum, 3, arr_y, 0);
    CoefficientMatrix[4] = CoefficientMatrix[1];
    CoefficientMatrix[5] = CoefficientMatrix[2];
    CoefficientMatrix[6] = CoefficientMatrix[3];
    CoefficientMatrix[7] = HelperAdd(DataNum, 4, arr_y, 0);
    CoefficientMatrix[8] = CoefficientMatrix[2];
    CoefficientMatrix[9] = CoefficientMatrix[3];
    CoefficientMatrix[10] = CoefficientMatrix[7];
    CoefficientMatrix[11] = HelperAdd(DataNum, 5, arr_y, 0);
    CoefficientMatrix[12] = CoefficientMatrix[3];
    CoefficientMatrix[13] = CoefficientMatrix[7];
    CoefficientMatrix[14] = CoefficientMatrix[11];
    CoefficientMatrix[15] = HelperAdd(DataNum, 5, arr_y, 0);

    double *vectorB = new double[4];
    for(int i=0; i<4; i++) vectorB[i] = HelperAdd(DataNum, i, arr_y, 1);
    double *ans = new double[4];
    SolveLinearEquations(4, CoefficientMatrix, vectorB, ans);
    double y = ans[0] + ans[1]*(DataNum+1) + ans[2]*(DataNum+1)*(DataNum+1) + ans[3]*pow(DataNum+1, 3);

#ifdef  _DEBUG
    cout << "\nx: ";
    for(int i=0; i<DataNum; i++) cout << arr_x[i] << " ";
    cout << "\ny: ";
    for(int i=0; i<DataNum; i++) cout << arr_y[i] << " ";
    printf("\n拟合方程为：y = %lf + %lfx + %lfx2 + %lfx3 \n",ans[0], ans[1], ans[2], ans[3]);
    printf("if forecase = %d, y=%lf \n\n", forecase, y);
#endif

    delete[] ans;
    delete[] vectorB;
    delete[] CoefficientMatrix;
    if(y < 0) return 0;
    else return (int)ceil(y);
}


int ThreeTimeFittingDataProcessing(const vector<int> data, int n, int k, int forecase) {
    int DataNum = n / k, tempN = n;

    double *arr_x = new double[DataNum];
    double *arr_y = new double[DataNum];
    for(int i=0; i < DataNum; i++) arr_x[i] = i+1;
    for (int i = DataNum - 1; i >= 0; i--) {     //数据不加处理
        arr_y[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            arr_y[i] += data[tempN];
        }
    }

    double *CoefficientMatrix = new double[16];  //根据公式求4*4系数矩阵
    CoefficientMatrix[0] = DataNum;
    CoefficientMatrix[1] = HelperAdd(DataNum, 1, arr_y, 0);
    CoefficientMatrix[2] = HelperAdd(DataNum, 2, arr_y, 0);
    CoefficientMatrix[3] = HelperAdd(DataNum, 3, arr_y, 0);
    CoefficientMatrix[4] = CoefficientMatrix[1];
    CoefficientMatrix[5] = CoefficientMatrix[2];
    CoefficientMatrix[6] = CoefficientMatrix[3];
    CoefficientMatrix[7] = HelperAdd(DataNum, 4, arr_y, 0);
    CoefficientMatrix[8] = CoefficientMatrix[2];
    CoefficientMatrix[9] = CoefficientMatrix[3];
    CoefficientMatrix[10] = CoefficientMatrix[7];
    CoefficientMatrix[11] = HelperAdd(DataNum, 5, arr_y, 0);
    CoefficientMatrix[12] = CoefficientMatrix[3];
    CoefficientMatrix[13] = CoefficientMatrix[7];
    CoefficientMatrix[14] = CoefficientMatrix[11];
    CoefficientMatrix[15] = HelperAdd(DataNum, 5, arr_y, 0);

    double *vectorB = new double[4];
    for(int i=0; i<4; i++) vectorB[i] = HelperAdd(DataNum, i, arr_y, 1);
    double *ans = new double[4];
    SolveLinearEquations(4, CoefficientMatrix, vectorB, ans);
    double y = ans[0] + ans[1]*(DataNum+1) + ans[2]*(DataNum+1)*(DataNum+1) + ans[3]*pow(DataNum+1, 3);

#ifdef  _DEBUG
    cout << "\nx: ";
    for(int i=0; i<DataNum; i++) cout << arr_x[i] << " ";
    cout << "\ny: ";
    for(int i=0; i<DataNum; i++) cout << arr_y[i] << " ";
    printf("\n拟合方程为：y = %lf + %lfx + %lfx2 + %lfx3 \n",ans[0], ans[1], ans[2], ans[3]);
    for(int i=0; i<DataNum+1; i++) cout << ans[0] + ans[1]*(i+1) + ans[2]*(i+1)*(i+1) + ans[3]*pow(i+1, 3) << " ";
#endif

    delete[] ans;
    delete[] vectorB;
    delete[] CoefficientMatrix;
    if(y < 0) return 0;
    else return (int)ceil(y);
}

/***************一次曲线拟合*************/
int LineFitting(const vector<int> data, int n, int k, int forecase) {
    int DataNum = n / k, tempN = n;

    int *arr_x = new int[DataNum];
    int *arr_y = new int[DataNum];
    for(int i=0; i < DataNum; i++) arr_x[i] = i+1;
    for (int i = DataNum - 1; i >= 0; i--) {
        arr_y[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            arr_y[i] += data[tempN];
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
    else return (int)round(y);
}

