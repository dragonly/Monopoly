//
//  Controller.hpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <iostream>
#include <unistd.h>
#include "GameContext.hpp"
#include "GameState.hpp"

namespace monopoly {
    
    class GameContext;
    
    class Controller {
    private:
        GameContext& gc;
        GameState& gs;
    public:
        Controller(GameContext& gc);
        void nextTurn();
        void eval(char*);
        void movePlayer(int);
        void movePlayerWithAnimation(int);
        void showTools();
        void popCurrentPlayer();
        void displayLandCorrectly(int, int);
        void displayPlayers();
        void handleEvents();
    };
}

#endif /* Controller_hpp */
