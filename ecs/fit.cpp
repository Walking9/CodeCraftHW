#include"fit.h"
#include"date.h"
#include <algorithm>
#include "init.h"
using namespace std;

extern Date* trainBeginDate;//训练开始日期
extern Date* trainEndDate;//训练结束日期
extern Date* predictBeginDate;//预测开始日期
extern Date* predictEndDate;//预测结束日期

extern time_t* trainBeginTime;//训练数据开始时间
extern time_t* trainEndTime;//训练数据结束时间
extern time_t* predictBeginTime;//预测开始时间
extern time_t* predictEndTime;//预测结束时间

extern string predictFlag;//预测目标

extern vector<Flavor*> vFlavor;//待检测虚拟机格式数组
extern vector<Server*> vServer;//构建物理服务器链表

extern int trainDays;
extern int spaceDays;
extern int predictDays;



bool cmp_server(Server* servera,Server* serserb)
{
    return (servera->_cpuNum+servera->_memSize)<(serserb->_cpuNum+serserb->_memSize);
}

bool cmp_flavor(Flavor* flavora,Flavor* flavorb)
{
    return (flavora->_cpuNum+flavora->_memSize)>(flavorb->_cpuNum+flavorb->_memSize);
}

//《尺寸可变的装箱问题的近似算法研究》
string firstFit()
{
    vector<Flavor*> vv;//
    int predictNum=0;
    for(size_t i=0;i<vFlavor.size();i++)
    {
        for(int j=0;j<vFlavor[i]->_predictNum;j++)
        {
            predictNum++;
            vv.push_back(vFlavor[i]);//直接把每台虚拟机的指针复制一份传进去
        }
    }

    sort(vv.begin(),vv.end(),cmp_flavor);//对CPU降序

    vector<Server*> servers;

    int flag=1;
    for(size_t i=0;i<vv.size();i++)
    {
        Flavor* flavor=vv[i];
        if(flag==1)
        {
            flag=0;
            Server* newServer;
            //找出这个虚拟机的最小物理机类型
            for(size_t k=0;k<vServer.size();k++)
            {
                if(vServer[k]->_cpuNum>=flavor->_cpuNum&&vServer[k]->_memSize>=flavor->_cpuNum)
                {
                    newServer=new Server(*vServer[k]);
                    break;
                }
            }
            newServer->putFlavor(flavor);
            servers.push_back(newServer);
            continue;
        }

        for(size_t j=0;j<servers.size();j++)
        {
            Server* server=servers[j];
            //是否能够放进去
            if((server->_cpuNum>=flavor->_cpuNum)&&(server->_memSize>=flavor->_memSize))
            {
                server->putFlavor(flavor);
                break;//如果已经放进去了，就不管别的了。
            }
            else{
                if(j==servers.size()-1)//如果找到最后一个都没有放进去
                {
                    Server* newServer;
                    //找出这个虚拟机的最小物理机类型
                    for(size_t k=0;k<vServer.size();k++)
                    {
                        if(vServer[k]->_cpuNum>=flavor->_cpuNum&&vServer[k]->_memSize>=flavor->_cpuNum)
                        {
                            newServer=new Server(*vServer[k]);
                            break;
                        }
                    }
                    newServer->putFlavor(flavor);
                    servers.push_back(newServer);
                    break;
                }
            }

        }
        //每次结束后进行一次新的排序？ 对物理机空余大小从小到大进行排序？？？
        sort(servers.begin(),servers.end(),cmp_server);
    }

    for(size_t i=0;i<servers.size();i++)
    {
        cout<<servers[i]->_name<<endl;
    }


    string str="";
    str=getPrintStr(predictNum,servers);

    for(size_t i=0;i<servers.size();i++)
    {
        delete servers[i];
    }

    return str;

}

int max(int x, int y)
{
    return x>y?x:y;
}

//4)动态规划
vector<Flavor*> dp2(Server* server,vector<Flavor*>& vv)
{
    sort(vv.begin()+1,vv.end(),cmp_flavor);//对flavor降序

    int n,c,d;
    n=vv.size()-1;//装入物品的个数
    c=server->_cpuNum;
    d=server->_memSize;
#ifdef _DEBUG
//    cout<<endl<<"***** dp2 print *******"<<endl;
//    cout<<"准备放进去的虚拟机id = ";
//    for(int i =1;i<=n;i++)
//    {
//        cout<<vv[i]->_id<<" ";
//    }
//    cout<<endl;
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
                    dp[i][j][k] = max(dp[i-1][j][k] , dp[i-1][j-flavor->_cpuNum][k-flavor->_memSize] + (flavor->_cpuNum+flavor->_memSize));//可优化时间
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
            vv.erase(vv.begin()+i);//把这个选中的删了?????????????????????????????????????????????????????????
        }
    }

    if(dp[1][tempc][tempd]>0)//计算第一个
    {
        ret.push_back(vv[1]);
        vv.erase(vv.begin()+1);
    }
    //x[1]=(dp[1][c][d])?1:0;

#ifdef _DEBUG
//    cout<<"背包能放物品的最大("<<predictFlag<<")为:"<<dp[n][c][d]<<endl;
//    cout<<"被选入背包cpu|mem|价值分别是:"<<endl;
//    for(size_t i=0;i<ret.size();i++)
//    {
//        cout<<"第"<<ret[i]->_id<<"个物品: "<<ret[i]->_cpuNum+ret[i]->_memSize<<endl;
//    }
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


string dpPath() {
#ifdef _DEBUG
    cout<<endl<<"~~~~dp~~~~"<<endl;
#endif
    vector<Flavor *> vv;//
    int predictNum = 0;
    for (size_t i = 0; i < vFlavor.size(); i++) {
        for (int j = 0; j < vFlavor[i]->_predictNum; j++) {
            predictNum++;
            vv.push_back(vFlavor[i]);//直接把每台虚拟机的指针复制一份传进去
        }
    }

    ////////////
    //sortRand(vv);
    ////////////

    vv.insert(vv.begin(), new Flavor(0, 0, 0));//动态规划  要多加一个0

    vector<Server*> servers;

    while (vv.size() != 1)
    {
        vector<Server*> cmpServer;
        //只计算资源利用率
        for (size_t i = 0; i < vServer.size(); i++) {
            vector<Flavor *> temp = vv;
            Server* newServer=new Server(*vServer[i]);
            vector<Flavor *> retFlavor = dp2(newServer,temp);
            for (size_t j = 0; j < retFlavor.size(); j++) {
                newServer->putFlavor(retFlavor[j]);
            }
            cmpServer.push_back(newServer);
        }

        //找出利用率最大的那一个
        double maxRate=0;
        int id=0;
        for(size_t i=0;i<cmpServer.size();i++)
        {
            double rate=(1-cmpServer[i]->_cpuNum/static_cast<double>(vServer[i]->_cpuNum))+(1-cmpServer[i]->_memSize/static_cast<double>(vServer[i]->_memSize));
            if(rate>maxRate) {
                maxRate = rate;
                id = i;
            }
        }

        Server *newServer = new Server(*vServer[id]);
        vector<Flavor*> temp=dp2(newServer, vv);
        for(size_t i=0;i<temp.size();i++)
        {
            newServer->putFlavor(temp[i]);
        }
        servers.push_back(newServer);

        for(size_t i=0;i<cmpServer.size();i++)
        {
            delete cmpServer[i];
            cmpServer[i]=NULL;
        }
        //再运行一遍
    }

    /*
    Server* serverEnd=(*(servers.end()-1));
    int breakFlavor=0;
    int rate=serverEnd->_cpuNum/vServer[serverEnd->_id]->_cpuNum+serverEnd->_memSize/vServer[serverEnd->_id]->_memSize;
    if(rate<0.5)
    {
        cout<<endl<<"++++++++++++"<<rate<<endl;
        for(size_t j=0;j<serverEnd->_flavorNum.size();j++)
        {
            if(serverEnd->_flavorNum[j]!=0)
            {
                for(size_t k=0;k<vFlavor.size();k++)
                {
                    if(vFlavor[k]->_id==j)
                    {
                        breakFlavor+=serverEnd->_flavorNum[j];
                        vFlavor[k]->_predictNum-=serverEnd->_flavorNum[j];
                        break;
                    }
                }
            }
        }
        servers.erase(servers.end()-1);
    }
    */

    string str=getPrintStr(predictNum,servers);
#ifdef _DEBUG
    string debugStr=getDebugStr(predictNum,servers);
    cout<<debugStr<<endl;
#endif
    for(size_t i=0;i<servers.size();i++)
    {
        delete servers[i];
        servers[i]=NULL;
    }
    return str;
}


string getPrintStr(int predictNum,vector<Server*>& servers)
{
    string str="";
    str+=to_string(predictNum)+"\n";
    for(size_t i =0;i<vFlavor.size();i++)
    {
        str+="flavor"+to_string(vFlavor[i]->_id)+" "+to_string(vFlavor[i]->_predictNum)+"\n";
    }

//    vector<Server*> generalServers;
//    vector<Server*> largeMemoryServers;
//    vector<Server*> highPerformanceServers;

    vector<vector<Server*> > vvserver(vServer.size());
    //组装三个数组
    for(vector<Server*>::iterator server=servers.begin();server!=servers.end();server++)
    {
        for(size_t i=0;i<vServer.size();i++)
            if((*server)->_name==vServer[i]->_name) vvserver[i].push_back(*server);
    }

    for(size_t i=0;i<vServer.size();i++)
    {
        str += "\n";
        str +=vServer[i]->_name+" " + to_string(vvserver[i].size()) + "\n";
        for (size_t k = 0; k < vvserver[i].size(); k++) {
            Server *server = vvserver[i][k];
            str +=vServer[i]->_name+"-" + to_string(k + 1);
            for (size_t j = 1; j < MAX_FLAVORS + 1; j++) {
                if (server->_flavorNum[j] != 0)
                    str += " flavor" + to_string(j) + " " + to_string(server->_flavorNum[j]) + " ";
            }
            str += "\n";//注意我这里在结尾换行了，有可能369
        }
    }

    return str;
}

string getDebugStr(int predictNum,vector<Server*>& servers)
{
    string str="";
    str+=to_string(predictNum)+"\n";
    for(size_t i =0;i<vFlavor.size();i++)
    {
        str+="flavor"+to_string(vFlavor[i]->_id)+" "+to_string(vFlavor[i]->_predictNum)+"\n";
    }

//    vector<Server*> generalServers;
//    vector<Server*> largeMemoryServers;
//    vector<Server*> highPerformanceServers;

    vector<vector<Server*> > vvserver(vServer.size());
    //组装三个数组
    for(vector<Server*>::iterator server=servers.begin();server!=servers.end();server++)
    {
        for(size_t i=0;i<vServer.size();i++)
            if((*server)->_name==vServer[i]->_name)
                vvserver[i].push_back(*server);
    }

    for(size_t i=0;i<vServer.size();i++)
    {
        str += "\n";
        str +=vServer[i]->_name+" " + to_string(vvserver[i].size()) + "\n";
        for (size_t k = 0; k < vvserver[i].size(); k++) {
            Server *server = vvserver[i][k];
            str +=vServer[i]->_name+"-" + to_string(k + 1);
            for (size_t j = 1; j < MAX_FLAVORS + 1; j++) {
                if (server->_flavorNum[j] != 0)
                    str += " flavor" + to_string(j) + " " + to_string(server->_flavorNum[j]) + " ";
            }
            double rate=(1-server->_cpuNum/static_cast<double>(vServer[i]->_cpuNum))+(1-server->_memSize/static_cast<double>(vServer[i]->_memSize));
            str +=" ["+to_string(server->_cpuNum)+","+to_string(server->_memSize)+"] ("+to_string(rate)+")";
            str += "\n";//注意我这里在结尾换行了，有可能369
        }
    }

    return str;
}



