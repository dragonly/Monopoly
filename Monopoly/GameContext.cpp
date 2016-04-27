//
//  game.cpp
//  Monopoly
//
//  Created by Dragonly on 4/22/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "GameContext.hpp"

using std::cin;
using std::cout;
using std::endl;

namespace monopoly {
    
    GameContext::GameContext(): gs() {
        landMap["land"] = LandType::land;
        landMap["toolStore"] = LandType::toolStore;
        landMap["bank"] = LandType::bank;
        landMap["news"] = LandType::news;
        landMap["gift"] = LandType::gift;
        landMap["blank"] = LandType::blank;
        landMap["lottery"] = LandType::lottery;
        landMap["coupon"] = LandType::coupon;
        landMap["void"] = LandType::VOID;
        
        IlandMap[LandType::land] = "land";
        IlandMap[LandType::toolStore] = "toolStore";
        IlandMap[LandType::bank] = "bank";
        IlandMap[LandType::news] = "news";
        IlandMap[LandType::gift] = "gift";
        IlandMap[LandType::blank] = "blank";
        IlandMap[LandType::lottery] = "lottery";
        IlandMap[LandType::coupon] = "coupon";
        IlandMap[LandType::VOID] = "void";
        
        
        posSymbolMap["land"] = string("◎  ");
        posSymbolMap["toolStore"] = BROWN+string("道 ")+NC;
        posSymbolMap["bank"] = BLUE+string("银 ")+NC;
        posSymbolMap["news"] = PURPLE+string("新 ")+NC;
        posSymbolMap["gift"] = CYAN+string("赠 ")+NC;
        posSymbolMap["blank"] = LGRAY+string("空 ")+NC;
        posSymbolMap["lottery"] = LCYAN+string("彩 ")+NC;
        posSymbolMap["coupon"] = LRED+string("券 ")+NC;
        posSymbolMap["player1"] = YELLOW+string("P1 ")+NC;
        posSymbolMap["player2"] = YELLOW+string("P2 ")+NC;
        posSymbolMap["player1Land"] = LCYAN+string("♤  ")+NC;
        posSymbolMap["player2Land"] = LBLUE+string("♧  ")+NC;
        posSymbolMap["void"] = "   ";
        
        int i, j, k;
        for (k = 0; k < gs.road.size(); k++) {
            // fix land type of road
            gs.road[k].landType = landMap[gs.road[k].name];
            i = gs.road[k].pos.first;
            j = gs.road[k].pos.second;
            // fix board
            gs.board[i][j].name = gs.road[k].name;
            gs.board[i][j].landType = gs.road[k].landType;
            gs.board[i][j].street = gs.road[k].street;
        }
        gs.board[0][0].name = gs.currentPlayer().name;
        
        controller = new Controller(*this);
        
        init();
    }
    
    GameContext::~GameContext() {
        delete controller;
    }
    
    void GameContext::init() {
        srand(static_cast<unsigned>(std::time(0)));
    }
    
    void GameContext::clear() {
        system("clear");
    }
    
    void GameContext::drawMap() {
        controller->displayPlayers();
        string symbol;
        cout << "╔═════════════════════════════════════════════════════════════════╗" << endl;
        for(int i = 0; i < 10; i++) {
            cout << "║   ";
            for (int j = 0; j < 20; j++) {
                symbol = posSymbolMap[gs.board[i][j].name];
                if (gs.board[i][j].landType == LandType::land && gs.board[i][j].owner == "none") {
                    if (gs.board[i][j].street % 2 == 0) { // interleaved streets color
                        symbol = LGRAY + symbol + NC;
                    }
                    else {
                        symbol = DGRAY + symbol + NC;
                    }
                }
                cout << symbol;
            }
            cout << "  ║" << endl;
        }
        cout << "╚═════════════════════════════════════════════════════════════════╝" << endl;
    }
    
    void GameContext::drawMenu() {
        Player& player = gs.currentPlayer();
        Land& land = gs.board[player.x][player.y];
        string posName;
        
        if (land.landType == LandType::land) {
            if (land.owner == "none") {
                posName = "land";
            }
            else {
                posName = land.owner + "Land";
            }
        }
        else {
            posName = IlandMap[land.landType];
        }
        
        cout << "今天是" << gs.today << endl;
        cout << "现在是玩家 " << player << " 的回合, "
        << PURPLE << (gs.currentPlayer().direction ? "顺" : "逆") << NC << "时针, "
        << "当前位置" << posSymbolMap[posName] << endl;
        
        cout << "玩家信息: ";
        cout << BROWN << "现金￥" << player.cash << ", 存款￥" << player.deposit;
        cout << NC << " | " << LBLUE << "道具: ";
        
        vector<Tool>::iterator it = player.tools.begin();
        int i = 0;
        for (; !player.tools.empty() && it != player.tools.end()-1; it++, i++) {
            cout << i << "." << it->type << ", ";
        }
        if (!player.tools.empty()) {
            cout << i << "." << it->type << endl;
        }
        else {
            cout << "无" << endl;
        }
        
        cout << NC << "你可以做:" << endl
//        << "t(tool) - 道具列表" << endl
        << "s(step) - 查看前后若干步的具体信息" << endl
//        << "i(info) - 查看玩家资产信息" << endl
        << "r(roll) - 掷骰子" << endl
        << "gg(GG)  - 是在下输了" << endl;
    }
    
    void GameContext::drawGame() {
        clear();
        drawMap();
        drawMenu();
        cout << static_cast<int>(gs.state) << endl;
    }
    
    void GameContext::gameLoop() {
        char cmd[10] = "i";
        
        while (true) {
            controller->eval(cmd);
            drawGame();
            if (gs.gameover) {
                cout << RED << "= = = = = GAME OVER = = = = =" << endl;
                cout << "玩家" << gs.currentPlayer() << RED << "获胜" << endl;
                return;
            }
            if (gs.error) {
                cout << endl << RED << gs.errMsg << NC << endl;
            }
            if (gs.lastRoll != -1) {
                cout << "玩家" + GREEN + gs.currentPlayer().name + NC + "刚刚掷出了 " << LCYAN << gs.lastRoll << NC << endl;
                gs.lastRoll = -1;
            }
            cout << gs.message << endl;
            gs.message = "\n";
            cout << "> ";
            cin >> cmd;
            gs.today.nextDay();
        }
    }
}