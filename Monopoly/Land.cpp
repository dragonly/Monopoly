//
//  Land.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Land.hpp"
using std::make_pair;

namespace monopoly {
    Land::Land() {
        name = "void";
        pos = make_pair(-1, -1);
        level = 1;
        maxLevel = 6;
        street = -1;
        basePrice = 200;
        updateRatio = 0.5;
        owner = "none";
        landType = LandType::VOID;
    }
    
    Land::Land(string name, int x, int y, int street): name(name) {
        //        name = "233";
        pos = make_pair(x, y);
        level = 1;
        maxLevel = 6;
        street = street;
        basePrice = 200;
        updateRatio = 0.5;
        owner = "none";
        landType = LandType::VOID;
    }
}