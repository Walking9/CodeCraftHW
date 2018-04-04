//
// Created by brunon on 18-4-1.
//

#ifndef ECS_EXPONENTIALSMOOTH_H
#define ECS_EXPONENTIALSMOOTH_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
//一次指数平滑预测
//传入的数据data以天为单位, n为数组大小,参数k表示k天为一组数据, forcase表示需要预测几组数据
//备注：n最好为k的倍数
//     实验发现用double和用float差距比较大
int ExponentialSmooth1(const vector<int> data, int n, int k) {
    int DataNum = n/k, tempN = n;
    int* DataHandled = new int[DataNum];
    for(int i=DataNum-1; i>=0; i--) {
        DataHandled[i] = 0;
        for(int j = 0; j<k; j++) {
            DataHandled[i] += data[--tempN];
        }
    }
    vector<double> s1, output;
    int x=0;
    for(int i=0; i<3; i++)
        x += DataHandled[i];
    s1.push_back((double)x/3);   //置s1初始值
    double a = 0.65;            //平滑系数暂时置为0.6,参数需要调节！！！！！！！！
    double MSE = 0, tempDouble;
    for(int i=0; i<DataNum; i++) {
        s1.push_back(a * DataHandled[i] + (1.0-a) * s1[i]); //使用公式
        tempDouble = s1[i] - DataHandled[i];
        MSE += tempDouble*tempDouble;
    }
    MSE = sqrt(MSE) / DataNum;
    delete[] DataHandled;
#ifdef _DEBUG
    printf("一次指数平滑预估值为： %lf, 均方误差为： %lf \n", s1[s1.size()-1], MSE);
#endif
    return (int)round(s1[s1.size()-1]);
}


//data格式： int data[] = {0, 2, 1, 1, 0, 6, 2, 1 ,0, 2, 1, 4, 3, 10, 2, 3, 1, 3, 3, 2, 5, 10, 1, 2, 8, 1, 2, 4, 2, 3};
//二次指数平滑预测
//传入的数据data以天为单位, n为数组大小,参数k表示k天为一组数据, forcase表示需要预测几组数据
//备注：n最好为k的倍数
//     实验发现用double和用float差距比较大
int ExponentialSmooth2(const vector<int> data, int n, int k, int forecase) {
    int DataNum = n / k, tempN = n;
    //数据预处理
    int avg = 0;
    for (int i=0; i<data.size(); i++) {
        avg += data[i];
    }
    avg = (int)round((double)avg/n);

    int *DataHandled = new int[DataNum];
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            if(0 != avg && data[tempN] > 5*avg) DataHandled[i] += avg;
            else DataHandled[i] += data[tempN];
        }
    }
    vector<double> s2_1_new, s2_2_new;
    double x=0;
    for(int i=0; i<3; i++)
        x += DataHandled[i];
    x /= 3;
    double s2_1 = x, s2_2 = x;   //置s2_1  s2_2初始值
    double a = 0.6;      //平滑系数0.65
    double MSE = 0, tempDouble;

    //先计算一次指数平滑的值
    for(int i=0; i<DataNum; i++) {
        if(0 == i) {
            s2_1_new.push_back(a*DataHandled[i] + (1-a)*s2_1);
        } else {
            s2_1_new.push_back(a*DataHandled[i] + (1-a)*s2_1_new[i-1]);
        }
    }

    //再计算二次指数平滑的值
    for(int i=0; i<DataNum; i++) {
        if(0 == i) {
            s2_2_new.push_back(a*s2_1_new[i] + (1-a)*s2_2);
        } else {
            s2_2_new.push_back(a*s2_1_new[i] + (1-a)*s2_2_new[i-1]);
        }
        tempDouble = (s2_2_new[i] - DataHandled[i]);
        MSE += tempDouble*tempDouble;
    }
    MSE = sqrt(MSE) / DataNum;
//    int forecase = 1;       //要预测几期，暂时置1!!!!!!!!!!!!!!
    double At = s2_1_new[s2_1_new.size()-1] * 2 - s2_2_new[s2_2_new.size()-1];
    double Bt = (a / (1-a)) * (s2_1_new[s2_1_new.size()-1] - s2_2_new[s2_2_new.size()-1] );
    double Xt = 0, tmp;
    for(int i=1; i<=forecase; i++) {
        tmp = At + Bt * forecase;
        tmp = tmp > 0 ? tmp : 0;
        Xt += tmp;
    }

#ifdef _DEBUG
    cout << "\n实际数据:";
    for(int i=0; i<DataNum; i++) {
        cout << DataHandled[i] << " ";
    }
    cout << endl;
    cout << "指数平滑:";
    for(int i=0;i<s2_2_new.size();i++) {
        cout << s2_2_new[i] <<" ";
    }
    cout << "\npredict num = " << Xt << "  均方误差：" << MSE  << "\n\n"<< endl;
#endif
    delete[] DataHandled;
    if(Xt > 0) return (int)ceil(Xt);
    else return 0;
}


//三次指数平滑预测
int ExponentialSmooth3(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;
    //数据预处理
    int avg = 0;
    for (int i=0; i<data.size(); i++) {
        avg += data[i];
    }
    avg = (int)round((double)avg/n);

    int *DataHandled = new int[DataNum];
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            if(0 != avg && data[tempN] > 5*avg) DataHandled[i] += avg;
            else DataHandled[i] += data[tempN];
        }
    }
    vector<double> s3_1_new, s3_2_new, s3_3_new;
    double x = 0;
    for (int i = 0; i < 3; i++)
        x += DataHandled[i];
    x /= 3;
    double s3_1 = x, s3_2 = x, s3_3 = x;
    double a = 0.4;      //平滑系数!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    double MSE = 0, tempDouble;

    //先计算一次指数平滑的值
    for(int i=0; i<DataNum; i++) {
        if(0 == i) {
            s3_1_new.push_back(a*DataHandled[i] + (1-a)*s3_1);
        } else {
            s3_1_new.push_back(a*DataHandled[i] + (1-a)*s3_1_new[i-1]);
        }
    }

    //再计算二次指数平滑的值
    for(int i=0; i<DataNum; i++) {
        if(0 == i) {
            s3_2_new.push_back(a*s3_1_new[i] + (1-a)*s3_2);
        } else {
            s3_2_new.push_back(a*s3_1_new[i] + (1-a)*s3_2_new[i-1]);
        }
    }

    //后计算三次指数平滑值
    for(int i=0; i<DataNum; i++){
        if(0 == i){
            s3_3_new.push_back(a*s3_2_new[i] + (1-a)*s3_3);
        } else {
            s3_3_new.push_back(a*s3_2_new[i] + (1-a)*s3_3_new[i-1]);
        }
        tempDouble = s3_3_new[i] - DataHandled[i];
        MSE += tempDouble*tempDouble;
    }
    MSE = sqrt(MSE) / DataNum;
    int forecase = 1;
    s3_1 = s3_1_new[s3_1_new.size()-1];
    s3_2 = s3_2_new[s3_2_new.size()-1];
    s3_3 = s3_3_new[s3_3_new.size()-1];
    double At = s3_1*3 - s3_2*3 + s3_3;
    double Bt = (a/(2*(1-a)*(1-a))) * ((6-5*a)*s3_1 - (10-8*a)*s3_2 + (4-3*a)*s3_3);
    double Ct = ((a*a)/(2*(1-a)*(1-a))) * (s3_1 - 2*s3_2 + s3_3);
    double Xt = At + Bt*forecase + Ct*forecase*forecase;

#ifdef _DEBUG
//    printf("未来%d期的二次指数平滑预估值为： %lf, 均方误差为： %lf\n",forecase, Xt, MSE);
    cout << "\n实际数据:";
    for(int i=0; i<DataNum; i++) {
        cout << DataHandled[i] << " ";
    }
    cout << endl;
    cout << "指数平滑:";
    for(int i=0;i<s3_3_new.size();i++) {
        cout << s3_3_new[i] <<" ";
    }
    cout << "\npredict num = " << Xt << "  均方误差：" << MSE  << "\n\n"<< endl;
#endif
    delete[] DataHandled;
    if(Xt > 0) return (Xt - (int)Xt) > 0.3 ? (int)ceil(Xt) : (int)Xt;     //ceil不如round好使
    else return 0;
}
#endif //ECS_EXPONENTIALSMOOTH_H
