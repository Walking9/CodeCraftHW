//
// Created by hengliy on 3/21/18.
//

#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include "date.h"
#include <ctime>
using namespace std;

ostream& operator<<(ostream& _cout, const Date& date)
{
    _cout << date._year << "/" << date._month << "/" << date._day;
    return _cout;
}

istream& operator>>(istream& _cin, Date& date)
{
    _cin >> date._year >> date._month >> date._day;
    return _cin;
}

Date::Date(int year, int month, int day)
{
    if (year > 0 && (month > 0 && month < 13) &&
        (day > 0 && day <= GetDaysInMonth(year, month)))
    {
        _year = year;
        _month = month;
        _day = day;
    }
    else {
        cout << "the date is illegal\n";
    }
}

Date::Date(const Date& date) : _year(date._year), _month(date._month), _day(date._day) {}

Date::Date(const string& str)
{
    char * strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(strc,"-");
    this->_year=atoi(tmpStr);
    tmpStr = strtok(NULL,"-");
    this->_month=atoi(tmpStr);
    tmpStr = strtok(NULL,"-");
    this->_day=atoi(tmpStr);
    delete[] strc;
}

Date& Date::operator=(const Date& date)
{
    if (this != &date) {
        _year = date._year;
        _month = date._month;
        _day = date._day;
    }
    return *this;
}

Date Date::operator+(int day)
{
    Date temp(*this);
    if (day < 0) {
        day = -day;
        return temp - day;
    }
    temp._day += day;
    while (temp._day > GetDaysInMonth(temp._year, temp._month)) {
        temp._day -= GetDaysInMonth(temp._year, temp._month);
        if (temp._month == 12) {
            temp._year++;
            temp._month = 1;
        }
        else {
            temp._month++;
        }
    }
    return temp;
}

Date& Date::operator++()//前置++形如++a
{
    return (*this = *this + 1);
}

Date Date::operator++(int)//后置++形如a++
{
    Date temp(*this);
    *this = *this + 1;
    return temp;
}

Date Date::operator-(int day)
{
    Date temp(*this);
    temp._day -= day;
    while (temp._day <= 0) {
        if (temp._month == 1) {
            temp._year--;
            temp._month = 12;
        }
        else {
            temp._month--;
        }
        temp._day += GetDaysInMonth(temp._year, temp._month);
    }
    return temp;
}

int Date::operator-(const Date& date)
{
    Date maxDate(*this);
    Date minDate(date);
    if (maxDate < minDate) {
        maxDate = date;
        minDate = *this;
    }
    int days = 0;
    while (1) {
        if (minDate + days == maxDate)
            break;
        days++;
    }
    return days;
}

Date& Date::operator--()
{
    return (*this = *this - 1);
}

Date Date::operator--(int)
{
    Date temp(*this);
    *this = *this - 1;
    return temp;
}

bool  Date::operator>(const Date& date)
{
    if (_year > date._year || (_year == date._year && _month > date._month) || (_year == date._year && _month == date._month && _day > date._day))
    {
        return true;
    }
    else
        return false;
}

bool  Date::operator<(const Date& date)
{
    if (_year < date._year || (_year == date._year && _month < date._month) || (_year == date._year && _month == date._month && _day < date._day))
    {
        return true;
    }
    else
        return false;
}

bool  Date::operator==(const Date& date)
{
    return _year == date._year && _month == date._month && _day == date._day;
}

bool  Date::operator!=(const Date& date)
{
    return _year != date._year || _month != date._month || _day != date._day;
}

bool  Date::operator>=(const Date& date)
{
    if (*this < date)
        return false;
    else
        return true;
}

bool  Date::operator<=(const Date& date)
{
    if (*this > date)
        return false;
    else
        return true;
}

time_t StringToDatetime(string str)
{
    char *cha = (char*)str.data();             // 将string转换成char*。
    tm tm_;                                    // 定义tm结构体。
    int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
    sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
    tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
    tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
    tm_.tm_mday = day;                         // 日。
    tm_.tm_hour = hour;                        // 时。
    tm_.tm_min = minute;                       // 分。
    tm_.tm_sec = second;                       // 秒。
    tm_.tm_isdst = 0;                          // 非夏令时。
    time_t t_ = mktime(&tm_);                  // 将tm结构体转换成time_t格式。
    return t_;                                 // 返回值。
}

string DatetimeToString(time_t time)
{
    tm *tm_ = localtime(&time);                // 将time_t格式转换为tm结构体
    int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
    year = tm_->tm_year + 1900;                // 临时变量，年，由于tm结构体存储的是从1900年开始的时间，所以临时变量int为tm_year加上1900。
    month = tm_->tm_mon + 1;                   // 临时变量，月，由于tm结构体的月份存储范围为0-11，所以临时变量int为tm_mon加上1。
    day = tm_->tm_mday;                        // 临时变量，日。
    hour = tm_->tm_hour;                       // 临时变量，时。
    minute = tm_->tm_min;                      // 临时变量，分。
    second = tm_->tm_sec;                      // 临时变量，秒。
    char yearStr[5], monthStr[3], dayStr[3], hourStr[3], minuteStr[3], secondStr[3];// 定义时间的各个char*变量。
    sprintf(yearStr, "%d", year);              // 年。
    sprintf(monthStr, "%d", month);            // 月。
    sprintf(dayStr, "%d", day);                // 日。
    sprintf(hourStr, "%d", hour);              // 时。
    sprintf(minuteStr, "%d", minute);          // 分。
    if (minuteStr[1] == '\0')                  // 如果分为一位，如5，则需要转换字符串为两位，如05。
    {
        minuteStr[2] = '\0';
        minuteStr[1] = minuteStr[0];
        minuteStr[0] = '0';
    }
    sprintf(secondStr, "%d", second);          // 秒。
    if (secondStr[1] == '\0')                  // 如果秒为一位，如5，则需要转换字符串为两位，如05。
    {
        secondStr[2] = '\0';
        secondStr[1] = secondStr[0];
        secondStr[0] = '0';
    }
    char s[20];                                // 定义总日期时间char*变量。
    sprintf(s, "%s-%s-%s %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minuteStr, secondStr);// 将年月日时分秒合并。
    string str(s);                             // 定义string变量，并将总日期时间char*变量作为构造函数的参数传入。
    return str;                                // 返回转换日期时间后的string变量。
}