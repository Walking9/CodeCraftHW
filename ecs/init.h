//
// Created by hengliy on 4/17/18.
//

#ifndef ECS_READ_H
#define ECS_READ_H


#include <lib_io.h>
#include <vector>
#include <iostream>
using namespace std;

#define MAX_TIME_DAYS 1000
#define MAX_HOUR_DAYS 4*30*24
#define MAX_DAYS 200
#define MAX_FLAVORS 18

class Flavor
{
    friend ostream & operator<<(ostream &out, Flavor &flavor);
public:
    Flavor(int id,int cpuNum,int memSize):
            _id(id),_cpuNum(cpuNum),_memSize(memSize){
        _dayLine.resize(MAX_DAYS);
        _hourLine.resize(MAX_HOUR_DAYS);
        //_timeLine.resize(MAX_TIME_DAYS);
    }

    vector<int> _dayLine;//每个虚拟机对应的每一天的请求量
    vector<int> _hourLine;
    vector<int> _timeLine;
    int _predictNum;//预测出来的总个数
    int _id;//虚拟机规格名称1
    int _cpuNum;//CPU核数
    int _memSize;//内存大小（MB）
};

class Server
{
    friend ostream & operator<<(ostream &,const Server &);
public:
    Server(int id,string name,int cpuNum,int memSize,int diskSize):
            _id(id),_name(name),_cpuNum(cpuNum),_memSize(memSize),_diskSize(diskSize) {
        _flavorNum.resize(MAX_FLAVORS+1);
    }

    Server(const  Server& other){
        _id=other._id;
        _name=other._name;
        _cpuNum=other._cpuNum;
        _memSize=other._memSize;
        _diskSize=other._diskSize;
        _flavorNum.resize(MAX_FLAVORS+1);
    }

    vector<int> _flavorNum; //第n台虚拟机在这个服务器上的个数
    int _id;
    string _name;
    int _cpuNum;//物理服务器CPU核数
    int _memSize;//内存大小（GB）
    int _diskSize;//硬盘大小（GB）
    void putFlavor(const Flavor* flavor);
};

void paint1(string name,vector<int> data1,int length1);
void paint2(std::vector<int> data1, int length1, std::vector<int>data2, int length2);

void initDataStruct(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM],int data_num);

int printTest(char* str);

void Test(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM],int data_num);

#endif //ECS_READ_H
