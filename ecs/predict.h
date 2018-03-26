#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include <string>
#include <vector>
using namespace std;

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename);

void initDataStruct(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM],int data_num);

bool isFlavor(int id);

int ExponentialSmooth2(vector<int> data, int n, int k) ;

//void place(const vector<vector<int> >& predictDayLine);

#endif
