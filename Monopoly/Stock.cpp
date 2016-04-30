//
//  Stock.cpp
//  Monopoly
//
//  Created by Dragonly on 4/30/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Stock.hpp"

namespace monopoly {
    
    Stock::Stock(string serial, string name, double price): serial(serial), name(name), price(price) {
    }
    
    StockMarket::StockMarket() {
        stocks.push_back(Stock("1", "A", 1));
        stocks.push_back(Stock("2", "B", 2));
        stocks.push_back(Stock("3", "C", 3));
        stocks.push_back(Stock("4", "D", 4));
        stocks.push_back(Stock("5", "E", 5));
        stocks.push_back(Stock("6", "F", 6));
        stocks.push_back(Stock("7", "G", 7));
        stocks.push_back(Stock("8", "H", 8));
        stocks.push_back(Stock("9", "I", 9));
        stocks.push_back(Stock("a", "J", 10));
    }
    
    void StockMarket::buy(string playerName, string serial, int n) {
        if (playerMap.find(playerName) == playerMap.end())
        {
            playerMap[playerName] = map<string, int>();
        }
        map<string, int>::iterator stockEntry = playerMap[playerName].find(serial);
        if (stockEntry == playerMap[playerName].end()) {
            playerMap[playerName][serial] = 0;
        }
        playerMap[playerName][serial] += n;
    }
    
    void StockMarket::sell(string playerName, string serial, int n) {
        if (playerMap.find(playerName) == playerMap.end()) return;
        if (playerMap[playerName].find(serial) == playerMap[playerName].end()) return;
        playerMap[playerName][serial] -= n;
        if (playerMap[playerName][serial] < 0) playerMap[playerName][serial] = 0;
    }
}