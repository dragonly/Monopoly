//
//  game.cpp
//  Monopoly
//
//  Created by Dragonly on 4/22/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "game.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::to_string;

monopoly::Date::Date() {
    d[0] = 2015; d[1] = 2; d[2] = 27;
}

bool monopoly::Date::isLeapYear() {
    if (d[0] % 400 == 0)
        return true;
    if (d[0] % 100 == 0)
        return false;
    if (d[0] % 4 == 0)
        return true;
    return false;
}

void monopoly::Date::nextDay() {
    if (d[2] <= 27) {
        d[2] += 1;
        return;
    }
    else if (d[1] == 2) { // Feb. and day >= 28
        if (isLeapYear()) { // Feb has 29
            if (d[2] == 28) {
                d[2] += 1;
                return;
            }
            d[1] = 3;
            d[2] = 1;
            return;
        }
        else { // Feb has 28
            d[1] = 3;
            d[2] = 1;
        }
    }
    else if (d[1] == 4 || d[1] == 6 || d[1] == 9 || d[1] == 11) {
        if (d[2] < 30) {
            d[2] += 1;
            return;
        }
        d[1] += 1;
        d[2] = 1;
    }
    else {
        if (d[2] < 31) {
            d[2] += 1;
            return;
        }
        if (d[1] != 12) {
            d[1] += 1;
            d[2] = 1;
            return;
        }
        d[0] += 1;
        d[1] = 1;
        d[2] = 1;
    }
}

ostream& monopoly::operator <<(ostream& os, monopoly::Date& date) {
    os << date.d[0] << "年" << date.d[1] << "月" << date.d[2] << "日";
    return os;
}

ostream& monopoly::operator <<(ostream& os, Tool& tool) {
    
    return os;
}

ostream& monopoly::operator <<(ostream& os, Player& player) {
    os << GREEN << player.name << NC;
    return os;
}

void monopoly::Controller::nextTurn() {
    // vector 超出下标不会报错而是 undefined behavior = =, 异常坑
    gs.playerIndex = (gs.playerIndex + 1) % gs.players.size();
}

void monopoly::Controller::eval(char* cmd) {
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
                gs.message += "玩家 " + posSymbolMap[player.name] + "信息:";
                gs.message += "\n现金: ￥" + to_string(player.cash);
                gs.message += "\n存款: ￥" + to_string(player.deposit);
            }
            else if (strcmp(cmd, "r") == 0) {
                gs.lastRoll = static_cast<int>(rand() % 6) + 1;
                gs.lastRoll = 5;
                controller.movePlayerWithAnimation(gs.lastRoll);
                controller.handleEvents();
            }
            else if (strcmp(cmd, "gg") == 0) {
                controller.popCurrentPlayer();
            }
            else if (strcmp(cmd, "y") == 0) {
                Player &player = gs.currentPlayer();
                int x = gs.road[player.curPos].pos.first;
                int y = gs.road[player.curPos].pos.second;
                Land &displayLand = gs.displayBoard[x][y];
                Land &logicLand = gs.logicBoard[x][y];
                if (logicLand.owner == "none") { // 无主之地
                    if (player.cash < 200) {
                        gs.errMsg = "现金不足";
                        controller.nextTurn();
                        break;
                    }
                    else {
                        logicLand.owner = displayLand.owner = player.name;
                        displayLand.name = player.name + "Land";
                        player.cash -= 200;
                    }
                }
                else if (logicLand.owner == player.name) { // 升级房屋
                    if (player.cash < 200) {
                        gs.errMsg = "现金不足";
                        controller.nextTurn();
                        break;
                    }
                    else if (logicLand.level == logicLand.maxLevel) {
                        gs.errMsg = "房屋达到最高等级";
                        controller.nextTurn();
                        break;
                    }
                    else {
                        player.cash -= 200;
                        logicLand.level += 1;
                    }
                }
                controller.nextTurn();
            }
            else if (strcmp(cmd, "n") == 0) {
                gs.message += "取消操作\n";
                controller.nextTurn();
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
                gs.message += posSymbolMap[gs.logicBoard[x][y].name];
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

void monopoly::Controller::movePlayerWithAnimation(int delta) {
    for (int i = 0; i < delta; i++) {
        movePlayer(1);
        drawGame();
        if (i != delta - 1) usleep(80000);
    }
}

void monopoly::Controller::movePlayer(int delta) {
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
    gs.displayBoard[curX][curY] = Land(player.name, curX, curY, gs.logicBoard[curX][curY].street);
    gs.displayBoard[preX][preY] = Land(gs.logicBoard[preX][preY]);
    player.x = curX;
    player.y = curY;
    controller.fixLand(preX, preY);
}

void monopoly::Controller::showTools() {
    Player& player = gs.currentPlayer();
    cout << "当前拥有道具:" << endl;
    for (int i = 0; i < player.tools.size(); i++) {
        cout << i << ". " << player.tools[i].type << "   ";
    }
}

void monopoly::Controller::popCurrentPlayer() {
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

void monopoly::Controller::fixLand(int x, int y) {
    for (int i = 0; i < gs.players.size(); i++) {
        if (gs.players[i].x == x && gs.players[i].y == y) {
            gs.displayBoard[x][y] = Land(gs.players[i].name, x, y, gs.logicBoard[x][y].street);
        }
    }
}

void monopoly::Controller::handleEvents() {
    Player &player = gs.currentPlayer();
    int x = gs.road[player.curPos].pos.first;
    int y = gs.road[player.curPos].pos.second;
    Land &logicLand = gs.logicBoard[x][y];
    // TODO: 完善所有土地类型事件
    switch (landMap[logicLand.name]) {
        case LandName::land:
        {
            if (logicLand.owner == "none") { // 无主之地
                gs.message += "当前土地闲置, 是否花费 ￥200 购买?(y/n)";
            }
            else if (logicLand.owner == player.name) { // 升级房屋
                gs.message += "房屋当前等级" + RED + to_string(logicLand.level) + NC;
                gs.message += "\n是否花费 ￥200 升级?(y/n)";
            }
            else {
                int penalty = logicLand.basePrice * logicLand.level;
                player.cash -= penalty;
                gs.getPlayerByName(logicLand.owner).cash += penalty;
                if (player.cash < 0) { // 玩家现金不够, 用银行存款抵扣
                    player.deposit += player.cash;
                    player.cash = 0;
                }
                gs.message += RED + "玩家" + GREEN + player.name + RED + "交过路费 ￥" + to_string(penalty) + NC;
                if (player.deposit < 0) {
                    gs.message =  RED + "玩家" + GREEN + player.name + RED + "破产\n" + NC;
                    controller.popCurrentPlayer();
                }
                controller.nextTurn();
            }
        }
            break;
            
        default:
            break;
    }
}

void monopoly::init() {
    srand(static_cast<unsigned>(std::time(0)));
}

void monopoly::clear() {
    system("clear");
}

void monopoly::drawMap() {
    cout << "╔═════════════════════════════════════════════════════════════════╗" << endl;
    for(int i = 0; i < 10; i++) {
        cout << "║   ";
        for (int j = 0; j < 20; j++) {
            cout << posSymbolMap[gs.displayBoard[i][j].name];
        }
        cout << "  ║" << endl;
    }
    cout << "╚═════════════════════════════════════════════════════════════════╝" << endl;
}

void monopoly::drawMenu() {
    Player& player = gs.currentPlayer();
    string posName = gs.logicBoard[player.x][player.y].name;
    
    cout << "今天是" << gs.today << endl;
    cout << "现在是玩家 " << player << " 的回合, "
         << PURPLE << (gs.currentPlayer().direction ? "顺" : "逆") << NC << "时针, "
         << "当前位置" << posSymbolMap[posName] << endl;
    cout << "你可以做:" << endl
         << "t(tool) - 道具列表" << endl
         << "s(step) - 查看前后若干步的具体信息" << endl
         << "i(info) - 查看玩家资产信息" << endl
         << "r(roll) - 掷骰子" << endl
         << "gg(GG)  - 是在下输了" << endl;
}

void monopoly::drawGame() {
    clear();
    drawMap();
    drawMenu();
    
    switch (gs.state) {
        case GS::tool:
        {
            controller.showTools();
        }
        default:
            break;
    }
}

monopoly::Player& monopoly::GameState::currentPlayer() {
    return players[playerIndex];
}

monopoly::GameState::GameState() {
    // convert road to board
    int i, j, k;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 20; j++) {
            logicBoard[i][j] = Land("void", i, j, -1);
            displayBoard[i][j] = Land("void", i, j, -1);
        }
    }
    for (k = 0; k < road.size(); k++) {
        i = road[k].pos.first;
        j = road[k].pos.second;
        logicBoard[i][j] = Land(road[k]);
        displayBoard[i][j] = Land(road[k]);
    }
    displayBoard[0][0] = Land("player1", 0, 0, 0);
    
    players.push_back(Player("player1"));
    players.push_back(Player("player2"));
    
    players[0].tools.push_back(Tool("红卡"));
    players[0].tools.push_back(Tool("福神卡"));
}

monopoly::Player &monopoly::GameState::getPlayerByName(string name) {
    for (int i = 0; i < players.size(); i++) {
        if (players[i].name == name) {
            return players[i];
        }
    }
    return players[0]; // this is bad :( , but no fix for now
}

void monopoly::gameLoop() {
    char cmd[10] = "i";
    while (true) {
        controller.eval(cmd);
        drawGame();
        if (gs.gameover) {
            cout << RED << "= = = = = GAME OVER = = = = =" << endl;
            cout << "玩家" << gs.currentPlayer() << RED << "获胜" << endl;
            return;
        }
        
        cout << gs.message << endl;
        gs.message = "\n";
        if (gs.error) {
            cout << endl << RED << gs.errMsg << NC << endl;
        }
        if (gs.lastRoll != -1) {
            cout << "你刚刚掷出了 " << LCYAN << gs.lastRoll << NC << endl;
            gs.lastRoll = -1;
        }
        cout << "> ";
        cin >> cmd;
        gs.today.nextDay();
    }
}