#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <sstream>
#include <cstring>
#include <numeric>
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

extern Server* server;//物理服务器
extern vector<Flavor*> vFlavor;//待检测虚拟机格式数组
extern vector<Server*> vServer;//待检测虚拟机格式数组
extern Date* beginDate;//预测开始日期
extern Date* nowDate;//预测开始日期
//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    // 需要输出的内容
    initDataStruct(info,data,data_num);//构建数据结构

    /********network******************/


    int days=*nowDate-*beginDate;//
    int predictDays=Date(predictBeginDate)-Date(predictEndDate);//
    for(int i=0;i<flavorNum;i++)
    {
        vFlavor[i]->_predictNum=ExponentialSmooth2(vFlavor[i]->dayLine,days,predictDays);
    }

    string outstr=firstFit();

    //string str="7\nflavor1 1\nflavor2 2\nflavor3 2\nflavor4 0\nflavor5 2\n1\n1 flavor1 1 flavor2 2 flavor3 2 flavor5 2";
    write_result(outstr.c_str(), filename);


    /***************************/

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


void initDataStruct(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM],int data_num)
{
    //物理服务器参数
    int s1,s2,s3;
    stringstream ssstream;
    ssstream<<info[0];
    //依次输出到result中，并存入res中
    ssstream>>s1>>s2>>s3;
    ssstream.clear();
    server =new Server(s1,s2,s3);

    //读取虚拟机个数
    ssstream<<info[2];
    ssstream>>flavorNum;
    ssstream.clear();

    //虚拟机格式
    for(int i=0;i<flavorNum;i++)
    {
        string flavorName;
        int cpuNum,memSize;
        ssstream<<info[i+3];
        ssstream>>flavorName>>cpuNum>>memSize;
        ssstream.clear();
        int id=atoi(flavorName.substr(6).c_str());
        Flavor* flavor=new Flavor(id,cpuNum,memSize);
        vFlavor.push_back(flavor);
    }

    //预测标志（cpu/mem）
    ssstream<<info[4+flavorNum];
    ssstream>>predictFlag;
    ssstream.clear();
    //预测开始时间
    ssstream<<info[6+flavorNum];
    ssstream>>predictBeginDate>>predictBeginTime;
    ssstream.clear();
    //预测结束时间
    ssstream<<info[7+flavorNum];
    ssstream>>predictEndDate>>predictEndTime;
    ssstream.clear();



    /*********读取训练数据**********************/
    //先拿出第一天
    string recordId1,flavorId1,date1,time1;
    ssstream<<data[0];
    ssstream>>recordId1>>flavorId1>>date1>>time1;
    ssstream.clear();
    beginDate=new Date(date1);

    int dayCount;
    for(int i=0;i<data_num;i++)//读取数据
    {
        string recordId,flavorName,date,time;//只考虑年月日
        ssstream<<data[i];
        ssstream>>recordId>>flavorName>>date>>time;
        ssstream.clear();
        int flavorId=atoi(flavorName.substr(6).c_str());
        if(flavorId>15)//如果不是需要预测的东西
        {
            //cout<<"*****************************"<<endl;
            continue;
        }
        else{
            for(int i=0;i<flavorNum;i++)
            {
                if(flavorId==vFlavor[i]->_id)
                {
                    nowDate=new Date(date);
                    dayCount=(*nowDate-*beginDate);//算出是哪一天的
                    vFlavor[i]->dayLine[dayCount]++;//这一天的这个虚拟机的个数+1
                }
            }
        }
    }

//////////////////////////////////////////////////////////
#ifdef _DEBUG
    cout<<"server: "<<*server;
    cout<<"flavorNum: "<<flavorNum<<endl;
    cout<<"flavor: "<<endl;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        cout<<vFlavor[i]->_id<<" "<<vFlavor[i]->_cpuNum<<" "<<vFlavor[i]->_memSize<<endl;
    }

    cout<<"predictFlag= "<<predictFlag<<endl;
    cout<<"predict_begin= "<<predictBeginDate<<endl;
    cout<<"predict_end= "<<predictEndDate<<endl;

    cout<<*beginDate<<" ~ "<<*nowDate<<endl;
    for(int i=0;i<flavorNum;i++)
    {
        cout<<*vFlavor[i];
    }

   // paint();

#endif
}


//data格式：    int data[] = {0, 2, 1, 1, 0, 6, 2, 1 ,0, 2, 1, 4, 3, 10, 2, 3, 1, 3, 3, 2, 5, 10, 1, 2, 8, 1, 2, 4, 2, 3};
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
//    for(int i=0;i<s2_2_new.size();i++)
//    {
//        cout<<s2_1_new[i]<<" ";
//    }
    cout<<" predict num = "<<int(Xt)<<endl;
#endif
    return (int)Xt;//4->5
}

