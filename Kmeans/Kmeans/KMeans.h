//
//  KMeans.h
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#ifndef __Kmeans__KMeans__
#define __Kmeans__KMeans__

#include <iostream>
#include "common.h"

class Cluster;

class KMeans {
public:
    vector<Cluster> _clusters; //聚类
private:
    int _coordCount; //数据的数量
    double2DVec _coordinates; //原始数据
    int _k; //聚类的数量
    //定义一个变量用于记录和跟踪每个资料点属于哪个群聚类
    //_clusterAssignments[j] = i;表示第j个资料点对象属于第i个群聚类
    intVec _clusterAssignments;
    //定义一个变量用于记录和跟踪每个资料点离聚类最近
    intVec _nearestCluster;
    //定义一个变量，来表示资料点到中心点的距离，其中_distanceCache[i][j]表示第i个资料点到第j个群聚对象中心点的距离
    double2DVec _distanceCache;
    void initRandom();
    static double getDistance(const doubleVec &coord, const doubleVec &center);
    int nearestCluster(int ndx);
public:
    KMeans(double2DVec &data, int k);
    void start();
public:
    ~KMeans(void);
};

#endif /* defined(__Kmeans__KMeans__) */
