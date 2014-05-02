//
//  ITokeniser.h
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#ifndef __Kmeans__ITokeniser__
#define __Kmeans__ITokeniser__

#include <iostream>
#include "common.h"

class ITokeniser {
public:
    virtual void partition(string input, strVec &retWords) = 0; //分词算法
};

#endif /* defined(__Kmeans__ITokeniser__) */
