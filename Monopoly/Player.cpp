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
        usingRoadblock = false;
    }
    
    void Player::addTool(ToolType type) {
        tools.emplace_back(Tool(type, toolMap[type]));
    }
    
    void Player::dropTool(int i) {
        vector<Tool>::iterator it = tools.begin();
        for (int j = 0; j < i; j++, it++) ;
        tools.erase(it);
    }
    
    ostream& operator <<(ostream& os, Player& player) {
        os << GREEN << player.name << NC;
        return os;
    }
}