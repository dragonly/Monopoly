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
    
    class Land {
    public:
        string name;
        pair<int, int> pos;
        int level;
        int street;
        int maxLevel;
        int basePrice;
        int updateRatio;
        string owner = "none";
        Land(string name, int x, int y, int street)
        :name(name), pos(make_pair(x,y)), street(street), level(1), maxLevel(6), basePrice(200), updateRatio(0.5)
        {
        }
        Land(): maxLevel(6), basePrice(200), updateRatio(0.5), street(-1) {}
        Land& operator= (const Land &l) {
            name = l.name;
            pos = l.pos;
            level = l.level;
            owner = l.owner;
            return *this;
        }
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
        int cash;
        int deposit;
        
        Player(string name)
        : name(name), direction(true), curPos(0), prePos(0), x(0), y(0), cash(1000), deposit(0)
        {
        }
    };
    ostream& operator <<(ostream&, Player&);
    
    enum class GS {
        normal, tool, step
    };
    
    class GameState {
    public:
        const vector<Land> road = { // this is just for initializing, don't do logic on it!!!
            {"land", 0, 0, 0}, {"land", 0, 1, 0}, {"land", 0, 2, 0}, {"land", 0, 3, 0}, {"land", 0, 4, 0},
            {"land", 0, 5, 0}, {"land", 0, 6, 0}, {"land", 0, 7, 0}, {"land", 1, 7, 1}, {"land", 1, 8, 1},
            {"land", 1, 9, 1}, {"news", 1, 10, 1}, {"land", 2, 10, 1}, {"land", 3, 10, 1}, {"land", 4, 10, 1},
            {"land", 4, 11, 2}, {"land", 4, 12, 2}, {"bank", 4, 13, 2}, {"land", 4, 14, 2}, {"land", 3, 14, 2},
            {"land", 2, 14, 2}, {"land", 1, 14, 2}, {"land", 0, 14, 2}, {"lottery", 0, 15, 3}, {"land", 0, 16, 3},
            {"land", 0, 17, 3}, {"land", 0, 18, 3}, {"coupon", 0, 19, 3}, {"land", 1, 19, 3}, {"land", 2, 19, 3},
            {"land", 3, 19, 3}, {"land", 4, 19, 3}, {"land", 5, 19, 3}, {"land", 6, 19, 4}, {"land", 6, 18, 4},
            {"blank", 6, 17, 4}, {"land", 6, 16, 4}, {"land", 7, 16, 4}, {"land", 8, 16, 4}, {"land", 8, 15, 4},
            {"land", 8, 14, 4}, {"toolStore", 7, 14, 5}, {"land", 6, 14, 5}, {"land", 6, 13, 5}, {"land", 6, 12, 5},
            {"land", 7, 12, 5}, {"land", 8, 12, 5}, {"gift", 9, 12, 5}, {"land", 9, 11, 5}, {"land", 9, 10, 5},
            {"land", 9, 9, 5}, {"land", 8, 9, 6}, {"land", 7, 9, 6}, {"news", 6, 9, 6}, {"land", 6, 8, 6},
            {"land", 6, 7, 6}, {"land", 6, 6, 6}, {"land", 5, 6, 6}, {"land", 5, 5, 6}, {"bank", 5, 4, 6},
            {"land", 5, 3, 6}, {"land", 5, 2, 6}, {"land", 4, 2, 7}, {"land", 3, 2, 7}, {"land", 2, 2, 7},
            {"lottery", 2, 1, 7}, {"land", 2, 0, 7}, {"land", 1, 0, 7}};
        // logicBoard is for game logic, displayBoard is for display
        Land logicBoard[10][20];
        Land displayBoard[10][20];  // index in road, -1 P1, -2 P2, -9 null
        GS state;
        bool error = false;
        bool gameover = false;
        string message = "";
        string errMsg = RED + "什么鬼 _(:з」∠)_" + NC;
        Date today;
        vector<Player> players;
        int playerIndex;
        int lastRoll = -1;
        
        GameState();
        Player &currentPlayer();
        Player &getPlayerByName(string);
    };
    
    class Controller {
    public:
        void nextTurn();
        void eval(char*);
        void movePlayer(int);
        void movePlayerWithAnimation(int);
        void showTools();
        void popCurrentPlayer();
        void fixLand(int x, int y);
        void handleEvents();
    };
    
    void init();
    void clear();
    void drawMap();
    void drawMenu();
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
        {"player1Land", LCYAN+string("♢  ")+NC},
        {"player2Land", LBLUE+string("♧  ")+NC},
        {"void", "   "}};
    enum class LandName {
        land, toolStore, bank, news, gift, blank, lottery, coupon, player1, player2, player1Land, player2Land, VOID
    };
    static map<string, LandName> landMap = {
        {"land", LandName::land},
        {"toolStore", LandName::toolStore},
        {"bank", LandName::bank},
        {"news", LandName::news},
        {"gift", LandName::gift},
        {"blank", LandName::blank},
        {"lottery", LandName::lottery},
        {"coupon", LandName::coupon},
        {"player1", LandName::player1},
        {"player1Land", LandName::player1Land},
        {"player2", LandName::player2},
        {"player2Land", LandName::player2Land}
    };
    static GameState gs;
    static Controller controller;
}

#endif /* game_hpp */
