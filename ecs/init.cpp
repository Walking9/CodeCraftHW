//
// Created by hengliy on 4/17/18.
//


#ifdef _DEBUG
#include <python2.7/Python.h>
#endif
#include "init.h"
#include "fit.h"
#include "date.h"
#include <sstream>
using namespace std;

Date* trainBeginDate;//训练数据开始时间
Date* trainEndDate;//训练数据结束时间
Date* predictBeginDate;//预测开始时间
Date* predictEndDate;//预测结束时间

time_t* trainBeginTime;//训练数据开始时间
time_t* trainEndTime;//训练数据结束时间
time_t* predictBeginTime;//预测开始时间
time_t* predictEndTime;//预测结束时间

string predictFlag;//预测标志 cpu/mem
int spaceDays;//间隔的天数
int trainDays;
int predictDays;

int predicetHours;

vector<Flavor*> vFlavor;//待检测虚拟机格式数组
vector<Server*> vServer;//构建预测的物理服务器链表



ostream & operator<<(ostream &out,const Server &server)
{
    out<<server._name<<" "<<server._cpuNum<<" "<<server._memSize<<" "<<server._diskSize<<endl;
    return out;
}

ostream & operator<<(ostream &out, Flavor &flavor)
{
    int days=*trainEndDate-*trainBeginDate+1;
    out<<flavor._id<<" "<<flavor._cpuNum<<" "<<flavor._memSize<<" ("<<days<<") -> ";
    for(int i=0;i<days;i++)
    {
        out<<flavor._dayLine[i]<<",";
    }
    out<<endl;
    return out;
}

void Server::putFlavor(const Flavor* flavor)
{
    _cpuNum-=flavor->_cpuNum;
    _memSize-=flavor->_memSize;
    _flavorNum[flavor->_id]++;//物理服务器装的某个虚拟机个数+1
}


void initDataStruct(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM],int data_num)
{
    //物理服务器参数
    int serverNum;
    string serverName;
    int cpuNum,memSize,Disk;
    stringstream ssstream;
    ssstream<<info[0];
    //依次输出到result中，并存入res中
    ssstream>>serverNum;
    ssstream.clear();

    for(int i=0;i<serverNum;i++)
    {
        ssstream<<info[i+1];
        ssstream>>serverName>>cpuNum>>memSize>>Disk;
        ssstream.clear();
        vServer.push_back(new Server(i,serverName,cpuNum,memSize,Disk));
    }

    int flavorNum;//虚拟机规格个数
    //读取虚拟机个数
    ssstream<<info[serverNum+2];
    ssstream>>flavorNum;
    ssstream.clear();

    //虚拟机格式
    for(int i=0;i<flavorNum;i++)
    {
        string flavorName;
        int cpuNum,memSize;
        ssstream<<info[3+serverNum+i];
        ssstream>>flavorName>>cpuNum>>memSize;
        ssstream.clear();
        int id=atoi(flavorName.substr(6).c_str());
        Flavor* flavor=new Flavor(id,cpuNum,memSize/1024);
        vFlavor.push_back(flavor);
    }

    //预测标志（cpu/mem）
//    ssstream<<info[4+flavorNum+serverNum];
//    ssstream>>predictFlag;
//    ssstream.clear();
    //预测开始时间
    string preBeDaStr,preBeDaStrT;
    string preEndDaStr,preEndDaStrT;
    ssstream<<info[4+flavorNum+serverNum];
    ssstream>>preBeDaStr>>preBeDaStrT;
    predictBeginDate=new Date(preBeDaStr);
    ssstream.clear();
    //预测结束时间
    ssstream<<info[5+flavorNum+serverNum];
    ssstream>>preEndDaStr>>preEndDaStrT;
    predictEndDate=new Date(preEndDaStr);
    ssstream.clear();

    /*********读取训练数据**********************/
    //先拿出第一天
    string recordId1,flavorId1,date1,time1;
    ssstream<<data[0];
    ssstream>>recordId1>>flavorId1>>date1>>time1;
    ssstream.clear();

    trainBeginTime=new time_t();
    *trainBeginTime = StringToDatetime(date1+" "+time1);
    trainBeginDate=new Date(date1);

    time_t preTime=*trainBeginTime;
    for(int i=0;i<data_num;i++)//读取数据
    {
        string recordId,flavorName,date,time;//只考虑年月日
        ssstream<<data[i];
        ssstream>>recordId>>flavorName>>date>>time;
        string nowTimeStr=date+" "+time;
        ssstream.clear();
        int flavorId=atoi(flavorName.substr(6).c_str());

        if(flavorId>MAX_FLAVORS)//如果不是需要预测的东西
        {
            continue;
        }
        else{
            for(int i=0;i<flavorNum;i++)
            {
                if(flavorId==vFlavor[i]->_id)
                {
                    Date nowDate(date);
                    int dayCount=(nowDate-*trainBeginDate);//算出是哪一天的
                    vFlavor[i]->_dayLine[dayCount]++;//这一天的这个虚拟机的个数+1

                    time_t nowTime=StringToDatetime(nowTimeStr);
                    int timeCount=(int)difftime(nowTime,preTime);
                    int hourCount=(int)difftime(nowTime,*trainBeginTime);
                    preTime=nowTime;
                    cout<<timeCount<<endl;
                    vFlavor[i]->_timeLine.push_back(timeCount);

                    vFlavor[i]->_hourLine[hourCount/3600]++;
                }
            }
        }
    }

    ssstream<<data[data_num-1];
    ssstream>>recordId1>>flavorId1>>date1>>time1;
    ssstream.clear();
    trainEndDate=new Date(date1);
    trainEndTime=new time_t();
    *trainEndTime =StringToDatetime(date1+" "+time1);
    predicetHours=((int)difftime(*trainEndTime,*trainBeginTime))/3600;

    trainDays = *trainEndDate-*trainBeginDate+1;//
    spaceDays = *predictBeginDate-*trainEndDate-1;
    predictDays = *predictBeginDate - *predictEndDate+1;//

//////////////////////////////////////////////////////////
#ifdef _DEBUG
    cout<<"/////////////////////////////"<<endl;
    cout<<"~~~~servers("<<serverNum<<")~~~~"<<endl;
    for(size_t i=0;i<vServer.size();i++)
    {
        cout<<*vServer[i];
    }

    cout<<endl;

    cout<<"~~~~flavors("<<flavorNum<<")~~~~"<<endl;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        cout<<*vFlavor[i];
    }

    cout<<"~~~~timeLine("<<flavorNum<<")~~~~"<<endl;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        Flavor* flavor=vFlavor[i];
        int days=*trainEndDate-*trainBeginDate+1;
        cout<<flavor->_id<<" "<<flavor->_cpuNum<<" "<<flavor->_memSize<<" ("<<flavor->_timeLine.size()<<") -> ";
        for(int j=0;j<flavor->_timeLine.size();j++)
            cout<<flavor->_timeLine[j]<<",";
        cout<<endl;
    }

    cout<<"~~~~hourLine("<<flavorNum<<")~~~~"<<endl;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        Flavor* flavor=vFlavor[i];
        int days=*trainEndDate-*trainBeginDate+1;
        cout<<flavor->_id<<" "<<flavor->_cpuNum<<" "<<flavor->_memSize<<" ("<<predicetHours<<") -> ";
        for(int j=0;j<predicetHours;j++)
            cout<<flavor->_hourLine[j]<<",";
        cout<<endl;
    }

    cout<<endl;

    cout<<"Train Date = ("<<trainDays<<")-> "<<*trainBeginDate<<" ~ "<<*trainEndDate<<endl;

    cout<<"spaceDays = ("<<spaceDays<<")"<<endl;

    cout<<"Predict Date = ("<<predictDays<<")-> "<<*predictBeginDate<<" ~ "<<*predictEndDate<<endl;

    cout<<endl;

    //paint(vFlavor[0]->_timeLine,vFlavor[0]->_timeLine.size(),vFlavor[0]->_timeLine,vFlavor[0]->_timeLine.size());
    //paint1("flavor ",vFlavor[0]->_hourLine,predicetHours);

#endif
}

void paint1(string name,vector<int> data1,int length1)
{
/*
#ifdef _DEBUG

    string str="";
    str+=name+"\n";
    for(int i=0;i<length1;i++)
    {
        str+=to_string(data1[i])+" ";
    }
    str+="\n";

    for(int i=0;i<length1;i++)
    {
        str+=to_string(data1[i])+" ";
    }

    write_result(str.c_str(), "../data/cpp_py.txt");

    //第一步：初始化Python
    //在调用Python的提供的给C的API之前，通过执行初始化
    //来添加Python的内建模块、__main__、sys等
    Py_Initialize();

    //检查初始化是否完成
    if (!Py_IsInitialized())
    {
        return;
    }

    //第二步：导入sys模块
    PyRun_SimpleString("import sys");

    //第三步：导入执行脚本时的命令行参数，如：./sample.py arg1 arg2
    //PyRun_SimpleString("sys.argv['arg1','arg2']");

    //第四步：执行调用脚本文件命令,注意文件的路径
    if (PyRun_SimpleString("execfile('../graph.py')") == NULL)
    {
        return;
    }

    //第五步：关闭Python解释器
    Py_Finalize();

    return;

#endif
*/
}

void paint2(vector<int> data1,int length1,vector<int>data2,int length2)
{
/*
#ifdef _DEBUG

    string str="";
    for(int i=0;i<length1;i++)
    {
        str+=to_string(data1[i])+" ";
    }
    str+="\n";

    for(int i=0;i<length2;i++)
    {
        str+=to_string(data2[i])+" ";
    }

    write_result(str.c_str(), "../data/cpp_py.txt");

    //第一步：初始化Python
    //在调用Python的提供的给C的API之前，通过执行初始化
    //来添加Python的内建模块、__main__、sys等
    Py_Initialize();

    //检查初始化是否完成
    if (!Py_IsInitialized())
    {
        return;
    }

    //第二步：导入sys模块
    PyRun_SimpleString("import sys");

    //第三步：导入执行脚本时的命令行参数，如：./sample.py arg1 arg2
    //PyRun_SimpleString("sys.argv['arg1','arg2']");

    //第四步：执行调用脚本文件命令,注意文件的路径
    if (PyRun_SimpleString("execfile('../graph.py')") == NULL)
    {
        return;
    }

    //第五步：关闭Python解释器
    Py_Finalize();

    return;

#endif
*/
}
