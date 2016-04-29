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
#include <map>
#include "Color.hpp"
#include "Tool.hpp"

using std::string;
using std::ostream;
using std::map;
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
        bool done; // 回合行动结束标志
        bool usingMagicDice; // 正在使用遥控骰子的标识
        map<ToolType, string> toolMap;
        
        Player(string, map<ToolType, string>);
        void addTool(ToolType);
    };
    ostream& operator <<(ostream&, Player&);
}

#endif /* Player_hpp */
