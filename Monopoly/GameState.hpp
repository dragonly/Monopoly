//
//  GameState.hpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <iostream>
#include <vector>
#include <string>
#include "Land.hpp"
#include "Player.hpp"
#include "Date.hpp"

using std::vector;
using std::string;

namespace monopoly {
    enum class GS {
        normal, tool, step, buy
    };
    
    class GameState {
    public:
        vector<Land> road;
        Land board[10][20];
        GS state;
        bool error;
        bool gameover;
        string message;
        string errMsg;
        Date today;
        vector<Player> players;
        int playerIndex;
        int lastRoll;
        
        GameState();
        Player& currentPlayer();
        Player& getPlayerByName(string);
    };
}

#endif /* GameState_hpp */
