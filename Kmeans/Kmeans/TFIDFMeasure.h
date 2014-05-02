//
//  TFIDFMeasure.h
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#ifndef __Kmeans__TFIDFMeasure__
#define __Kmeans__TFIDFMeasure__

#include <iostream>
#include <map>
#include "ITokeniser.h"
#include "common.h"

class TFIDFMeasure {
private:
    strVec _docs; //文档集合，每一行字符串代表一份文档
    int _numDocs; //文档数目
    int _numTerms; //单词数目
    strVec _terms; //单词集合
    int2DVec _termFreq; //每个单词出现在每份文档中的频率
    double2DVec _termWeight; //每个单词在每份文档的权重
    intVec _maxTermFreq; //记录每一份文档的最大词频
    intVec _docFreq; //出现单词的文档概率
    ITokeniser *_tokenizer; //分词器
    map<string, int> _wordsIndex; //单词映射表，保存每一个单词及其对应的下标

public:
    TFIDFMeasure(const strVec &document, ITokeniser *tokeniser);
public:
    ~TFIDFMeasure(void);
protected:
    void init(); //初始化tf-idf计算器
    void generateTerms(const strVec &docs, strVec &terms); //分词处理
    void generateTermFrequency(); //计算词频
    void generateTermWeight(); //计算词的权重
    void getWordFrequency(string &input, map<string, int> &freq);
    int countWords(string &word, const strVec &words); //统计词数
    int getTermIndex(const string &term); //查询词语对应的下标
    double computeTermWeight(int term, int doc); //计算词语在指定文档中的权重值
    double getTermFrequency(int term, int doc); //获取词语在指定文档的词频
    double getInverseDocumentFrequency(int term); //计算倒排文件频率
public:
    inline int numTerms() const {
        return this->_numTerms;
    }
    void getTermVector(int doc, doubleVec &vec); //获取项向量
};

#endif /* defined(__Kmeans__TFIDFMeasure__) */
