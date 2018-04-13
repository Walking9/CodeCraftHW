//
// Created by brunon on 18-4-8.
//
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "ExponentialSmooth.h"
#include "PolynomialCurveFitting.h"

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
//s2_2_new 是平滑后的数据
int ExponentialSmooth2(const vector<int> data, int n, int k, vector<double>& s2_2_new) {
    int DataNum = n / k, tempN = n;
    //数据预处理
//    int avg = 0;
//    for (unsigned int i=0; i<data.size(); i++) {
//        avg += data[i];
//    }
//    avg = (int)round((double)avg/n);

    int *DataHandled = new int[DataNum];
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            //if(0 != avg && data[tempN] > 5*avg) DataHandled[i] += avg;
            DataHandled[i] += data[tempN];
        }
    }
    vector<double> s2_1_new;
    double x=0;
    for(int i=0; i<3; i++)
        x += DataHandled[i];
    x /= 3;
    double s2_1 = x, s2_2 = x;   //置s2_1  s2_2初始值
    double a = 0.615;      //平滑系数0.615
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
//    for(int i=1; i<=forecase; i++) {
    tmp = At + Bt;
    tmp = tmp > 0 ? tmp : 0;
    Xt += tmp;
//    }

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
    if(Xt > 0) return (int)ceil(Xt+0.2);    //得分情况：+0.2 > ceil > round > floor, 最高分75.598
    else return 0;
//    if(Xt > 0) return (Xt - (int)Xt) > 0.4 ? (int)ceil(Xt) : (int)Xt;     //ceil不如round好使,这样的方法掉分。。
//    else return 0;
//    if(Xt > 0) return (int)round(Xt);
//    else return 0;
}

//三次指数平滑预测
int ExponentialSmooth3(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;
    //数据预处理............................没有好的方法暂时不对数据预处理了
    int avg = 0;
    for (unsigned int i=0; i<data.size(); i++) {
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
    double s3_1 = x, s3_2 = x, s3_3 = x;  //初始化s值
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
    cout << "\npredict num = " << Xt << " 均方误差：" << MSE  << "\n\n"<< endl;
#endif
    delete[] DataHandled;
    if(Xt > 0) return (Xt - (int)Xt) >= 0.45 ? (int)ceil(Xt) : (int)Xt;     //0.35~0.45 > round > ceil, 最高分76.182(不对数据处理76.173)
    else return 0;
}

//Tukey‘s test方法对数据异常值处理
void DataProcessing(const vector<int> input, int n, int Domain, vector<int>& output) {
    int avg = 0;
    for (int i=0; i<n; i++) {
        avg += input[i];
    }
    avg = (int)round((double)avg/n);

    vector<int> data;
    for(int i=0; i<n; i++) data.push_back(input[i]);
    sort(data.begin(), data.end());
    cout << "\nsort: ";
    for(unsigned int i=0; i<data.size(); i++) {
        cout << data[i] << " ";
    }
    int Q1 = data[data.size()/4];
    //int Q2 = data[data.size()/2];
    int Q3 = data[data.size()*3/4];
    int maxValuation = Q3 + Domain*(Q3-Q1);
    if(maxValuation == 0) maxValuation = 1;
    for(int i=0; i<n; i++) {
        if(input[i] > maxValuation && i>0) output.push_back((int)round(0.4*input[i]));   //用avg81.507   //*0.4 83.87!!
        else output.push_back(input[i]);
    }
}

//三次指数平滑预测改进
int ExponentialSmooth3fix(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;
    int avg = 0;
    for (int i=0; i<n; i++) {
        avg += data[i];
    }
    avg = (int)round((double)avg/n);

    int *DataHandled = new int[DataNum];
    vector<int> output;
//    DataProcessing(data, n, 5, output);
//    ThreeTimeFittingDataProcessing(data, n,  1, 11, output);
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            if(0 != avg && data[tempN] > 5*avg) DataHandled[i] += avg;
            else DataHandled[i] += data[tempN];
        }
    }
    vector<double> s3_1_new, s3_2_new, s3_3_new;
    double x = DataHandled[0];
    double s3_1 = x, s3_2 = x, s3_3 = x;//初始化s值
    double a_fit = 0.4;      //平滑系数!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    double Differces = INTMAX_MAX;
    double Xt;

    for(double a=0.4; a<=0.72; a+=0.0001) {     //0.4 ~ 0.72
        for (int i = 0; i < DataNum-1; i++) {
            if (0 == i) {
                s3_1_new.push_back(a * DataHandled[i] + (1 - a) * s3_1);
                s3_2_new.push_back(a * s3_1_new[i] + (1 - a) * s3_2);
                s3_3_new.push_back(a * s3_2_new[i] + (1 - a) * s3_3);
            } else {
                s3_1_new.push_back(a * DataHandled[i] + (1 - a) * s3_1_new[i - 1]);
                s3_2_new.push_back(a * s3_1_new[i] + (1 - a) * s3_2_new[i - 1]);
                s3_3_new.push_back(a * s3_2_new[i] + (1 - a) * s3_3_new[i - 1]);
            }
        }

        s3_1 = s3_1_new[s3_1_new.size() - 1];
        s3_2 = s3_2_new[s3_2_new.size() - 1];
        s3_3 = s3_3_new[s3_3_new.size() - 1];
        double At = s3_1 * 3 - s3_2 * 3 + s3_3;
        double Bt = (a / (2 * (1 - a) * (1 - a))) * ((6 - 5 * a) * s3_1 - (10 - 8 * a) * s3_2 + (4 - 3 * a) * s3_3);
        double Ct = ((a * a) / (2 * (1 - a) * (1 - a))) * (s3_1 - 2 * s3_2 + s3_3);
        Xt = At + Bt + Ct;

        if(abs(Xt-DataHandled[DataNum-1]) < Differces) {
            Differces = abs(Xt-DataHandled[DataNum-1]);
            a_fit = a;
        }
        s3_1_new.clear();
        s3_2_new.clear();
        s3_3_new.clear();
    }

    for (int i = 0; i < DataNum; i++) {
        if (0 == i) {
            s3_1_new.push_back(a_fit * DataHandled[i] + (1 - a_fit) * s3_1);
            s3_2_new.push_back(a_fit * s3_1_new[i] + (1 - a_fit) * s3_2);
            s3_3_new.push_back(a_fit * s3_2_new[i] + (1 - a_fit) * s3_3);
        } else {
            s3_1_new.push_back(a_fit * DataHandled[i] + (1 - a_fit) * s3_1_new[i - 1]);
            s3_2_new.push_back(a_fit * s3_1_new[i] + (1 - a_fit) * s3_2_new[i - 1]);
            s3_3_new.push_back(a_fit * s3_2_new[i] + (1 - a_fit) * s3_3_new[i - 1]);
        }
    }
    s3_1 = s3_1_new[s3_1_new.size() - 1];
    s3_2 = s3_2_new[s3_2_new.size() - 1];
    s3_3 = s3_3_new[s3_3_new.size() - 1];
    double At = s3_1 * 3 - s3_2 * 3 + s3_3;
    double Bt = (a_fit / (2 * (1 - a_fit) * (1 - a_fit))) * ((6 - 5 * a_fit) * s3_1 - (10 - 8 * a_fit) * s3_2 + (4 - 3 * a_fit) * s3_3);
    double Ct = ((a_fit * a_fit) / (2 * (1 - a_fit * (1 - a_fit))) * (s3_1 - 2 * s3_2 + s3_3));
    Xt = At + Bt + Ct;

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
    cout << "\npredict num = " << Xt << " a_fit："  << a_fit << "\n\n"<< endl;
#endif
    delete[] DataHandled;
    if(Xt > 0) return (int)round(Xt+0.1);
    else return 0;
}

//二次指数平滑另一种公式
int ExponentialSmooth22(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;

    int *DataHandled = new int[DataNum];
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            DataHandled[i] += data[tempN];
        }
    }
    vector<double> s, t;
    double x=0;
    for(int i=0; i<3; i++)
        x += DataHandled[i];
    x /= 3;
    s.push_back(x); t.push_back(x);   //置s0 t0初始值
    double a = 0.72;      //平滑系数0.62  0.78
    double b = 0.1;

    for(int i=0; i<DataNum; i++) {
        s.push_back(a*DataHandled[i] + (1-a)*(s[i] + t[i]));
        t.push_back(b*(s[i+1] - s[i]) + (1-b)*t[i]);
    }

    double Xt = s[s.size()-1] + t[t.size()-1];   //预测后一期
//
#ifdef _DEBUG
    cout << "\n实际数据:";
    for(int i=0; i<DataNum; i++) {
        cout << DataHandled[i] << " ";
    }
    cout << "\npredict num = " << Xt  << "\n\n"<< endl;
#endif
    delete[] DataHandled;
    if(Xt > 0) return (int)ceil(Xt);
    else return 0;
}

int ExponentialSmooth22fix(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;

    int *DataHandled = new int[DataNum];
    vector<int> output;
    DataProcessing(data, n, 4, output);
    //ThreeTimeFittingDataProcessing(data, n, 1, output);
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            DataHandled[i] += output[tempN];
        }
    }
    vector<double> s, t;
    double x=0;
    for(int i=0; i<3; i++)
        x += DataHandled[i];
    x /= 3;
    double a_fix = 0.62, b_fix = 0.76;      //平滑系数0.615

    //网格法求最适系数
    double Xt;
    double Differces = INTMAX_MAX;
    for(double a=0.77; a<=0.85; a+=0.001) {
        for(double b=0.13; b<=0.5; b+=0.001) {
            s.push_back(x); t.push_back(x);   //置s0 t0初始值
            for(int i=0; i<DataNum-1; i++) {
                s.push_back(a*DataHandled[i] + (1-a)*(s[i] + t[i]));
                t.push_back(b*(s[i+1] - s[i]) + (1-b)*t[i]);
            }
            Xt = s[s.size()-1] + t[t.size()-1];
            if(abs(Xt-DataHandled[DataNum-1]) < Differces) {
                Differces = abs(Xt-DataHandled[DataNum-1]);
                a_fix = a;
                b_fix = b;
            }
            s.clear();
            t.clear();
        }
    }
    //a_fix = 0.62; b_fix = 0.76;
    s.push_back(x); t.push_back(x);   //置s0 t0初始值
    for(int i=0; i<DataNum; i++) {
        s.push_back(a_fix*DataHandled[i] + (1-a_fix)*(s[i] + t[i]));
        t.push_back(b_fix*(s[i+1] - s[i]) + (1-b_fix)*t[i]);
    }
    Xt = s[s.size()-1] + t[t.size()-1];

#ifdef _DEBUG
    cout << "\n实际数据:";
    for(int i=0; i<DataNum; i++) {
        cout << DataHandled[i] << " ";
    }
    cout << "\npredict num = " << Xt << endl;
    cout << "a=" << a_fix << ", b=" << b_fix << "\n\n" << endl;
#endif
    delete[] DataHandled;
    if(Xt > 0) return (int)ceil(Xt);
    else return 0;
}
