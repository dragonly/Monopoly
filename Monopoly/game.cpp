//
//  game.cpp
//  Monopoly
//
//  Created by Dragonly on 4/22/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "game.hpp"

using std::cout;
using std::endl;
using std::cin;
using std::map;
//using monopoly::gs;
//using monopoly::Position;

void monopoly::init()
{
}

void monopoly::drawMap()
{
//    using monopoly::gs;
//    using
    int cur;
    string name;
    for(int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            cur = gs.board[i][j];
            if (cur == -1) {
                cout << "   ";
            }
            else {
                name = gs.road[cur].name;
                cout << nameSymbolMap[name] << " ";
            }
        }
        cout << endl;
    }
}

void monopoly::drawScreen()
{
    drawMap();
}

void monopoly::gameLoop()
{
    while (true) {
        system("clear");
        
        drawScreen();
        
        cout << "> ";
        cin.get();
    }
}

monopoly::GameState::GameState()
{
    int i, j, k;
    for(i = 0; i < 10; i++)
        for(j = 0; j < 20; j++)
            board[i][j] = -1;
    
    for (k = 0; k < road.size(); k++) {
        i = road[k].pos.first;
        j = road[k].pos.second;
        board[i][j] = k;
    }
}