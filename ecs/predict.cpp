#include <stdio.h>
#include <iostream>
#include "ExponentialSmooth.h"
#include "code.h"
#include "date.h"
#include "predict.h"
#include <sstream>

using namespace std;

extern int flavorNum;//虚拟机规格个数
extern Date* predictBeginDate;//预测开始日期
extern Date* predictBeginTime;//预测开始时间
extern Date* predictEndDate;//预测结束日期
extern Date* predictEndTime;//预测结束时间

extern string predictFlag;//预测目标

extern Server* server;//物理服务器标量
extern vector<Flavor*> vFlavor;//待检测虚拟机格式数组
extern vector<Server*> vServer;//构建物理服务器链表
extern Date* beginDate;//训练开始日期
extern Date* endDate;//训练结束日期
//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    //需要输出的内容
    initDataStruct(info, data, data_num);//构建数据结构

    /********训练数据与打印输出使用******************/
    int days = *endDate-*beginDate;//
    int spaceDays=*predictBeginDate-*endDate-1;
    int predictDays = *predictBeginDate - *predictEndDate;//

    for(int i=0; i<flavorNum; i++)
    {
#ifdef _DEBUG
        //训练每个虚拟机的数据集
        cout << "训练虚拟机" << vFlavor[i]->_id <<" ";
#endif
        vFlavor[i]->_predictNum = predict(vFlavor[i]->_dayLine,days,spaceDays,predictDays);
    }

    /**********装箱*************/
//    string outstr;
//    stringstream ss;
//
//    string outstr1 = dpPath();
//    int serverN1;
//    ss<<outstr1;
//    ss>>serverN1;
//    ss.clear();

    string outstr = firstFit();
//    int serverN0;
//    ss<<outstr0;
//    ss>>serverN0;
//    ss.clear();


//  outstr=serverN0<serverN1?outstr0:outstr1;


//    if(serverN0<=serverN1)
//    {
//        outstr=outstr0;
//        cout<<"++++++++++++++++++++++++++++++++ use first fit"<<endl;
//    }
//    else
//    {
//        outstr=outstr1;
//        cout<<"++++++++++++++++++++++++++++++++ use dp2 path"<<endl;
//    }
//

    /*********输出**************/
    write_result(outstr.c_str(), filename);

    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，
    // 如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
    for(size_t i=0; i<vServer.size(); i++)
    {
        delete vServer[i];
    }
    for(size_t i=0; i<vFlavor.size(); i++)
    {
        delete vFlavor[i];
    }
}

int predict(const vector<int> data, int n,int spaceDays,int predictDays)
{
    int ret ;

    ret = ExponentialSmooth3(data,n, predictDays);//指数平滑算法

    //ret=RnnPredict(data,n,predictDays);

    return ret;
}


