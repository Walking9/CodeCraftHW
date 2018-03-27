#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include "code.h"
#include "date.h"
#include "predict.h"

using namespace std;

extern int flavorNum;//虚拟机规格个数
extern string predictBeginDate;//预测开始日期
extern string predictBeginTime;//预测开始时间
extern string predictEndDate;//预测结束日期
extern string predictEndTime;//预测结束时间

extern string predictFlag;//预测目标

extern Server* server;//物理服务器标量
extern vector<Flavor*> vFlavor;//待检测虚拟机格式数组
extern vector<Server*> vServer;//构建物理服务器链表
extern Date* beginDate;//预测开始日期
extern Date* endDate;//预测开始日期
//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    // 需要输出的内容
    initDataStruct(info,data,data_num);//构建数据结构

    /********训练数据与打印输出使用******************/
    int days=*endDate-*beginDate;//

    int predictDays=Date(predictBeginDate)-Date(predictEndDate);//
    for(int i=0;i<flavorNum;i++)
    {   
        //训练每个虚拟机的数据集
        cout<<"训练虚拟机"<<vFlavor[i]->_id<<" ";
        vFlavor[i]->_predictNum=predict(vFlavor[i]->_dayLine,days,predictDays);
    }

    /**********装箱*************/

//    string outstr=firstFit();
    string outstr=dpPath();
//
    /*********输出**************/
    write_result(outstr.c_str(), filename);

    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，
    // 如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
    for(size_t i=0;i<vServer.size();i++)
    {
        delete vServer[i];
    }
    for(size_t i=0;i<vFlavor.size();i++)
    {
        delete vFlavor[i];
    }
}

int predict(const vector<int> data, int n,int predictDays)
{
    int ret ;

    ret=ExponentialSmooth2(data,n,predictDays);//指数平滑算法

    //ret=RnnPredict(data,n,predictDays);

    return ret;
}


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
int ExponentialSmooth2(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;
    int *DataHandled = new int[DataNum];
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            DataHandled[i] += data[--tempN];
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
    int forecase = 1;       //要预测几期，暂时置1!!!!!!!!!!!!!!
    double At = s2_1_new[s2_1_new.size()-1] * 2 - s2_2_new[s2_2_new.size()-1];
    double Bt = (a / (1-a)) * (s2_1_new[s2_1_new.size()-1] - s2_2_new[s2_2_new.size()-1] );
    double Xt = At + Bt * forecase;

    delete[] DataHandled;
    
#ifdef _DEBUG
    printf("***************************************************\n");
    printf("未来%d期的二次指数平滑预估值为： %lf, 均方误差为： %lf\n",forecase, Xt, MSE);
    for(int i=0;i<s2_2_new.size();i++)
    {
        cout<<s2_1_new[i]<<" ";
    }
    cout<<" predict num = "<<int(Xt)<<endl;
#endif
    return (int)round(Xt);//4->5
}


//三次指数平滑预测
int ExponentialSmooth3(const vector<int> data, int n, int k) {
    int DataNum = n / k, tempN = n;
    int *DataHandled = new int[DataNum];
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            DataHandled[i] += data[--tempN];
        }
    }
    vector<double> s3_1_new, s3_2_new, s3_3_new;
    double x = 0;
    for (int i = 0; i < 3; i++)
        x += DataHandled[i];
    x /= 3;
    double s3_1 = x, s3_2 = x, s3_3 = x;    double a = 0.6;      //平滑系数
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
    cout << "\n\n实际数据:";
    for(int i=0; i<DataNum; i++) {
        cout << DataHandled[i] << " ";
    }
    cout << endl;
    cout << "指数平滑:";
    for(int i=0;i<s3_3_new.size();i++) {
        cout << s3_3_new[i] <<" ";
    }
    cout << "predict num = " << Xt << "  均方误差：" << MSE << endl;
#endif
    delete[] DataHandled;

    return (int)round(Xt);
}

