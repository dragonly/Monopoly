//
//  Tool.hpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#ifndef Tool_hpp
#define Tool_hpp

#include <iostream>

using std::string;
using std::ostream;

namespace monopoly {
    
enum class ToolType {
        TURNING_CARD, // turn around immediately
        STAY_CARD, // stay for one turn, trigger event in this position
        TURTLE_CARD, // make another player can only go 1 step in 3 turns
        MAGIC_DICE, // make a roll number as you want
        ROADBLOCK, // can be placed in 8 steps, make anyone passing by stop
        BUY_CARD, // buy the other player's land in this position
        TAX_CARD, // make the other player lose 30% deposit
        AVERAGE_CARD, // make all players' cash averaged
        LAND_CARD, // in 5 turns, make land your own in your position
        REMOVE_CARD, // remove all house in this street, owners get 150% of current price
        MONSTER_CARD, // make all house 1 level in this street
        MONEY_CARD, // get 10,000 cash immediately, won't pay on others' land in 8 turns
        FORTUNE_CARD, // ge't a random card immediately, won't pay on others' land in 8 turns
        LOTTERY_CARD, // manipulate result of lottery this month
        RED_CARD, // make stock rise 10% for 2 days
        BLACK_CARD // make stock fall 10% for 2 days
    };
    
    class Tool {
    public:
        ToolType type;
        string name;
        
        Tool(ToolType, string);
    };
    ostream& operator <<(ostream&, Tool&);
}

#endif /* Tool_hpp */
