//
//  GameState.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "GameState.hpp"

namespace monopoly {
    GameState::GameState()
    : today(2016, 5, 1) {
        Land lands[] = { // this is just for initializing, don't do logic on it!!!
            {"land", 0, 0, 0}, {"land", 0, 1, 0}, {"land", 0, 2, 0}, {"coupon", 0, 3, 0}, {"land", 0, 4, 0},
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
        for (int i = 0; i < sizeof(lands)/sizeof(lands[0]); i++) {
            road.push_back(Land(lands[i]));
            streets[road[i].street].push_back(i);
        }
        
        error = false;
        gameover = false;
        message = "";
        errMsg = "";
        playerIndex = 0;
        lastRoll = -1;
    }
    
    Player& GameState::currentPlayer() {
        return players[playerIndex];
    }
    
    Player& GameState::getPlayerByName(string name) {
        for (int i = 0; i < players.size(); i++) {
            if (players[i].name == name) {
                return players[i];
            }
        }
        return players[0]; // this is bad :( , but no fix for now
    }
    
    int GameState::streetPenalty(int streetNum, string owner) {
        int ret = 0;
        int x, y;
        // road 本身的owner信息不可靠, 不用于判断, 只用于存储对应board位置和街道信息
        for (auto i : streets[streetNum]) {
            x = road[i].pos.first;
            y = road[i].pos.second;
            if (board[x][y].owner == owner) {
                ret += road[i].basePrice * road[i].level;
            }
        }
        ret *= 0.1;
        return ret;
    }
}