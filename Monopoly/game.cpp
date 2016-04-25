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
                gs.state = GS::step;
            }
            else if (strcmp(cmd, "i") == 0) {
                gs.state = GS::info;
            }
            else if (strcmp(cmd, "r") == 0) {
                gs.lastRoll = static_cast<int>(rand() % 6) + 1;
                controller.movePlayerWithAnimation(gs.lastRoll);
                controller.nextTurn();
            }
            else if (strcmp(cmd, "gg") == 0) {
                controller.popCurrentPlayer();
            }
            else {
                gs.errMsg = "什么鬼 _(:з」∠)_";
                break; // error
            }
            return;
        }
        case GS::tool:
        {
            size_t count = gs.currentPlayer().tools.size();
            if (strlen(cmd) == 1 && cmd[0] >= 48/*0*/ && cmd[0] < 48 + count) {
                gs.message = string("选择工具") + cmd + gs.currentPlayer().tools[atoi(cmd)].type;
                gs.state = GS::normal;
                return;
            }
            else {
                gs.errMsg = RED + "没有这个工具 :(" + NC;
                break; // error
            }
        }
        case GS::info:
        {
            return;
        }
        case GS::step:
        {
            gs.message = "请输入步数(-9 ~ 9), 负号表示反向于前进方向: ";
            int s = atoi(cmd);
            if (s > -10 && s < 10){
                int pos = (gs.currentPlayer().curPos + s) % gs.road.size();
                gs.message = "玩家" + LBLUE;
                if (s > 0) gs.message += "前";
                else       gs.message += "后";
                gs.message += NC + std::to_string(abs(s)) + "步是";
                gs.message += posSymbolMap[gs.road[pos].name];
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
    gs.board[curX][curY] = Position(player.name, curX, curY);
    gs.board[preX][preY] = Position(gs.initBoard[preX][preY]);
    player.x = curX;
    player.y = curY;
    controller.fixPosition(preX, preY);
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
    gs.playerIndex = (gs.playerIndex + 1) % gs.players.size();
    gs.players.erase(it);
}

void monopoly::Controller::fixPosition(int x, int y) {
    for (int i = 0; i < gs.players.size(); i++) {
        if (gs.players[i].x == x && gs.players[i].y == y) {
            gs.board[x][y] = Position(gs.players[i].name, x, y);
        }
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
            cout << posSymbolMap[gs.board[i][j].name];
        }
        cout << "  ║" << endl;
    }
    cout << "╚═════════════════════════════════════════════════════════════════╝" << endl;
}

void monopoly::drawPrompt() {
    Player& player = gs.currentPlayer();
    string posName = gs.initBoard[player.x][player.y].name;
    
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
    drawPrompt();
}

monopoly::Player& monopoly::GameState::currentPlayer() {
    return players[playerIndex];
}

monopoly::GameState::GameState() {
    // convert road to board
    int i, j, k;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 20; j++) {
            initBoard[i][j] = Position("void", i, j);
            board[i][j] = Position("void", i, j);
        }
    }
    for (k = 0; k < road.size(); k++) {
        i = road[k].pos.first;
        j = road[k].pos.second;
        initBoard[i][j] = Position(road[k]);
        board[i][j] = Position(road[k]);
    }
    board[0][0] = Position("player1", 0, 0);
    
    players.push_back(Player("player1"));
    players.push_back(Player("player2"));
    
    players[0].tools.push_back(Tool("红卡"));
    players[0].tools.push_back(Tool("福神卡"));
}

void monopoly::gameLoop() {
    char cmd[10] = "init";
    while (true) {
        controller.eval(cmd);
        drawGame();
        switch (gs.state) {
            case GS::normal:
                if (gs.players.size() == 1) {
                    cout << RED << "= = = = = GAME OVER = = = = =" << endl;
                    cout << "玩家" << gs.currentPlayer() << RED << "获胜" << endl;
                    return;
                }
                break;
            case GS::tool:
                cout << endl;
                controller.showTools();
                cout << endl;
                break;
            case GS::info:
                break;
            case GS::step:
                break;
        }
        cout << gs.message << endl;
        gs.message = "";
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