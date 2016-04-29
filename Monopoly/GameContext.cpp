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
using std::to_string;

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
        posSymbolMap["player1"] = LCYAN+string("P1 ")+NC;
        posSymbolMap["player2"] = LBLUE+string("P2 ")+NC;
        posSymbolMap["player1Land1"] = LCYAN+string("Д1 ")+NC;
        posSymbolMap["player1Land2"] = LCYAN+string("Д2 ")+NC;
        posSymbolMap["player1Land3"] = LCYAN+string("Д3 ")+NC;
        posSymbolMap["player1Land4"] = LCYAN+string("Д4 ")+NC;
        posSymbolMap["player1Land5"] = LCYAN+string("Д5 ")+NC;
        posSymbolMap["player1Land6"] = LCYAN+string("Д6 ")+NC;
        posSymbolMap["player2Land1"] = LBLUE+string("П1 ")+NC;
        posSymbolMap["player2Land2"] = LBLUE+string("П2 ")+NC;
        posSymbolMap["player2Land3"] = LBLUE+string("П3 ")+NC;
        posSymbolMap["player2Land4"] = LBLUE+string("П4 ")+NC;
        posSymbolMap["player2Land5"] = LBLUE+string("П5 ")+NC;
        posSymbolMap["player2Land6"] = LBLUE+string("П6 ")+NC;
        posSymbolMap["roadblock"] = RED+string("☀  ")+NC;
        posSymbolMap["void"] = "   ";
        
        toolMap[ToolType::MAGIC_DICE] = "遥控骰子";
        toolMap[ToolType::ROADBLOCK] = "路障";
        toolMap[ToolType::TURNING_CARD] = "转向卡";
        toolMap[ToolType::AVERAGE_CARD] = "均富卡";
        toolMap[ToolType::BUY_CARD] = "购地卡";
        toolMap[ToolType::REMOVE_CARD] = "拆迁卡";
        toolMap[ToolType::MONSTER_CARD] = "怪兽卡";
        
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
        
        controller = new Controller(*this);
        
        // finish GameState initialization
        gs.players.push_back(Player("player1", toolMap));
        gs.players.push_back(Player("player2", toolMap));
        
        gs.players[0].addTool(ToolType::ROADBLOCK);
        gs.players[0].addTool(ToolType::MAGIC_DICE);
        
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
                Land& land = gs.board[i][j];
                symbol = posSymbolMap[land.name];
                if (land.landType == LandType::land && !land.roadblock) {
                    if (land.owner == "none") {
                        if (land.street % 2 == 0) { // interleaved streets color
                            symbol = LGRAY + symbol + NC;
                        }
                        else {
                            symbol = DGRAY + symbol + NC;
                        }
                    }
                    else {
//                        symbol = posSymbolMap[land.owner + "Land" + to_string(land.level)];
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
                posName = land.owner + "Land" + to_string(land.level);
            }
        }
        else {
            posName = IlandMap[land.landType];
        }
        
        cout << "今天是" << gs.today << endl;
        cout << "现在是玩家 " << player << " 的回合, "
        << PURPLE << (gs.currentPlayer().direction ? "顺" : "逆") << NC << "时针, "
        << "当前位置" << posSymbolMap[posName];
        if (land.owner != "none") {
//            cout << ("等级 " + GREEN + to_string(land.level) + NC);
        }
        cout << endl;
        
        cout << "玩家信息: ";
        cout << BROWN << "现金￥" << player.cash << ", 存款￥" << player.deposit;
        cout << NC << " | " << LBLUE << "道具: ";
        
        vector<Tool>::iterator it = player.tools.begin();
        int i = 0;
        for (; !player.tools.empty() && it != player.tools.end()-1; it++, i++) {
            cout << i << "." << it->name << ", ";
        }
        if (!player.tools.empty()) {
            cout << i << "." << it->name << endl;
        }
        else {
            cout << "无" << endl;
        }
        
        cout << NC << "你可以做:" << endl
        << "t(tool) - 道具列表" << endl
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
//            controller->nextDay();
//            gs.today.nextDay();
        }
    }
}