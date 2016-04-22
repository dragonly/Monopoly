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
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;

namespace monopoly
{
    void init();
    void drawMap();
    void drawScreen();
    void gameLoop();
    
    class Position
    {
    public:
        string name;
        pair<int, int> pos;
        Position(string name, int x, int y) : name(name), pos(make_pair(x,y)) {}
        Position() {}
    };
    
    class GameState
    {
    public:
        vector<Position> road = {{"land", 0, 0}, {"land", 0, 1}, {"land", 0, 2}, {"land", 0, 3}, {"land", 0, 4}, {"land", 0, 5}, {"land", 0, 6}, {"land", 0, 7}, {"land", 1, 7}, {"land", 1, 8}, {"land", 1, 9}, {"news", 1, 10}, {"land", 2, 10}, {"land", 3, 10}, {"land", 4, 10}, {"land", 4, 11}, {"land", 4, 12}, {"bank", 4, 13}, {"land", 4, 14}, {"land", 3, 14}, {"land", 2, 14}, {"land", 1, 14}, {"land", 0, 14}, {"lottery", 0, 15}, {"land", 0, 16}, {"land", 0, 17}, {"land", 0, 18}, {"coupon", 0, 19}, {"land", 1, 19}, {"land", 2, 19}, {"land", 3, 19}, {"land", 4, 19}, {"land", 5, 19}, {"land", 6, 19}, {"land", 6, 18}, {"blank", 6, 17}, {"land", 6, 16}, {"land", 7, 16}, {"land", 8, 16}, {"land", 8, 15}, {"land", 8, 14}, {"toolStore", 7, 14}, {"land", 6, 14}, {"land", 6, 13}, {"land", 6, 12}, {"land", 7, 12}, {"land", 8, 12}, {"gift", 9, 12}, {"land", 9, 11}, {"land", 9, 10}, {"land", 9, 9}, {"land", 8, 9}, {"land", 7, 9}, {"news", 6, 9}, {"land", 6, 8}, {"land", 6, 7}, {"land", 6, 6}, {"land", 5, 6}, {"land", 5, 5}, {"bank", 5, 4}, {"land", 5, 3}, {"land", 5, 2}, {"land", 4, 2}, {"land", 3, 2}, {"land", 2, 2}, {"lottery", 2, 1}, {"land", 2, 0}, {"land", 1, 0}};
        int board[10][20];
        
        GameState();
    };
    
    static GameState gs;
    static map<string, string> nameSymbolMap = {{"land", "◎ "}, {"toolStore", "道"}, {"bank", "银"}, {"news", "新"}, {"gift", "赠"}, {"blank", "空"}, {"lottery", "彩"}, {"coupon", "券"}, {"player1", "1  "}, {"player1Land", "♠ "}, {"player2", "2  "}, {"palyer2Land", "♧ "}};
    
    class Player
    {
        
    };
}

#endif /* game_hpp */
