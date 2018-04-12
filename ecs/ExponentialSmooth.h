//
// Created by brunon on 18-4-1.
//

#ifndef ECS_EXPONENTIALSMOOTH_H
#define ECS_EXPONENTIALSMOOTH_H

#include <iostream>
#include <vector>
using namespace std;

//一次指数平滑预测
//传入的数据data以天为单位, n为数组大小,参数k表示k天为一组数据, forcase表示需要预测几组数据
//备注：n最好为k的倍数
//     实验发现用double和用float差距比较大
int ExponentialSmooth1(const vector<int> data, int n, int k);

//data格式： int data[] = {0, 2, 1, 1, 0, 6, 2, 1 ,0, 2, 1, 4, 3, 10, 2, 3, 1, 3, 3, 2, 5, 10, 1, 2, 8, 1, 2, 4, 2, 3};
//二次指数平滑预测
//传入的数据data以天为单位, n为数组大小,参数k表示k天为一组数据, forcase表示需要预测几组数据
//备注：n最好为k的倍数
//     实验发现用double和用float差距比较大
int ExponentialSmooth2(const vector<int> data, int n, int k, int forecase, vector<double >& s2_2_new);
//三次指数平滑预测
int ExponentialSmooth3(const vector<int> data, int n, int k);
int ExponentialSmooth3fix(const vector<int> data, int n, int k);
int ExponentialSmooth22(const vector<int> data, int n, int k, int forecase);   //二次指数平滑另一种公式
int ExponentialSmooth22fix(const vector<int> data, int n, int k);

#endif //ECS_EXPONENTIALSMOOTH_H
