//
// Created by mojoncaliente on 2/13/20.
//

#ifndef DATE_H
#define DATE_H

#include <vector>
#include <string>

using namespace std;

class Date {
private:
    int m_day;
    int m_month;
    int m_year;
    string m_monthStr;
    string m_dateStr;
    vector<int> m_dateVec;
public:
    Date() {}
    ~Date() {}

    void getCurrentDate(vector<int>&);               // Gets current date and puts into vector<int>
    string dateToString(vector<int>&);               // Converts vector<int> dates to string dates
    string rDateString(string);						 // Creates a human readable date string
    string transRDateString(string);                       // Creates a human readable date for Transaction list
    vector<int> stringToDate(string);                // Converts string dates to vecot<int> dates
    vector<int> getInterval(vector<int>&);           // Gets interval of days used for init_bal
    int daysCount(vector<int>&, vector<int>&, int);  // Counts number of days between dates
};

#endif
