#include"code.h"
#include<iostream>
#include<string>
#include<vector>
#include <lib_io.h>

#ifdef _DEBUG
#include <python2.7/Python.h>
#endif

#include <algorithm>
using namespace std;

int flavorNum;//虚拟机规格个数
string predictBeginDate;//预测开始时间
string predictEndDate;//预测结束时间
string predictBeginTime;//预测开始时间
string predictEndTime;//预测结束时间

string predictFlag;

Server* server;//物理服务器
vector<Flavor*> vFlavor;//待检测虚拟机格式数组
vector<Server*> vServer;
Date* beginDate;
Date* nowDate;

ostream & operator<<(ostream &out,const Server &server)
{
    out<<server._cpuNum<<" "<<server._memSize<<" "<<server._diskSize<<endl;
    return out;
}

ostream & operator<<(ostream &out, Flavor &flavor)
{
    int days=*nowDate-*beginDate;
    out<<flavor._id<<" "<<flavor._cpuNum<<" "<<flavor._memSize<<" ("<<days<<") -> ";
    for(int i=0;i<days;i++)
    {
        out<<flavor.dayLine[i]<<",";
    }
    out<<endl;
    return out;
}

void paint()
{
#ifdef _DEBUG
    int days=*nowDate-*beginDate;
    string str="";
    for(int i=0;i<flavorNum;i++)
    {
        str+=to_string(vFlavor[i]->_id)+" "+to_string(days)+" ";//to_string(vFlavor[i]->_cpuNum)+" "+to_string(vFlavor[i]->_memSize)+"("+to_string(days)+") ";
        for(int j=0;j<days;j++)
        {
            str+=to_string(vFlavor[i]->dayLine[j])+" ";
        }
        str+='\n';
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
   // PyRun_SimpleString("sys.argv['arg1','arg2']");

    //第四步：执行调用脚本文件命令,注意文件的路径
    if (PyRun_SimpleString("execfile('../graph.py')") == NULL)
    {
        return;
    }

    //第五步：关闭Python解释器
    Py_Finalize();
    return;
#endif
}



bool cmp(Flavor* a,Flavor* b)
{
    return a->_cpuNum<b->_cpuNum;
}

//void coutt(int n)
//{
//    for(int i=0;i<10;i++)
//    {
//        cout<<n;
//    }
//    cout<<endl;
//
//}

string firstFit()
{
    sort(vFlavor.begin(),vFlavor.end(),cmp);//先排序

#ifdef _DEBUG
    cout<<"all flavor size = ";
    cout<<endl;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        cout<<" id: "<<vFlavor[i]->_id<<" = "<<vFlavor[i]->_predictNum<<endl;
    }
    cout<<endl;
#endif

    int allNum=0;
    for(int i=0;i<flavorNum;i++)
    {
        allNum+=vFlavor[i]->_predictNum;
    }
    string str="";
    str+=to_string(allNum)+"\n";
    for(int i =0;i<flavorNum;i++)
    {
        str+="flavor"+to_string(vFlavor[i]->_id)+" "+to_string(vFlavor[i]->_predictNum)+"\n";
    }
    str+="\n";


    vServer.push_back(new Server(server->_cpuNum,server->_memSize,server->_diskSize));
    for(size_t i=0;i<vFlavor.size();i++)//循环所有的虚拟机
    {
        Flavor* flavor=vFlavor[i];
        while(flavor->_predictNum)//检查此虚拟机预测的个数
        {
            for(size_t j=0;j<vServer.size();j++)//循环物理服务器链表
            {
                Server* server0=vServer[j];
                if(flavor->_predictNum==0)   continue;//如果这个虚拟机没有了

                if(flavor->_cpuNum>server0->_cpuNum||flavor->_memSize>server0->_memSize)//如果这个物理服务器不能装下这个虚拟机了
                {
                    if(j==vServer.size()-1)//且如果这个服务器是最后一个服务器了
                    {
                        vServer.push_back(new Server(server->_cpuNum,server->_memSize,server->_diskSize));//再申请一个新的物理机
                    }
                    //continue;//如果此服务器不能装下这个虚拟机 就向后推动
                }
                else//如果装下这个虚拟机
                {
                    server0->_cpuNum-=flavor->_cpuNum;//物理服务器cpu资源消耗
                    server0->_memSize-=flavor->_memSize;//物理服务器mem资源消耗
                    server0->_flavorNum[flavor->_id]++;//物理服务器装的某个虚拟机个数+1
                    flavor->_predictNum--;//虚拟机个数-1
                }
            }
        }
    }

    str+=to_string(vServer.size());
    for(size_t i=0;i<vServer.size();i++)
    {
        str+="\n"+to_string(i+1)+" ";
        for(size_t j=0;j<vServer[i]->_flavorNum.size();j++)
        {
            if(vServer[i]->_flavorNum[j]!=0)
                str+="flavor"+to_string(j)+" "+to_string(vServer[i]->_flavorNum[j])+" ";
        }
    }


#ifdef _DEBUG
    cout<<"create server = "<<vServer.size()<<endl;
    for(size_t i=0;i<vServer.size();i++)
    {
        cout<<"server"<<i<<" ("<<vServer[i]->_cpuNum<<") ";
        for(size_t j=0;j<vServer[i]->_flavorNum.size();j++)
        {
            if(vServer[i]->_flavorNum[j]!=0)
                cout<<"flavor"<<j<<" "<<vServer[i]->_flavorNum[j]<<" ";
        }
        cout<<endl;
    }
    cout<<str;
#endif

    return str;
}



