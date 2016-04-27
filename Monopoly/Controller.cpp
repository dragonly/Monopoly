//
//  Controller.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Controller.hpp"

using std::to_string;
using std::cout;
using std::endl;

namespace monopoly {
    Controller::Controller(GameContext& gc): gc(gc), gs(gc.gs) {
    }
    
    void Controller::nextTurn() {
        // vector 超出下标不会报错而是 undefined behavior = =, 异常坑
        gs.playerIndex = (gs.playerIndex + 1) % gs.players.size();
    }
    
    void Controller::eval(char* cmd) {
        gs.error = false;
        switch (gs.state){
                // return makes a good one, break makes a bad one
            case GS::normal:
            {
                if (strcmp(cmd, "t") == 0) {
                    gs.state = GS::tool;
                }
                else if (strcmp(cmd, "s") == 0) {
                    gs.message += "请输入步数(-9 ~ 9), 负号表示反向于前进方向: (输入 " + RED + "x" + NC + " 返回)";
                    gs.state = GS::step;
                }
                else if (strcmp(cmd, "i") == 0) {
                    Player &player = gs.currentPlayer();
                    gs.message += "玩家 " + gc.posSymbolMap[player.name] + "信息:";
                    gs.message += "\n现金: ￥" + to_string(player.cash);
                    gs.message += "\n存款: ￥" + to_string(player.deposit);
                }
                else if (strcmp(cmd, "r") == 0) {
                    gs.lastRoll = static_cast<int>(rand() % 6) + 1;
                    //                    gs.lastRoll = 5; // TODO: remove
                    movePlayerWithAnimation(gs.lastRoll);
                    handleEvents();
                }
                else if (strcmp(cmd, "gg") == 0) {
                    popCurrentPlayer();
                }
                else if (strcmp(cmd, "y") == 0) {
                    Player &player = gs.currentPlayer();
                    int x = gs.road[player.curPos].pos.first;
                    int y = gs.road[player.curPos].pos.second;
                    Land &land = gs.board[x][y];
                    if (land.owner == "none") { // 无主之地
                        if (player.cash < 200) {
                            gs.errMsg = "现金不足";
                            nextTurn();
                            break;
                        }
                        else {
                            land.owner = player.name;
                            land.name = player.name + "Land";
                            player.cash -= 200;
                        }
                    }
                    else if (land.owner == player.name) { // 升级房屋
                        if (player.cash < 200) {
                            gs.errMsg = "现金不足";
                            nextTurn();
                            break;
                        }
                        else if (land.level == land.maxLevel) {
                            gs.errMsg = "房屋达到最高等级";
                            nextTurn();
                            break;
                        }
                        else {
                            player.cash -= 200;
                            land.level += 1;
                        }
                    }
                    nextTurn();
                }
                else if (strcmp(cmd, "n") == 0) {
                    gs.message += "取消操作\n";
                    nextTurn();
                }
                else {
                    gs.errMsg = "什么鬼 _(:з」∠)_";
                    break; // error
                }
                return;
            }
            case GS::tool:
            {
                Player& player = gs.currentPlayer();
                size_t count = player.tools.size();
                if (strlen(cmd) == 1 && cmd[0] >= 48/*0*/ && cmd[0] < 48 + count) {
                    gs.message = string("你选择了道具: ") + cmd + player.tools[atoi(cmd)].type;
                    gs.state = GS::normal;
                    return;
                }
                else {
                    gs.errMsg = RED + "没有这个工具 :(" + NC;
                    break; // error
                }
            }
            case GS::step:
            {
                if (strcmp(cmd, "x") == 0) {
                    gs.state = GS::normal;
                    return;
                }
                int s = atoi(cmd);
                if (s > -10 && s < 10){
                    int pos = (gs.currentPlayer().curPos + s) % gs.road.size();
                    gs.message += "玩家" + LBLUE;
                    if (s > 0) gs.message += "前";
                    else       gs.message += "后";
                    gs.message += NC + to_string(abs(s)) + "步是";
                    int x = gs.road[pos].pos.first;
                    int y = gs.road[pos].pos.second;
                    gs.message += gc.posSymbolMap[gs.board[x][y].name];
                    gs.state = GS::normal;
                    return;
                }
                else {
                    gs.errMsg = RED + "无效的步数" + NC;
                    break; // error
                }
            }
        }
        gs.error = true;
    }
    
    void Controller::movePlayerWithAnimation(int delta) {
        for (int i = 0; i < delta; i++) {
            movePlayer(1);
            gc.drawGame();
            if (i != delta - 1) usleep(80000);
        }
    }
    
    void Controller::movePlayer(int delta) {
        int curX, curY, preX, preY;
        Player &player = gs.currentPlayer();
        player.prePos = player.curPos;
        if (player.direction) {
            player.curPos = (player.curPos + delta) % gs.road.size();
        }
        else {
            player.curPos = (player.curPos - delta) % gs.road.size();
        }
        curX = gs.road[player.curPos].pos.first;
        curY = gs.road[player.curPos].pos.second;
        preX = gs.road[player.prePos].pos.first;
        preY = gs.road[player.prePos].pos.second;
        gs.board[curX][curY].name = player.name;
        player.x = curX;
        player.y = curY;
        displayLandCorrectly(preX, preY);
    }
    
    // TODO: don't use cout here!
    void Controller::showTools() {
        Player& player = gs.currentPlayer();
        cout << "当前拥有道具:" << endl;
        for (int i = 0; i < player.tools.size(); i++) {
            cout << i << ". " << player.tools[i].type << "   ";
        }
    }
    
    void Controller::popCurrentPlayer() {
        vector<Player>::iterator it = gs.players.begin();
        for (int i = 0; i < gs.playerIndex; i++) {
            it++;
        }
        // erase a player will make the next player the currentPlayer
        // if last player quit, we should make the 0 indexed player the currentPlayer
        if (gs.playerIndex == gs.players.size() - 1) {
            gs.playerIndex = 0;
        }
        gs.players.erase(it);
        if (gs.players.size() == 1) {
            gs.gameover = true;
        }
    }
    
    void Controller::displayLandCorrectly(int x, int y) {
        Land &land = gs.board[x][y];
        // restore init display name
        if (gs.board[x][y].owner == "none") {
            gs.board[x][y].name = gc.IlandMap[gs.board[x][y].landType];
        }
        else {
            gs.board[x][y].name = gs.board[x][y].owner + string("Land");
        }
        for (int i = 0; i < gs.players.size(); i++) { // if player on land then display player
            if (gs.players[i].x == x && gs.players[i].y == y) {
                //            gs.board[x][y].name = gs.players[i].name;
                land.name = gs.players[i].name;
            }
        }
    }
    
    void Controller::displayPlayers() {
        int x, y;
        for (int i = 0; i < gs.players.size(); i++) {
            x = gs.players[i].x;
            y = gs.players[i].y;
            gs.board[x][y].name = gs.players[i].name;
        }
        // force current player on top
        x = gs.currentPlayer().x;
        y = gs.currentPlayer().y;
        gs.board[x][y].name = gs.currentPlayer().name;
    }
    
    void Controller::handleEvents() {
        Player &player = gs.currentPlayer();
        int x = gs.road[player.curPos].pos.first;
        int y = gs.road[player.curPos].pos.second;
        Land &land = gs.board[x][y];
        // TODO: 完善所有土地类型事件
        switch (gc.landMap[land.name]) {
            case LandType::land:
            {
                if (land.owner == "none") { // 无主之地
                    gs.message += "当前土地闲置, 是否花费 ￥200 购买?(y/n)";
                }
                else if (land.owner == player.name) { // 升级房屋
                    gs.message += "房屋当前等级" + RED + to_string(land.level) + NC;
                    gs.message += "\n是否花费 ￥200 升级?(y/n)";
                }
                else {
                    int penalty = land.basePrice * land.level;
                    player.cash -= penalty;
                    gs.getPlayerByName(land.owner).cash += penalty;
                    if (player.cash < 0) { // 玩家现金不够, 用银行存款抵扣
                        player.deposit += player.cash;
                        player.cash = 0;
                    }
                    gs.message += RED + "玩家" + GREEN + player.name + RED + "交过路费 ￥" + to_string(penalty) + NC;
                    if (player.deposit < 0) {
                        gs.message =  RED + "玩家" + GREEN + player.name + RED + "破产\n" + NC;
                        popCurrentPlayer();
                    }
                    nextTurn();
                }
            }
                break;
                
            default:
                break;
        }
    }
}