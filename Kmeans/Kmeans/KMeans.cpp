//
//  KMeans.cpp
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include "KMeans.h"
#include <time.h>
#include "Cluster.h"
#include "TermVertor.h"
#include <limits>

KMeans::KMeans(double2DVec &data, int k) {
    this->_coordinates.resize(data.size());
    for (int i = 0; i < data.size(); i++) {
        copy(data[i].begin(), data[i].end(), back_inserter(_coordinates[i]));
    }
    _coordCount = data.size();
    _k = k;
    _clusters.resize(k);
    _clusterAssignments.resize(_coordCount);
    _nearestCluster.resize(_coordCount);
    _distanceCache.resize(_coordCount);
    for (int i = 0; i < _coordCount; i++) {
        _distanceCache[i].resize(_coordCount);
    }
    initRandom();
}

void KMeans::initRandom() {
    srand(unsigned(time(NULL)));
    for (int i = 0; i < _k; i++) {
        int temp = rand() % (_coordCount); //产生随机数
        _clusterAssignments[temp] = i; //记录第temp个资料属于第i个聚类
        _clusters[i] = *new Cluster(temp, _coordinates[temp]);
    }
}

void KMeans::start() {
    int iter = 0, i, j;
    while (true) {
        cout << "Iteration " << iter++ << "...\n";
        //重新计算每个聚类的均值
        for (i = 0; i < _k; i++) {
            _clusters[i].updateMean(_coordinates);
        }
        //计算每个数据和每个聚类中心的距离
        for (i = 0; i < _coordCount; i++) {
            for (j = 0; j < _k; j++) {
                double dist = getDistance(_coordinates[i], _clusters[j].Mean);
                _distanceCache[i][j] = dist;
            }
        }

        //计算每个数据离哪个聚类最近
        for (i = 0; i < _coordCount; i++) {
            _nearestCluster[i] = this->nearestCluster(i);
        }

        //比较每个数据最近的聚类是否就是它所属的聚类，如果全相等表示所有的点已经是最佳距离了，直接返回
        int k = 0;
        for (i = 0; i < _coordCount; i++) {
            if (_nearestCluster[i] == _clusterAssignments[i]) {
                k++;
            }
        }
        if (k == _coordCount) {
            break;
        }
        //否则需要重新调整资料点和群聚类的关系，调整完毕后再重新开始循环；需要修改每个聚类的成员和表示某个数据属于哪个聚类的变量
        for (j = 0; j < _k; j++) {
            _clusters[j].currentMembership.clear();
        }
        for (i = 0; i < _coordCount; i++) {
            _clusters[_nearestCluster[i]].currentMembership.push_back(i);
            _clusterAssignments[i] = _nearestCluster[i];
        }
    }
}

double KMeans::getDistance(const doubleVec &coord, const doubleVec &center) {
    return 1 - TermVertor::computeCosineSimilarity(coord, center);
}

int KMeans::nearestCluster(int ndx) {
    int nearest = -1;
    double min = numeric_limits<double>::max();
    for (int c = 0; c < _k; c++) {
        double d= _distanceCache[ndx][c];
        if (d < min) {
            min = d;
            nearest = c;
        }
    }
    return nearest;
}

KMeans::~KMeans() {
    vector<Cluster>::iterator it;
//    for (it = this->_clusters.begin(); it != _clusters.end(); it++) {
//        delete (it);
//    }
    _clusters.clear();
}

