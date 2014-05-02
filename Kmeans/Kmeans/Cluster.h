//
//  Cluster.h
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#ifndef __Kmeans__Cluster__
#define __Kmeans__Cluster__

#include <iostream>
#include "common.h"

class Cluster {
public:
    intVec currentMembership; //该聚类的数据成员索引
    doubleVec Mean; //该聚类的中心
public:
    Cluster(void);
    Cluster(int dataindex, doubleVec &data);
    void updateMean(double2DVec &coordinates);
public:
    ~Cluster(void);
};

#endif /* defined(__Kmeans__Cluster__) */
