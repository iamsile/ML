//
//  Cluster.cpp
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include "Cluster.h"

Cluster::Cluster(void) {
}

Cluster::Cluster(int dataindex, doubleVec &data) {
    currentMembership.push_back(dataindex);
    copy(data.begin(), data.end(), back_inserter(Mean));
}

//计算聚类对象的均值
void Cluster::updateMean(double2DVec &coordinates) {
    //根据mcurrentMembership取得原始资料点对象coord，该对象是coordinates的一个子集；
    //然后取出该子集的均值，取均值的算法很简单，可以把coordinates想像成一个m＊n的矩阵，
    //每个均值就是每个纵向列的取和平均值，该值保存在mcenter中

    for (int i = 0; i < currentMembership.size(); i++) {
        doubleVec &coord = coordinates[currentMembership[i]];
        for (int j = 0; j < coord.size(); j++) {
            Mean[j] += coord[j]; //得到每个纵向列的和
        }
        for (int k = 0; k < Mean.size(); k++) {
            Mean[k] /= coord.size(); //对每个纵向列取平均值
        }
    }
}

Cluster::~Cluster(void) {
}