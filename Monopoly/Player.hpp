//
//  Player.hpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <vector>
#include "Color.hpp"
#include "Tool.hpp"

using std::string;
using std::ostream;
using std::vector;

namespace monopoly {
    
    class Player {
    public:
        string name;
        bool direction;
        int curPos;
        int prePos;
        int x, y;
        vector<Tool> tools;
        int cash;
        int deposit;
        
        Player(string);
    };
    ostream& operator <<(ostream&, Player&);
}

#endif /* Player_hpp */
