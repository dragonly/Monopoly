//
//  Stock.hpp
//  Monopoly
//
//  Created by Dragonly on 4/30/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef Stock_hpp
#define Stock_hpp

#include <string>
#include <vector>
#include <map>
#include <cstdlib>

namespace monopoly {
    
    using std::string;
    using std::vector;
    using std::map;
    
    class Stock {
    public:
        string serial;
        string name;
        double price;
        double ratio;
        
        Stock(string, string, double);
    };
    
    class StockMarket {
    public:
        vector<Stock> stocks;
        map<string, map<string, int>> playerMap;
        
        StockMarket();
        Stock find(string);
        void buy(string, string, int);
        void sell(string, string, int);
        void update();
    };
    
}
#endif /* Stock_hpp */
