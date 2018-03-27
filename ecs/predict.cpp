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
extern Date* nowDate;//预测开始日期
//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    // 需要输出的内容
    initDataStruct(info,data,data_num);//构建数据结构

    /********训练数据与打印输出使用******************/
    int days=*nowDate-*beginDate;//

    int predictDays=Date(predictBeginDate)-Date(predictEndDate);//
    for(int i=0;i<flavorNum;i++)
    {   
        //训练每个虚拟机的数据集
        vFlavor[i]->_predictNum=predict(vFlavor[i]->_dayLine,days,predictDays);
    }

    /**********装箱*************/

    string outstr=dpPath();

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
    double a = 0.6;      //平滑系数
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
    printf("未来%d期的二次指数平滑预估值为： %lf, 均方误差为： %lf\n",forecase, Xt, MSE);
    for(int i=0;i<s2_2_new.size();i++)
    {
        cout<<s2_1_new[i]<<" ";
    }
    cout<<" predict num = "<<int(Xt)<<endl;
#endif
    return (int)Xt;//4->5
}

