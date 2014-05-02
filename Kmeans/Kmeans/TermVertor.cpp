//
//  TermVertor.cpp
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include "TermVertor.h"
#include <math.h>

double TermVertor::computeCosineSimilarity(const doubleVec &vector1, const doubleVec &vector2) {
    if (vector1.size() != vector2.size()) {
        throw string("differ lenght");
    }

    double denom = (vectorLength(vector1) * vectorLength(vector2));
    if (denom == 0)
        return 0;
    return (innerProduct(vector1, vector2) / denom);
}

double TermVertor::innerProduct(const doubleVec &vector1, const doubleVec &vector2) {
    if (vector1.size() != vector2.size()) {
        throw string("differ length are not allowed");
    }
    double result = 0.0;
    for (int i = 0; i < vector1.size(); i++) {
        result += vector1[i] * vector2[i];
    }
    return result;
}

double TermVertor::vectorLength(const doubleVec &vector) {
    double sum = 0.0f;
    for (int i = 0; i < vector.size(); i++) {
        sum = sum + (vector[i] * vector[i]);
    }
    return (double)sqrt(sum);
}