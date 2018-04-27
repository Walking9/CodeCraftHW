#ifndef CODE_H
#define CODE_H

#include <vector>
#include "init.h"

string firstFit();
string dpPath();
string getPrintStr(int predictFlavorNum,vector<Server*>& servers);
string getDebugStr(int predictNum,vector<Server*>& servers);
#endif // CODE_H
