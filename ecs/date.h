//
// Created by hengliy on 3/21/18.
//

#ifndef ECS_DATE_H
#define ECS_DATE_H

#include <iostream>
using namespace std;

class Date {
    friend ostream& operator<<(ostream& _cout, const Date& date);
    friend istream& operator>>(istream& _cin, Date& date);
public:
    Date(int year = 1990, int month = 1, int day = 1);
    Date(const Date& date);
    Date(const string& str);
    Date& operator=(const Date& date);
    Date operator+(int day);
    Date& operator++();
    Date operator++(int);
    Date operator-(int day);
    int operator-(const Date& date);
    Date& operator--();
    Date operator--(int);
    bool operator>(const Date& date);
    bool operator<(const Date& date);
    bool operator==(const Date& date);
    bool operator!=(const Date& date);
    bool operator>=(const Date& date);
    bool operator<=(const Date& date);
private:
    bool IsLeapYear(int year)
    {
        return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    }
    int GetDaysInMonth(int year, int month)
    {
        int months[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (IsLeapYear(year)) {
            months[2] = 29;
        }
        return months[month];
    }
private:
    int _year;
    int _month;
    int _day;
};

#endif //ECS_DATE_H
