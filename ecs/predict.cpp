#include "predict.h"
#include <stdio.h>
#include <iostream>
#include "ExponentialSmooth.h"
#include "fit.h"
#include "date.h"
#include "init.h"
#include "PolynomialCurveFitting.h"
#include <sstream>
#include <numeric>

using namespace std;

extern Date* predictBeginDate;//预测开始日期
extern Date* predictBeginTime;//预测开始时间
extern Date* predictEndDate;//预测结束日期
extern Date* predictEndTime;//预测结束时间

extern string predictFlag;//预测目标

extern vector<Flavor*> vFlavor;//待检测虚拟机格式数组
extern vector<Server*> vServer;//构建物理服务器链表
extern Date* beginDate;//训练开始日期
extern Date* endDate;//训练结束日期
extern int trainDays;
extern int spaceDays;
extern int predictDays;

//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    //需要输出的内容
    initDataStruct(info, data, data_num);//构建数据结构

    /********训练数据与打印输出使用******************/

    /*************/
    //int v[5]={3,3,3,3,3};
    for(size_t i=0; i<vFlavor.size(); i++)
    {
        vFlavor[i]->_predictNum= predict(vFlavor[i]->_dayLine,trainDays,spaceDays,predictDays);

/******************************************/

//        int a=0,b=0;
//        if(predictDays<trainDays)
//            a=trainDays-predictDays;
//        vFlavor[i]->_predictNum=accumulate(vFlavor[i]->_dayLine.begin()+a,vFlavor[i]->_dayLine.begin()+trainDays,0);
//
//        vector<int> output;
//        DataProcessing(vFlavor[i]->_dayLine, trainDays,2,output);
//        cout << "训练()()()" << vFlavor[i]->_id <<" = "<< accumulate(output.begin()+a,output.begin()+trainDays,0)<<endl;

/******************************************/
//        vector<int> output;
//        DataProcessing(vFlavor[i]->_dayLine, trainDays,2,output);
        //cout << "训练()()()" << vFlavor[i]->_id <<" = "<< (accumulate(output.begin(),output.begin()+trainDays,0)/static_cast<double>(trainDays))*predictDays<<endl;
    }

#ifdef _DEBUG

    for(int i=0;i<vFlavor.size();i++)
    {
        //训练每个虚拟机的数据集
        cout << "训练虚拟机" << vFlavor[i]->_id <<" = "<< vFlavor[i]->_predictNum<<endl;
    }
#endif

    /**********装箱****************************/
    string outstr=dpPath();

    /*********输出*****************************/
    write_result(outstr.c_str(), filename);

    //printTest("../data/contest1/TestData_2016.1.8_2016.1.14.txt");

    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，
    // 如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
    for(size_t i=0; i<vServer.size(); i++) {
        delete vServer[i];
    }
    for(size_t i=0; i<vFlavor.size(); i++) {
        delete vFlavor[i];
    }

}


int predict(const vector<int> data, int n,int spaceDays,int predictDays)
{
//    vector<int> output;
//
//    DataProcessing(data, n, 3,output);
//
//    return test_fun(data,n,spaceDays,predictDays);

//    if(spaceDays==0)
//    {
        int retLine, retSpace = 0;// retEx2, retEx3 ;
        vector<double> v;
        retLine = ExponentialSmooth22fix(data, n, predictDays+spaceDays); //指数平滑算法
        if(0 != spaceDays)
            retSpace = ExponentialSmooth22fix(data, n, spaceDays);
        return abs(retLine - retSpace);
//    }
//    else
//    {
//        int num=ceil((accumulate(data.begin(),data.begin()+trainDays,0)/static_cast<double>(trainDays))*predictDays);
//        return num;
//    }
}


