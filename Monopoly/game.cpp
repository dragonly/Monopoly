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
    d[0] = 2016; d[1] = 1; d[2] = 1;
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

ostream& monopoly::operator <<(ostream& os, Player& player) {
    os << player.name;
    return os;
}

monopoly::Controler::Controler() {
    players.push_back(Player("1P"));
    players.push_back(Player("2P"));
}

monopoly::Player& monopoly::Controler::currentPlayer() {
    return players[curPlayer];
}

void monopoly::Controler::nextTurn() {
    curPlayer = (curPlayer + 1) % players.size();
}

void monopoly::Controler::eval(char* cmd) {
    if (strcmp(cmd, "t") == 0) {
        
    }
    else if (strcmp(cmd, "s") == 0) {
        
    }
    else if (strcmp(cmd, "i") == 0) {
        
    }
    else if (strcmp(cmd, "r") == 0) {
        
    }
    else if (strcmp(cmd, "gg") == 0) {
        
    }
    else {
        
    }
}

void monopoly::init() {
}

void monopoly::drawMap() {
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

void monopoly::drawPrompt() {
    cout << "今天是" << monopoly::today << endl;
    cout << "现在是玩家 " << GREEN << controler.currentPlayer() << NC << " 的回合, ";
    cout << PURPLE << (controler.currentPlayer().direction ? "顺" : "逆") << NC << "时针" << endl;
    cout << "你可以做:" << endl
         << "t(tool) - 道具列表" << endl
         << "s(step) - 查看前后若干步的具体信息" << endl
         << "i(info) - 查看玩家资产信息" << endl
         << "r(roll) - 掷骰子" << endl
         << "gg(GG)  - 是在下输了" << endl;
}

void monopoly::drawGame() {
    drawMap();
    drawPrompt();
}

void monopoly::gameLoop() {
    char cmd[10] = "init";
    while (true) {
        system("clear");
        
        drawGame();
        controler.eval(cmd);
        
        cout << endl << PURPLE << cmd << NC << endl;
        cout << "> ";
        cin >> cmd;
    }
}

monopoly::GameState::GameState() {
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