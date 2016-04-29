//
//  Tool.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Tool.hpp"

namespace monopoly {
    Tool::Tool(ToolType type, string name): type(type), name(name) {
    }
    
    ostream& operator <<(ostream& os, Tool& tool) {
        // TODO
        return os;
    }
}