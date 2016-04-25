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
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#define RED string("\033[0;31m")
#define GREEN string("\033[0;32m")
#define BROWN string("\033[0;33m")
#define BLUE string("\033[0;34m")
#define PURPLE string("\033[0;35m")
#define CYAN string("\033[0;36m")
#define LGRAY string("\033[0;37m")
#define DGRAY string("\033[1;30m")
#define LRED string("\033[1;31m")
#define LGREEN string("\033[1;32m")
#define YELLOW string("\033[1;33m")
#define LBLUE string("\033[1;34m")
#define LPURPLE string("\033[1;35m")
#define LCYAN string("\033[1;36m")
#define NC string("\033[0m")

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::ostream;

namespace monopoly {
    
    class Date {
    public:
        int d[3];
        
        Date();
        bool isLeapYear();
        void nextDay();
    };
    
    ostream& operator <<(ostream&, Date&);
    
    class Position {
    public:
        string name;
        pair<int, int> pos;
        Position(string name, int x, int y) : name(name), pos(make_pair(x,y)) {}
        Position() {}
    };
    
    enum class ToolType {
        TURNING_CARD, // turn around immediately
        STAY_CARD, // stay for one turn, trigger event in this position
        TURTLE_CARD, // make another player can only go 1 step in 3 turns
        MAGIC_DICE, // make a roll number as you want
        ROADBLOCK, // can be placed in 8 steps, make anyone passing by stop
        BUY_CARD, // buy the other player's land in this position
        TAX_CARD, // make the other player lose 30% deposit
        AVERAGE_CARD, // make all players' cash averaged
        LAND_CARD, // in 5 turns, make land your own in your position
        REMOVE_CARD, // remove all house in this street, owners get 150% of current price
        MONSTER_CARD, // make all house 1 level in this street
        MONEY_CARD, // get 10,000 cash immediately, won't pay on others' land in 8 turns
        FORTUNE_CARD, // get a random card immediately, won't pay on others' land in 8 turns
        LOTTERY_CARD, // manipulate result of lottery this month
        RED_CARD, // make stock rise 10% for 2 days
        BLACK_CARD // make stock fall 10% for 2 days
    };

    class Tool {
    public:
        string type;
        
        Tool(string type)
        : type(type)
        {
        }
    };
    ostream& operator <<(ostream&, Tool&);
    
    class Player {
    public:
        string name;
        bool direction;
        int curPos;
        int prePos;
        int x, y;
        vector<Tool> tools;
        
        Player(string name)
        : name(name), direction(true), curPos(0), prePos(0), x(0), y(0)
        {
        }
    };
    ostream& operator <<(ostream&, Player&);
    
    enum class GS {
        normal, tool, step, info
    };
    
    class GameState {
    public:
        vector<Position> road = {
            {"land", 0, 0}, {"land", 0, 1}, {"land", 0, 2}, {"land", 0, 3}, {"land", 0, 4},
            {"land", 0, 5}, {"land", 0, 6}, {"land", 0, 7}, {"land", 1, 7}, {"land", 1, 8},
            {"land", 1, 9}, {"news", 1, 10}, {"land", 2, 10}, {"land", 3, 10}, {"land", 4, 10},
            {"land", 4, 11}, {"land", 4, 12}, {"bank", 4, 13}, {"land", 4, 14}, {"land", 3, 14},
            {"land", 2, 14}, {"land", 1, 14}, {"land", 0, 14}, {"lottery", 0, 15}, {"land", 0, 16},
            {"land", 0, 17}, {"land", 0, 18}, {"coupon", 0, 19}, {"land", 1, 19}, {"land", 2, 19},
            {"land", 3, 19}, {"land", 4, 19}, {"land", 5, 19}, {"land", 6, 19}, {"land", 6, 18},
            {"blank", 6, 17}, {"land", 6, 16}, {"land", 7, 16}, {"land", 8, 16}, {"land", 8, 15},
            {"land", 8, 14}, {"toolStore", 7, 14}, {"land", 6, 14}, {"land", 6, 13}, {"land", 6, 12},
            {"land", 7, 12}, {"land", 8, 12}, {"gift", 9, 12}, {"land", 9, 11}, {"land", 9, 10},
            {"land", 9, 9}, {"land", 8, 9}, {"land", 7, 9}, {"news", 6, 9}, {"land", 6, 8},
            {"land", 6, 7}, {"land", 6, 6}, {"land", 5, 6}, {"land", 5, 5}, {"bank", 5, 4},
            {"land", 5, 3}, {"land", 5, 2}, {"land", 4, 2}, {"land", 3, 2}, {"land", 2, 2},
            {"lottery", 2, 1}, {"land", 2, 0}, {"land", 1, 0}};
        Position initBoard[10][20];
        Position board[10][20];  // index in road, -1 P1, -2 P2, -9 null
        GS state;
        bool error = false;
        string message = "";
        string errMsg = string(RED) + string("什么鬼 _(:з」∠)_") + string(NC);
        Date today;
        vector<Player> players;
        int playerIndex;
        int lastRoll = -1;
        
        GameState();
        Player &currentPlayer();
    };
    
    class Controller {
    public:
        void nextTurn();
        void eval(char*);
        void movePlayer(int);
        void movePlayerWithAnimation(int);
        void showTools();
        void popCurrentPlayer();
        void fixPosition(int x, int y);
    };
    
    void init();
    void clear();
    void drawMap();
    void drawPrompt();
    void drawGame();
    void gameLoop();
    
    static map<string, string> posSymbolMap = {
        {"land", LGREEN+string("◎  ")+NC},
        {"toolStore", BROWN+string("道 ")+NC},
        {"bank", BLUE+string("银 ")+NC},
        {"news", PURPLE+string("新 ")+NC},
        {"gift", CYAN+string("赠 ")+NC},
        {"blank", LGRAY+string("空 ")+NC},
        {"lottery", LCYAN+string("彩 ")+NC},
        {"coupon", LRED+string("券 ")+NC},
        {"player1", YELLOW+string("P1 ")+NC},
        {"player2", YELLOW+string("P2 ")+NC},
        {"player1Land", LCYAN+string("♠  ")+NC},
        {"palyer2Land", LBLUE+string("♧  ")+NC},
        {"void", "   "}};
    static GameState gs;
    static Controller controller;
}

#endif /* game_hpp */
