//
//  Date.hpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef Date_hpp
#define Date_hpp

#include <iostream>
using std::ostream;
namespace monopoly {

class Date {
public:
    int d[3];
    
    Date(int, int, int);
    bool isLeapYear();
    void nextDay();
};

ostream& operator <<(ostream&, Date&);

}

#endif /* Date_hpp */
