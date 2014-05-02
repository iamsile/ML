//
//  TermVertor.h
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#ifndef __Kmeans__TermVertor__
#define __Kmeans__TermVertor__

#include <iostream>
#include "common.h"

class TermVertor {
public:
    static double computeCosineSimilarity(const doubleVec &vector1, const doubleVec &vector2);
    static double innerProduct(const doubleVec &vector1, const doubleVec &vector2);
    static double vectorLength(const doubleVec &vector);
};

#endif /* defined(__Kmeans__TermVertor__) */
