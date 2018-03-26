#include"code.h"
#include<iostream>
#include<string>
#include<vector>
#include <lib_io.h>

#ifdef _DEBUG
#include <python2.7/Python.h>
#endif

#include <algorithm>
#include <sstream>
#include <cstring>

using namespace std;

int flavorNum;//虚拟机规格个数
string predictBeginDate;//预测开始时间
string predictEndDate;//预测结束时间
string predictBeginTime;//预测开始时间
string predictEndTime;//预测结束时间

string predictFlag;//预测标志 cpu/mem

Server* server;//物理服务器
vector<Flavor*> vFlavor;//待检测虚拟机格式数组
vector<Server*> vServer;//构建预测的物理服务器链表
Date* beginDate;//训练数据开始时间
Date* nowDate;//训练数据结束时间

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
        out<<flavor._dayLine[i]<<",";
    }
    out<<endl;
    return out;
}

void paint()
{
#ifdef _DEBUG
//    int days=*nowDate-*beginDate;
//    string str="";
//    for(int i=0;i<flavorNum;i++)
//    {
//        str+=to_string(vFlavor[i]->_id)+" "+to_string(days)+" ";//to_string(vFlavor[i]->_cpuNum)+" "+to_string(vFlavor[i]->_memSize)+"("+to_string(days)+") ";
//        for(int j=0;j<days;j++)
//        {
//            str+=to_string(vFlavor[i]->_dayLine[j])+" ";
//        }
//        str+='\n';
//    }
//
//    write_result(str.c_str(), "../data/cpp_py.txt");
//
//    //第一步：初始化Python
//    //在调用Python的提供的给C的API之前，通过执行初始化
//    //来添加Python的内建模块、__main__、sys等
//    Py_Initialize();
//
//    //检查初始化是否完成
//    if (!Py_IsInitialized())
//    {
//        return;
//    }
//
//    //第二步：导入sys模块
//    PyRun_SimpleString("import sys");
//
//    //第三步：导入执行脚本时的命令行参数，如：./sample.py arg1 arg2
//   // PyRun_SimpleString("sys.argv['arg1','arg2']");
//
//    //第四步：执行调用脚本文件命令,注意文件的路径
//    if (PyRun_SimpleString("execfile('../graph.py')") == NULL)
//    {
//        return;
//    }
//
//    //第五步：关闭Python解释器
//    Py_Finalize();
//    return;
#endif
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
                    vFlavor[i]->_dayLine[dayCount]++;//这一天的这个虚拟机的个数+1
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



bool cmp_cpu(Flavor* a,Flavor* b)
{
    return a->_cpuNum>b->_cpuNum;
}
bool cmp_mem(Flavor* a,Flavor* b)
{
    return a->_memSize>b->_memSize;
}

string firstFit()
{
    if(predictFlag=="cpu")  sort(vFlavor.begin(),vFlavor.end(),cmp_cpu);//先排序
    else  sort(vFlavor.begin(),vFlavor.end(),cmp_mem);//先排序

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


    vServer.push_back(new Server(server->_cpuNum,server->_memSize/1024,server->_diskSize));

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
                        vServer.push_back(new Server(server->_cpuNum,server->_memSize/1024,server->_diskSize));//再申请一个新的物理机
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


int max(int x, int y)
{
    return x>y?x:y;
}

vector<Flavor*> dp(vector<Flavor*>& vv)
{
#ifdef _DEBUG
    cout<<"dpN = ";
    for(size_t i=0;i<vv.size();i++)
    {
        cout<<vv[i]->_cpuNum<<" ";
    }
    cout<<endl;
#endif
    int c=server->_cpuNum;
    int n=vv.size()-1;
    int m[1000][1000];

    memset(m,0,sizeof(m));
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=c;j++)
        {
            if(j>=vv[i]->_cpuNum)
                m[i][j]=max(m[i-1][j],m[i-1][j-vv[i]->_cpuNum]+vv[i]->_cpuNum);

            else
                m[i][j]=m[i-1][j];
        }
    }
    vector<Flavor*> ret;
    int temp=c;
    for(size_t i=n;i>1;i--)
    {
        if(m[i][temp]!=m[i-1][temp])
        {
            ret.push_back(vv[i]);
            temp-=vv[i]->_cpuNum;
            vv.erase(vv.begin()+i);
        }
    }
    if(m[1][temp]>0)
    {
        ret.push_back(vv[1]);
        vv.erase(vv.begin()+1);
    }
    //x[1]=(m[1][temp]>0)?1:0;

#ifdef _DEBUG
    for(int i=0;i<=c;i++)
    {
        cout<<i<<" ";
    }
    cout<<endl;
    for(int i=0;i<=c;i++)
    {
        cout<<"- ";
    }
    cout<<endl;

    for(int i=1;i<=n;i++)
    {
        cout<<i<<"|";
        for(int j=1;j<=c;j++)
        {
            cout<<m[i][j]<<' ';
        }
        cout<<endl;
    }

    cout<<"~~~~~~~~start~~~~~~~~~~~~~~~~"<<endl;

    cout<<"dp end = ";
    for(size_t i=1;i<vv.size();i++)
    {
        cout<<vv[i]->_cpuNum<<" ";
    }
    cout<<endl;

    for(size_t i=0;i<ret.size();i++)
    {
        cout<<ret[i]->_cpuNum<<" ";
    }
    cout<<endl;

    cout<<" "<<20-m[n][c]<<endl;
    cout<<"~~~~~~~~~end~~~~~~~~~~~~~~~~~"<<endl;
#endif
    return ret;
}

string dpPath()
{
    vector<Flavor*> vv;//
    vv.push_back(new Flavor(0,0,0));//动态规划  要多加一个0
    int predictFlavorNum=0;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        for(int j=0;j<vFlavor[i]->_predictNum;j++)
        {
            predictFlavorNum++;
            vv.push_back(vFlavor[i]);//传进去的是每台虚拟机的大小
        }
    }

    string str="";
    str+=to_string(predictFlavorNum)+"\n";
    for(int i=0;i<flavorNum;i++)
    {
        str+="flavor"+to_string(vFlavor[i]->_id)+" "+to_string(vFlavor[i]->_predictNum)+"\n";
    }

    int serverNum=0;

    int aa[MAX_FLAVORS];

    string str1="";
    while(vv.size()!=1)
    {
        memset(aa,0, sizeof(aa));
        vector<Flavor*> temp=dp(vv);
        serverNum++;

        for(size_t i =0;i<temp.size();i++)
        {
            aa[temp[i]->_id]++;
        }

        str1+=to_string(serverNum);
        for(int i=1;i<MAX_FLAVORS;i++)
        {
            if(aa[i])
                str1+=" flavor"+to_string(i)+" "+to_string(aa[i]);
        }
        str1+="\n";
    }

    str1+"\n";
    str+="\n"+to_string(serverNum)+"\n"+str1;


    return str;
}




