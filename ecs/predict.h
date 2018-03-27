#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include <string>
#include <vector>
using namespace std;

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename);

void initDataStruct(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM],int data_num);

//
int predict(const vector<int> data, int n,int predictDays);

//             训练数组从第一天开始， 数组长度， 预测时长
int RnnPredict(vector<int> data, int n,int predictDays);

int ExponentialSmooth2(vector<int> data, int n, int k);


#endif
