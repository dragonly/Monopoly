//
//  Tool.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Tool.hpp"

namespace monopoly {
    Tool::Tool(string type): type(type) {
    }
    
    ostream& operator <<(ostream& os, Tool& tool) {
        // TODO
        return os;
    }
}