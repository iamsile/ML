//
//  Tokeniser.h
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#ifndef __Kmeans__Tokeniser__
#define __Kmeans__Tokeniser__

#include <iostream>
#include "ITokeniser.h"

class Tokeniser: public ITokeniser {
public:
    Tokeniser(void);
    ~Tokeniser(void);
public:
    void partition(string input, strVec &reWords);
};
#endif /* defined(__Kmeans__Tokeniser__) */
