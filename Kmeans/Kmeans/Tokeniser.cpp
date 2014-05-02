//
//  Tokeniser.cpp
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include "Tokeniser.h"
#include "StopWordsHandler.h"

using namespace std;

Tokeniser::Tokeniser(void) {
}

Tokeniser::~Tokeniser(void) {
}

void Tokeniser::partition(string input, strVec &retWords) {
    //分词算法，input为输入库，retWords为处理后所分开的单词，这里就简单化处理了，以空格符为分隔符进行分词
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    string::iterator pos = input.begin();
    StopWordsHandler stopHandler;
    do {
        string temp;
        pos = find(input.begin(), input.end(), ' '); //找到分隔符
        copy(input.begin(), pos, back_inserter(temp));
        if (!stopHandler.isStopWord(temp)) {
            //不是停用词则保存
            retWords.push_back(temp); //保存分出的单词
        }
        if (pos == input.end()) {
            //最后一个单词
            break;
        } else {
            input.erase(input.begin(), ++pos);
        }
    } while (pos != input.end());
}