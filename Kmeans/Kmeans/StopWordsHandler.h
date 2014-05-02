//
//  StopWordsHandler.h
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#ifndef __Kmeans__StopWordsHandler__
#define __Kmeans__StopWordsHandler__

#include <iostream>
#include "common.h"

//用于移除停用词
class StopWordsHandler {
public:
    StopWordsHandler(void);
    ~StopWordsHandler(void);
    bool isStopWord(string &str);
private:
    strVec stopWords;
};
#endif /* defined(__Kmeans__StopWordsHandler__) */
