//
//  Land.hpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef Land_hpp
#define Land_hpp

#include <iostream>

using std::string;
using std::pair;
namespace monopoly {
    
    enum class LandType {
        land, toolStore, bank, news, gift, blank, lottery, coupon, player1, player2, player1Land, player2Land, VOID
    };
    
    class Land {
    public:
        string name;
        pair<int, int> pos;
        int street;
        int level;
        int maxLevel;
        int basePrice;
        int updateRatio;
        string owner;
        LandType landType;
        
        Land();
        Land(string, int, int, int);
    };
}

#endif /* Land_hpp */
