//
//  Date.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Date.hpp"

namespace monopoly {
    
    Date::Date(int year, int month, int day) {
        d[0] = year;
        d[1] = month;
        d[2] = day;
    }
    
    bool Date::isLeapYear() {
        if (d[0] % 400 == 0)
            return true;
        if (d[0] % 100 == 0)
            return false;
        if (d[0] % 4 == 0)
            return true;
        return false;
    }
    
    void Date::nextDay() {
        if (d[2] <= 27) {
            d[2] += 1;
            return;
        }
        else if (d[1] == 2) { // Feb. and day >= 28
            if (isLeapYear()) { // Feb has 29
                if (d[2] == 28) {
                    d[2] += 1;
                    return;
                }
                d[1] = 3;
                d[2] = 1;
                return;
            }
            else { // Feb has 28
                d[1] = 3;
                d[2] = 1;
            }
        }
        else if (d[1] == 4 || d[1] == 6 || d[1] == 9 || d[1] == 11) {
            if (d[2] < 30) {
                d[2] += 1;
                return;
            }
            d[1] += 1;
            d[2] = 1;
        }
        else {
            if (d[2] < 31) {
                d[2] += 1;
                return;
            }
            if (d[1] != 12) {
                d[1] += 1;
                d[2] = 1;
                return;
            }
            d[0] += 1;
            d[1] = 1;
            d[2] = 1;
        }
    }
    
    ostream& operator <<(ostream& os, monopoly::Date& date) {
        os << date.d[0] << "年" << date.d[1] << "月" << date.d[2] << "日";
        return os;
    }

}