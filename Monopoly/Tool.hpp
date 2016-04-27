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
    class Tool {
    public:
        string type;
        
        Tool(string);
    };
    ostream& operator <<(ostream&, Tool&);
}

#endif /* Tool_hpp */
