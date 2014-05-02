//
//  main.cpp
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "common.h"
#include <algorithm>
#include "TFIDFMeasure.h"
#include "Tokeniser.h"
#include "StopWordsHandler.h"
#include "KMeans.h"
#include "Cluster.h"
using namespace std;

int main(int argc, const char * argv[])
{
    //读入所有文档数据
    strVec strvec;
    ifstream infile("/Users/taowei/Documents/ml/Kmeans/Kmeans/data.txt");
    string tempStr;
    while (getline(infile, tempStr)) {
        strvec.push_back(tempStr);
    }
    //初始化tf－idf计算器，用来生产每个文档的tf-idf权重
    TFIDFMeasure tf(strvec, new Tokeniser());

    int docCount = strvec.size(); //文档个数
    int k = 3; //聚成3个聚类

    //生成k-means的输入数据，是一个联合数组，第一维表示文档个数
    //第二维表示所有文档分出来的所有词
    double2DVec data;
    data.resize(docCount);
    int dimension = tf.numTerms(); //所有词的数目
    for (int i = 0; i < docCount; i++) {
        for (int j = 0; j < dimension; j++) {
            tf.getTermVector(i, data[i]); //获取第i个文档的tfidf权重向量
        }
    }
    //初始化k－means算法，第一个参数表示输入数据，第二个参数表示要聚成几个类
    KMeans kmeans(data, k);
    //开始迭代
    kmeans.start();
    //获取聚类结果并输出
    vector<Cluster> clusters = kmeans._clusters;
    vector<Cluster>::iterator it;
    intVec::iterator iter;
    for (it = clusters.begin(); it != clusters.end(); it++) {
        cout << "-----------------------------------------" << endl;
        intVec &vec = it->currentMembership;
        for (iter = vec.begin(); iter != vec.end(); iter++) {
            cout << strvec[*iter] << endl;
        }
    }
    return 0;
}

