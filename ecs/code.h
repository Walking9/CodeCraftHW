#ifndef CODE_H
#define CODE_H

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<list>
#include<time.h>
#include "date.h"
using namespace std;

#define MAX_DAYS 100
#define MAX_FLAVORS 16

class Server
{
    friend ostream & operator<<(ostream &,const Server &);
public:
    Server(int cpuNum,int memSize,int diskSize):
        _cpuNum(cpuNum),_memSize(memSize),_diskSize(diskSize) {
        _flavorNum.resize(MAX_FLAVORS);
        _memSize=_memSize*1024;
    }

    vector<int> _flavorNum; //第n台虚拟机在这个服务器上的个数
    int _cpuNum;//物理服务器CPU核数
    int _memSize;//内存大小（GB）
    int _diskSize;//硬盘大小（GB）
};


class Flavor
{
    friend ostream & operator<<(ostream &out, Flavor &flavor);
public:
    Flavor(int id,int cpuNum,int memSize):
        _id(id),_cpuNum(cpuNum),_memSize(memSize){
        _dayLine.resize(MAX_DAYS);
    }

    vector<int> _dayLine;//每个虚拟机对应的每一天的请求量
    int _predictNum;//预测出来的总个数
    int _id;//虚拟机规格名称1
    int _cpuNum;//CPU核数
    int _memSize;//内存大小（MB）
};

void paint();
string firstFit();
string dpPath();

#endif // CODE_H
