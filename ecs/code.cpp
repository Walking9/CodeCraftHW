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
#include <numeric>

using namespace std;

int flavorNum;//虚拟机规格个数
Date* predictBeginDate;//预测开始时间
Date* predictEndDate;//预测结束时间
Date* predictBeginTime;//预测开始时间
Date* predictEndTime;//预测结束时间

string predictFlag;//预测标志 cpu/mem
int spaceDays;//间隔的天数

Server* server;//物理服务器
vector<Flavor*> vFlavor;//待检测虚拟机格式数组
vector<Server*> vServer;//构建预测的物理服务器链表
Date* beginDate;//训练数据开始时间
Date* endDate;//训练数据结束时间

ostream & operator<<(ostream &out,const Server &server)
{
    out<<server._cpuNum<<" "<<server._memSize<<" "<<server._diskSize<<endl;
    return out;
}

ostream & operator<<(ostream &out, Flavor &flavor)
{
    int days=*endDate-*beginDate+1;
    out<<flavor._id<<" "<<flavor._cpuNum<<" "<<flavor._memSize<<" ("<<days<<") -> ";
    for(int i=0;i<days;i++)
    {
        out<<flavor._dayLine[i]<<",";
    }
    out<<endl;
    return out;
}

void paint(vector<int> data1,int length1,vector<int>data2,int length2)
{
#ifdef _DEBUG
/*
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
*/
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
        Flavor* flavor=new Flavor(id,cpuNum,memSize/1024);
        vFlavor.push_back(flavor);
    }

    //预测标志（cpu/mem）
    ssstream<<info[4+flavorNum];
    ssstream>>predictFlag;
    ssstream.clear();
    //预测开始时间
    string preBeDaStr,preBeDaStrT;
    string preEndDaStr,preEndDaStrT;
    ssstream<<info[6+flavorNum];
    ssstream>>preBeDaStr>>preBeDaStrT;
    predictBeginDate=new Date(preBeDaStr);
    ssstream.clear();
    //预测结束时间
    ssstream<<info[7+flavorNum];
    ssstream>>preEndDaStr>>preEndDaStrT;
    predictEndDate=new Date(preEndDaStr);
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
                    Date* nowDate=new Date(date);
                    dayCount=(*nowDate-*beginDate);//算出是哪一天的
                    vFlavor[i]->_dayLine[dayCount]++;//这一天的这个虚拟机的个数+1
                }
            }
        }
    }
    ssstream<<data[data_num-1];
    ssstream>>recordId1>>flavorId1>>date1>>time1;
    ssstream.clear();
    endDate=new Date(date1);

//////////////////////////////////////////////////////////
#ifdef _DEBUG
    cout<<"/////////////////////////////"<<endl;
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

    cout<<*beginDate<<" ~ "<<*endDate<<endl;
    for(int i=0;i<flavorNum;i++)
    {
        cout<<*vFlavor[i];
    }
    //paint();
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


void sortRand(vector<Flavor*>& vv)
{
    srand((unsigned)time(NULL));
    int count=vv.size()+1;
    vector<Flavor*> temp;
    while(count--!=1)
    {
        int x=rand()%count;
        temp.push_back(vv[x]);
        vv.erase(vv.begin()+x);
    }
    vv=temp;
}


//1)交叉装箱算法(CF)
//从大到小排序,先放入最大的一个,再从最右边往左依次放入物品,直到下一个物品不能放入箱子.
//再重复上述操作直到所有物品放进去.
string crossFit()
{
#ifdef _DEBUG
    cout<<"all flavor size = ";
    cout<<endl;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        cout<<" id: "<<vFlavor[i]->_id<<" = "<<vFlavor[i]->_predictNum<<"("<<vFlavor[i]->_cpuNum<<")"<<endl;
    }
    cout<<endl;
#endif

    //把链表拉直了
    vector<Flavor*> vv;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        for(int j=0;j<vFlavor[i]->_predictNum;j++)
        {
            vv.push_back(vFlavor[i]);//直接把每台虚拟机的指针复制一份传进去
        }
    }
    string str="";
    str+=to_string(vv.size())+"\n";
    for(int i =0;i<flavorNum;i++)
    {
        str+="flavor"+to_string(vFlavor[i]->_id)+" "+to_string(vFlavor[i]->_predictNum)+"\n";
    }
    str+="\n";

    //针对链表指针进行降序排序
    if(predictFlag=="CPU")  sort(vv.begin(),vv.end(),cmp_cpu);//对CPU降序
    else  sort(vv.begin(),vv.end(),cmp_mem);//对MEM降序
#ifdef _DEBUG
    cout<<"()()()()()()()";
    for(int i=0;i<vv.size();i++)
    {
        cout<<" "<<vv[i]->_id<<"("<<vv[i]->_cpuNum<<")";
    }
    cout<<endl;
#endif
    while (vv.size())//此处进行大循环
    {
        Server* server0=new Server(*server);//每进行一次循环就证明后面的flavor放进去了
        //先放第一个节点

        Flavor* flavor=*vv.begin();
#ifdef _DEBUG
        cout<<endl<<"#############"<<server0->_cpuNum<<" ";
        cout<<" "<<flavor->_id<<"("<<flavor->_cpuNum<<")";
#endif
        //先放入最大的节点
        server0->_cpuNum-=flavor->_cpuNum;//物理服务器cpu资源消耗
        server0->_memSize-=flavor->_memSize;//物理服务器mem资源消耗
        server0->_flavorNum[flavor->_id]++;//物理服务器装的某个虚拟机个数+1
        flavor->_predictNum--;//虚拟机个数-1
        //在从后向前循环后面的节点

        for(vector<Flavor*>::iterator it=vv.end()-1;it!=vv.begin();it--)//已经把第一个放进去了,再从后向前(从小往大)装入
        {
            if((*it)->_cpuNum<=server0->_cpuNum&&(*it)->_memSize<=server0->_memSize)//如果两个都可以放下,就进放置
            {
                server0->_cpuNum-=(*it)->_cpuNum;//物理服务器cpu资源消耗
                server0->_memSize-=(*it)->_memSize;//物理服务器mem资源消耗
                server0->_flavorNum[(*it)->_id]++;//物理服务器装的某个虚拟机个数+1
                (*it)->_predictNum--;//虚拟机个数-1
                vv.erase(it);//从排序链表中摘除这个文件
#if _DEBUG
                cout<<" "<<(*it)->_id<<"("<<(*it)->_cpuNum<<")";
#endif
            }
            else{
                break;
            }
        }
        vv.erase(vv.begin());
        vServer.push_back(server0);
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

//
//
////2)启发式搜索(BIF)
//string BIF()
//{
//
//}



//3)首次适应算法
string firstFit()
{
//    //针对链表指针进行排序
//    if(predictFlag=="CPU")  sort(vFlavor.begin(),vFlavor.end(),cmp_cpu);//先排序
//    else  sort(vFlavor.begin(),vFlavor.end(),cmp_mem);//先排序


    sortRand(vFlavor);
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

    int flag=1;
    for(int i=0;i<flavorNum;i++)//循环所有的虚拟机
    {
        Flavor* flavor=vFlavor[i];
        while(flavor->_predictNum)//检查此虚拟机预测的个数
        {
            if(flag==1)
            {
                flag=0;
                vServer.push_back(new Server(server->_cpuNum,server->_memSize,server->_diskSize));
            }

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


int max(int x, int y)
{
    return x>y?x:y;
}
//4)动态规划
vector<Flavor*> dp2(vector<Flavor*>& vv)
{
    if(predictFlag=="CPU")  sort(vv.begin()+1,vv.end(),cmp_cpu);//先排序
    else  sort(vv.begin()+1,vv.end(),cmp_mem);//先排序
//    for(int i=0;i<vv.size()-1;i++)
//    {
//        cout<<vv[i]->_id<<" ";
//    }
//    cout<<"||||||||||||||||||||||||"<<endl;

//    srand((unsigned)time(NULL));
//    int count=vv.size()+1;
//    vector<Flavor*> temp;
//    while(count--!=1)
//    {
//        int x=rand()%count+1;
//        temp.push_back(vv[x]);
//        vv.erase(vv.begin()+x);
//    }
//    vv=temp;

//    for(int i=0;i<vv.size()-1;i++)
//    {
//        cout<<vv[i]->_id<<" ";
//    }
//    cout<<"||||||||||||||||||||||||"<<endl;

    int n,c,d;
    n=vv.size()-1;//装入物品的个数
    c=server->_cpuNum;
    d=server->_memSize;
#ifdef _DEBUG
    cout<<endl<<"***** dp2 print *******"<<endl;
    cout<<"准备放进去的虚拟机id = ";
    for(int i =1;i<=n;i++)
    {
        cout<<vv[i]->_id<<" ";
    }
    cout<<endl;
#endif

    //int dp[n+1][c+1][d+1]={0};

    int*** dp;
    //const int dpSize=70;
    dp=new int**[n+1];
    for(int i=0;i<n+1;i++)
    {
        dp[i]=new int*[c+1];
        for(int j=0;j<c+1;j++)
        {
            dp[i][j]=new int[d+1];
        }
    }

    //dp[i][j][k] i代表着第1到第i个虚拟机，j代表的是cpu，k代表的是mem，dp为最优价值
    for(int i=0;i<n+1;i++)
        for(int j=0;j<c+1;j++)
            for(int k=0;k<d+1;k++)
                dp[i][j][k]=0;


    for(int i=1;i<=n;i++)//循环所有虚拟机
    {
        Flavor* flavor=vv[i];
        for(int j =1;j<=c;j++)//从小到大循环cpu
            for(int k = 1 ;k <= d ; k++)//从小到大循环mem
            {
                if(flavor->_cpuNum<=j&&flavor->_memSize<=k)  //cpu可以放到当前物理机中，内存也可以满足的情况下，才考虑放进去
                    //dp[i][j][k] = max(dp[i-1][j][k] , dp[i-1][j-w[i]][k-b[i]] + v[i]);
                {
                    if(predictFlag=="CPU")
                        dp[i][j][k] = max(dp[i-1][j][k] , dp[i-1][j-flavor->_cpuNum][k-flavor->_memSize] + flavor->_cpuNum);//可优化时间
                    else
                        dp[i][j][k] = max(dp[i-1][j][k] , dp[i-1][j-flavor->_cpuNum][k-flavor->_memSize] + flavor->_memSize);
                }
                else dp[i][j][k] = dp[i-1][j][k];
            }
    }

    vector<Flavor*> ret;
    int tempc=c;
    int tempd=d;
    for(int i =n;i>1;i--)
    {
        if(dp[i][tempc][tempd]!=dp[i-1][tempc][tempd])
        {
            ret.push_back(vv[i]);
            tempc -= vv[i]->_cpuNum;
            tempd -= vv[i]->_memSize;
            vv.erase(vv.begin()+i);//把这个选中的删了
        }
    }

    if(dp[1][tempc][tempd]>0)//计算第一个
    {
        ret.push_back(vv[1]);
        vv.erase(vv.begin()+1);
    }
    //x[1]=(dp[1][c][d])?1:0;

#ifdef _DEBUG
    cout<<"背包能放物品的最大("<<predictFlag<<")为:"<<dp[n][c][d]<<endl;
    cout<<"被选入背包cpu|mem|价值分别是:"<<endl;
    for(size_t i=0;i<ret.size();i++)
    {
        if(predictFlag=="CPU")
            cout<<"第"<<ret[i]->_id<<"个物品: "<<ret[i]->_cpuNum<<"  "<<ret[i]->_memSize<<"  "<<ret[i]->_cpuNum<<endl;
        else
            cout<<"第"<<ret[i]->_id<<"个物品: "<<ret[i]->_cpuNum<<"  "<<ret[i]->_memSize<<"  "<<ret[i]->_memSize<<endl;
    }
#endif

    for(int i=0;i<n+1;i++)
    {
        for(int j=0;j<c+1;j++)
        {
            delete[] dp[i][j];
        }
        delete[] dp[i];
    }
    delete dp;

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
            vv.push_back(vFlavor[i]);//直接把每台虚拟机的指针复制一份传进去
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
        vector<Flavor*> temp=dp2(vv);
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




