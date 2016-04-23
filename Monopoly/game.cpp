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

void monopoly::Controler::nextTurn() {
    gs.playerIndex = (gs.playerIndex + 1) % gs.players.size();
}

void monopoly::Controler::eval(char* cmd) {
    gs.error = false;
    if (strcmp(cmd, "t") == 0) {
        cout << "您现在拥有的道具如下:" << endl;
        controler.showTools();
    }
    else if (strcmp(cmd, "s") == 0) {
        
    }
    else if (strcmp(cmd, "i") == 0) {
        
    }
    else if (strcmp(cmd, "r") == 0) {
        gs.lastRoll = static_cast<int>(rand() % 6) + 1;
        controler.movePlayerWithAnimation(gs.lastRoll);
    }
    else if (strcmp(cmd, "gg") == 0) {
        
    }
    else {
        gs.error = true;
    }
}

void monopoly::Controler::movePlayerWithAnimation(int delta) {
    for (int i = 0; i < delta; i++) {
        cout << i << endl;
        movePlayer(1);
        drawGame();
        if (i != delta - 1) usleep(80000);
    }
}

void monopoly::Controler::movePlayer(int delta) {
    int curX, curY, preX, preY;
//    string direction;
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
//    direction = player.direction ? string("R") : string("L");
    gs.board[curX][curY] = Position(player.name, curX, curY);
    gs.board[preX][preY] = Position(gs.initBoard[preX][preY]);
    player.x = curX;
    player.y = curY;
}

void monopoly::Controler::showTools() {
    Player& player = gs.currentPlayer();
    for (int i = 0; i < player.tools.size(); i++) {
        cout << i << ". " << player.tools[i].type << "   ";
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

void monopoly::gameLoop() {
    char cmd[10] = "init";
    while (true) {
        controler.eval(cmd);
        drawGame();
        
        if (gs.error)
            cout << endl << RED << gs.errMsg << NC << endl;
        if (gs.lastRoll != -1) cout << "你刚刚掷出了 " << LCYAN << gs.lastRoll << NC << endl;
        cout << "> ";
        cin >> cmd;
        gs.today.nextDay();
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
    players.push_back(Player("palyer2"));
}