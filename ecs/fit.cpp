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
            str +=vServer[i]->_name+"-" + to_string(i + 1);
            for (size_t j = 1; j < MAX_FLAVORS + 1; j++) {
                if (server->_flavorNum[j] != 0)
                    str += " flavor" + to_string(j) + " " + to_string(server->_flavorNum[j]) + " ";
            }
            str += "\n";//注意我这里在结尾换行了，有可能369
        }
    }

    return str;
}








