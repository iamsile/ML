//
//  StopWordsHandler.cpp
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include "StopWordsHandler.h"
using namespace std;

string stopWordsList[] = {"的", "我","们","要","自己","之","将","”","“","，","（","）","后","应","到","某","后","个","是","位","新","一","两","在","中","或","有","更","好",""};//停用词
int stopWordsLen = sizeof(stopWordsList) / sizeof(stopWordsList[0]);

StopWordsHandler::StopWordsHandler(void) {
    for (int i = 0; i < stopWordsLen; i++) {
        stopWords.push_back(stopWordsList[i]);
    }
}

StopWordsHandler::~StopWordsHandler() {
}

bool StopWordsHandler::isStopWord(string &str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower); //确保小写化
    return find(stopWords.begin(), stopWords.end(), str) != stopWords.end();
}