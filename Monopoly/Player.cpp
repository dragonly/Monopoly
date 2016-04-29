//
//  Player.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Player.hpp"

namespace monopoly {
    Player::Player(string name, map<ToolType, string> m): name(name), toolMap(m) {
        direction = true;
        curPos = 0;
        prePos = 0;
        x = 0;
        y = 0;
        cash = 1000;
        deposit = 0;
        done = false;
        usingMagicDice = false;
    }
    
    void Player::addTool(ToolType type) {
        tools.emplace_back(Tool(type, toolMap[type]));
    }
    
    ostream& operator <<(ostream& os, Player& player) {
        os << GREEN << player.name << NC;
        return os;
    }
}