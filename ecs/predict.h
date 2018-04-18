#ifndef __ROUTE_H__
#define __ROUTE_H__


#include <lib_io.h>
#include <vector>

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename);

int predict(const std::vector<int> data, int n,int spaceDays,int predictDays);

//             训练数组从第一天开始， 数组长度， 预测时长
int RnnPredict(std::vector<int> data, int n,int predictDays);

#endif
