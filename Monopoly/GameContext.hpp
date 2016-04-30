//
//  game.hpp
//  Monopoly
//
//  Created by Dragonly on 4/22/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include <iostream>
#include <map>

#include "Color.hpp"
#include "Controller.hpp"
#include "Date.hpp"
#include "GameState.hpp"
#include "Land.hpp"
#include "Player.hpp"
#include "Tool.hpp"

using std::map;

namespace monopoly {
    
    class Controller;
    
    class GameContext {
    public:
        GameState gs;
        Controller* controller;
        map<string, LandType> landMap;
        map<LandType, string> IlandMap;
        map<string, string> posSymbolMap;
        map<ToolType, string> toolMap;
        
        GameContext();
        ~GameContext();
        void init();
        void clear();
        void drawMap();
        void drawMenu();
        void drawGame();
        void gameLoop();
    };
}

#endif /* game_hpp */
