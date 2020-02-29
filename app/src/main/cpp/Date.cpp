//
// Created by mojoncaliente on 2/13/20.
//

#include "Date.h"

#include "SQL.h"

#include <iostream>
#include <vector>
#include <string>
#include <time.h>

using namespace std;

/************************************************
* Uses time_t and struct tm to get current date *
************************************************/
void Date::getCurrentDate(vector<int>& dateVec) {
    // time_t, struct tm, and localtime_s for getting current date
    time_t rawtime = time(NULL);
    struct tm* timeinfo = new tm;
    localtime_s(timeinfo, &rawtime);

    dateVec.push_back((timeinfo->tm_year + 1900)); // Gets current year
    dateVec.push_back((timeinfo->tm_mon + 1)); // Gets current month
    dateVec.push_back(timeinfo->tm_mday); // Gets current day
}

/******************************************
* Converts a vector<int> into string type *
******************************************/
string Date::dateToString(vector<int>& dateVec) {
    string dateStr = "";

    int count = 0;
    for (int c : dateVec) {
        if (c < 10) {
            dateStr.append("0");
        }
        dateStr.append(to_string(c));
        count++;
        if (count < 3) {
            dateStr.append("-");
        }
    }

    return dateStr;
}

/*********************************************************
* Creates a date string that is more readable for humans *
*********************************************************/
string Date::rDateString(string date) {
    // Instantiate class objects
    SQL* sql = new SQL();

    // Convert string to int
    m_dateVec.clear();
    m_dateVec = stringToDate(date);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Get month string from DB
        pstmt = sql->con->prepareStatement("SELECT month FROM months WHERE id = ?");
        pstmt->setInt(1, m_dateVec[1]);
        res = pstmt->executeQuery();

        while (res->next()) {
            m_monthStr = res->getString("month");
        }

        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    // Create new date string
    m_dateStr.clear();
    if (m_dateVec[0] < 10) {
        m_dateStr.append("0");
    }
    m_dateStr.append(to_string(m_dateVec[0]));
    m_dateStr.append(m_monthStr);
    m_dateStr.append(to_string(m_dateVec[2]));

    return m_dateStr;
}

/*********************************************************
* Creates a date string that is more readable for humans *
*********************************************************/
string Date::transRDateString(string date) {
    // Instantiate class objects
    SQL* sql = new SQL();

    // Convert string to int
    m_dateVec.clear();
    m_dateVec = stringToDate(date);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Get month string from DB
        pstmt = sql->con->prepareStatement("SELECT month FROM months WHERE id = ?");
        pstmt->setInt(1, m_dateVec[1]);
        res = pstmt->executeQuery();

        while (res->next()) {
            m_monthStr = res->getString("month");
        }

        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    // Create new date string
    m_dateStr.clear();
    m_dateStr.append(m_monthStr);
    m_dateStr.append(" ");
    if (m_dateVec[0] < 10) {
        m_dateStr.append("0");
    }
    m_dateStr.append(to_string(m_dateVec[0]));

    return m_dateStr;
}

/*************************************************
* Converts a string type date into a vector<int> *
*************************************************/
vector<int> Date::stringToDate(string dbDates) {
    // Convert date from string to int
    m_year = stoi(dbDates.substr(0, 4));
    m_month = stoi(dbDates.substr(5, 2));
    m_day = stoi(dbDates.substr(8, 2));

    // Push_back into vector
    m_dateVec.push_back(m_day);
    m_dateVec.push_back(m_month);
    m_dateVec.push_back(m_year);

    return m_dateVec;
}

/*****************************************
* Finds the date 30 days from given date *
*****************************************/
vector<int> Date::getInterval(vector<int>& dateVec) {
    int first = 0;
    int last = 0;
    const int MAX = 12;
    int monthDays[MAX] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    vector<int> endDate;

    // count number of days between given date and end of month
    first = monthDays[dateVec[2]] - dateVec[0];
    last = 30 - first; // Should be the day of the following month

    // Set day to value of last
    m_day = last;

    // Set value of month
    if (last != 0) {
        if (dateVec[1] == 12) {
            // If last != 0, and the given month is already december,
            // then set month to 1 for January
            m_month = 1;
        }
        else if (dateVec[1] != 12) { // else set month to value of month of given day and add 1
            m_month = dateVec[1] + 1;
        }
    }
    else if (last == 0){ // if last is == 0, then the month stays the same
        m_month = dateVec[1];
    }

    // If month is a lower value than the given date, there is a change of year
    if (m_month < dateVec[1]) {
        m_year = dateVec[2] + 1;
    }

    // Push back date into vector
    endDate.push_back(m_day);
    endDate.push_back(m_month);
    endDate.push_back(m_year);

    return endDate;
}

/**********************************************
* Counts the number of days between two dates *
**********************************************/
int Date::daysCount(vector<int>& curDate, vector<int>& dateVec, int leapYears) {
    int days = 0;
    int years = 0;
    const int MAX = 12;
    int monthDays[MAX] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    if (curDate[1] != dateVec[1]) {
        // Count number of full years between dates
        years = curDate[2] - dateVec[2] - 1;

        // Calculate number of days for each year
        days = years * 365;

        // Add the Two Leap Years
        days += leapYears;

        // Calculate number of days for full months in start year
        for (int c = dateVec[1]; c < MAX; c++) {
            days += monthDays[c];
        }

        // Calculate number of days for full months in end year
        for (int c = (curDate[1] - 2); c > -1; c--) {
            days += monthDays[c];
        }

        // Count days for Start month
        days += monthDays[dateVec[1]] - dateVec[0];

        // Count days for End month
        days += curDate[0] + 1;
    }
    else {
        days = curDate[0] - dateVec[0];
    }

    return days;
}