//
//  TFIDFMeasure.cpp
//  Kmeans
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include "TFIDFMeasure.h"
#include <limits>
#include <iostream>
#include <math.h>
using namespace std;

TFIDFMeasure::~TFIDFMeasure() {
    //销毁分词器
    if (this->_tokenizer != NULL) {
        delete _tokenizer;
        _tokenizer = NULL;
    }
    //清空数据
    _docs.clear();
    _terms.clear();
    _wordsIndex.clear();
}

TFIDFMeasure::TFIDFMeasure(const strVec &documents, ITokeniser *tokeniser) {
    _docs = documents;
    _numDocs = documents.size();
    _tokenizer = tokeniser;
    this->init();
}

void TFIDFMeasure::generateTerms(const strVec &docs, strVec &terms) {
    for (int i = 0; i < docs.size(); i++) {
        strVec words;
        _tokenizer->partition(docs[i], words); //这里最后可以加入自己的分词系统
        for (int j = 0; j < words.size(); j++) {
            //不在单词表中，则加入
            if (find(terms.begin(), terms.end(), words[j]) == terms.end()) {
                terms.push_back(words[j]);
            }
        }
    }
}

void TFIDFMeasure::init() {
    //初始化
    this->generateTerms(_docs, _terms); //分出所有词项
    this->_numTerms = _terms.size(); //所有文档中的词项数目
    //准备好存储空间
    _maxTermFreq.resize(_numDocs);
    _docFreq.resize(_numTerms);
    _termFreq.resize(_numTerms);
    _termWeight.resize(_numTerms);

    for (int i = 0; i < _terms.size(); i++) {
        _termWeight[i].resize(_numDocs);
        _termFreq[i].resize(_numDocs);
        _wordsIndex[_terms[i]] = i; //将单词放入单词映射表中，_wordsIndex的下标代表的是单词
    }
    this->generateTermFrequency(); //计算单词频率
    this->generateTermWeight(); //计算单词权重
}

void TFIDFMeasure::getWordFrequency(string &input, map<string, int> &freq) {
    //计算单词频率
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    strVec temp;
    this->_tokenizer->partition(input, temp); //对当前文档分词
    unique(temp.begin(), temp.end());
    strVec::iterator iter;
    for (iter = temp.begin(); iter != temp.end(); iter++) {
        int count = countWords(*iter, temp); //计算单词在文档中出现的次数
        freq[*iter] = count; //保存单词频率
    }
}

void TFIDFMeasure::getTermVector(int doc, doubleVec &vec) {
    vec.resize(this->_numTerms);
    for (int i = 0; i < this->_numTerms; i++) {
        vec[i] = _termWeight[i][doc]; //第i个单词在文档doc中的权重
    }
}

//用于字符串比较的仿函数
class WordComp {
public:
    WordComp(string &sWord): word(sWord) {
    }
    bool operator() (const string &lhs) {
        return lhs.compare(word) == 0;
    }
private:
    string word;
};

int TFIDFMeasure::countWords(string &word, const strVec &words) {
    int nCount = 0;
    nCount = count_if(words.begin(), words.end(), WordComp(word));
    return nCount;
}

int TFIDFMeasure::getTermIndex(const string &term) {
    map<string, int>::iterator pos = _wordsIndex.find(term);
    if (pos != _wordsIndex.end()) {
        return pos->second;
    }
    return -1;
}

void TFIDFMeasure::generateTermFrequency() {
    //计算每个单词在每份文档出现的频率
    for (int i = 0; i < _numDocs; i++) {
        string curDoc = _docs[i]; //当前待处理的文档
        map<string, int> freq;
        this->getWordFrequency(curDoc, freq);
        map<string, int>::iterator it;
        _maxTermFreq[i] = numeric_limits<int>::min();
        for (it = freq.begin(); it != freq.end(); it++) {
            string word = it->first;
            int wordFreq = it->second;
            int termIndex = getTermIndex(word); //单词下标
            if (termIndex == -1)
                continue;
            _termFreq[termIndex][i] = wordFreq; //单词在第i份文档中出现的频率
            _docFreq[termIndex]++; //出现第termIndex单词的文档频率加1

            if (wordFreq > _maxTermFreq[i]) {
                _maxTermFreq[i] = wordFreq; //记录第i份文档中的最大词频
            }
        }
    }
}

void TFIDFMeasure::generateTermWeight() {
    //计算每个单词在每份文档中的权重
    for (int i = 0; i < _numTerms; i++) {
        for (int j = 0; j < _numDocs; j++) {
            _termWeight[i][j] = computeTermWeight(i, j);
        }
    }
}

double TFIDFMeasure::getTermFrequency(int term, int doc) {
    int freq = _termFreq[term][doc]; //词频
    int maxfreq = _maxTermFreq[doc];
    return ((float)freq / (float)maxfreq);
}

double TFIDFMeasure::computeTermWeight(int term, int doc) {
    //计算单词在文档中的权重
    float tf = getTermFrequency(term, doc);
    float idf = getInverseDocumentFrequency(term);
    return tf * idf;
}

double TFIDFMeasure::getInverseDocumentFrequency(int term) {
    int df = _docFreq[term]; //包含单词term的文档数目
    return log((float)(_numDocs) / (float)df);
}