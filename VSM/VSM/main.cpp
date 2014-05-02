//
//  main.cpp
//  VSM
//
//  Created by 陶玮 on 14-4-1.
//
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
using namespace std;

//定义常量
const int dictionaryMaxSize = 500; //特征词典大小的最大值
const int documentMaxSize = 100; //文档个数的最大值

//定义文件对象
fstream fin, fout; //文件读写对象

//子操作，定义为外部函数
char *getTerm(char ch); //特征词获取
char *getType(char ch); //特征词词性获取

//特征词
struct term {
    int termID; //特征词标号
    char t[10]; //特征词
    char type[5]; //特征词的词性
};

//类，特征词典

class termDictionary {
public:
    struct term termArray[dictionaryMaxSize]; //特征词标号
    int dictionarySize; //特征词典大小
    termDictionary() {
        dictionarySize = 0;
    }
    void createDictionary(); //特征词典生成
}TD;

//特征词典生成
void termDictionary::createDictionary() {
    fin.open("文档集.txt", ios::in);
    if (!fin) {
        cout << "打不开文件－－文档集.txt!\n";
        exit(0);
    }
    
    char ch, str1[10], str2[5];
    char type[50][5] = {"a", "ad", "Ag", "b", "c", "d", "e", "m", "ms", "o", "p", "q", "u", "w", "y", "z", ""}; //不纳入特征词典的词性
    int id = 0, flag1 = 0, flag2 = 0;
    
    ch = fin.get();
    while (!fin.eof()) {
        if (ch == '\n') {
            ch = fin.get();
            continue;
        }
        strcpy(str1, getTerm(ch));
        for (int i = 0; i < dictionarySize; i++) { //提取特征词后，扫描特征词典，若词典中存在此特征词，则置标志为1
            if (!strcmp(TD.termArray[i].t, str1)) {
                flag1 = 1;
                break;
            }
        }
        ch = fin.get();
        strcpy(str2, getType(ch));
        for (int i = 0; i < 50; i++) { //提取特征词词性以后，扫描不纳入特征词典的词性数组，若存在词性，则置标志位为1
            if (!strcmp(str2, type[i])) {
                flag2 = 1;
                break;
            }
        }
        if (!flag1 && !flag2) { //若两个标志位均未0，则把特征词id,特征词，词性，插入特征词典
            TD.termArray[dictionarySize].termID = id;
            strcpy(TD.termArray[dictionarySize].t, str1);
            strcpy(TD.termArray[dictionarySize].type, str2);
            dictionarySize++;
            id++;
        }
        flag1 = 0;
        flag2 = 0;
        ch = fin.get();
        ch = fin.get();
    }
    fin.close();
    fout.open("特征词典.txt", ios::out);
    if (!fout) {
        cout << "打不开特征词典.txt!\n";
        exit(0);
    }
    for (int i = 0; i < dictionarySize; i++) { //把特征词典输出到文件－特征词典.txt
        fout << TD.termArray[i].termID << "     " << termArray[i].t << "        " << TD.termArray[i].type << endl;
    }
    fin.close();
}

//索引项
struct index {
    int termID; //特征词标号
    int TF[documentMaxSize]; //词频数组
    int ni; //词i在段落中出现的段落数目
};

//类、倒排索引
class indexTable {
public:
    struct index indexArray[dictionaryMaxSize]; //索引项数组
    int documentSize; //文档个数
    indexTable() {
        documentSize = 0;
    }
    void createIndexTable(); //倒排索引表生成
}IT;

//倒排索引表生成
void indexTable::createIndexTable() {
    char ch;
    int line = 0;
    for (int i = 0; i < dictionaryMaxSize; i++) {
        for (int j = 0; j < documentMaxSize; j++) {
            IT.indexArray[i].TF[j] = 0; //给词频矩阵赋初值
        }
    }
    fin.open("文档集.txt", ios::in);
    if (!fin) {
        cout << "打不开文件－－文档集\n";
        exit(0);
    }
    ch = fin.get(); //计算document的数目
    while (!fin.eof()) {
        if (ch == '\n') {
            IT.documentSize++;
        }
        ch = fin.get();
    }
    IT.documentSize++;
    fin.close();
    //cout << IT.documentSize << endl;
    //cout << TD.dictionarySize << endl;
    for (int i = 0; i < TD.dictionarySize; i++) { //计算词频TF
        fin.open("文档集.txt", ios::in);
        if (!fin) {
            cout << "打不开文件－－文档集.txt!\n";
            exit(0);
        }
        IT.indexArray[i].termID = TD.termArray[i].termID;
//        cout << fin.tellg() << endl;
//        fin.seekg(0, ios::beg);
//        cout << fin.tellg() << endl;
        ch = fin.get();
        while (!fin.get()) { //若读到的字符为文本结束符，则扫描结束
            if (ch == '\n') { //若读到的字符为回车符，则段落数增1，扫描下一个字符
                line++;
                ch = fin.get();
            }
            if (!strcmp(TD.termArray[i].t, getTerm(ch))) {
                IT.indexArray[i].TF[line]++; //若特征词典中当前获取特征词和此特征词相等，则TF[][]加1
            }
            ch = fin.get();
            getType(ch);
            ch = fin.get();
            ch = fin.get();
        }
        line = 0;
        fin.close();
    }
    fin.close();
    for (int i = 0; i < TD.dictionarySize; i++) {
        IT.indexArray[i].ni = 0;
    }
    //cout << TD.dictionarySize << endl;
    //cout << IT.documentSize << endl;
    for (int i = 0; i < TD.dictionarySize; i++) { //计算ni，扫描倒排索引表，对于每一个特征词，统计大于0的词频个数
        for (int j = 0; j < IT.documentSize; j++) {
            if (IT.indexArray[i].TF[j]) {
                IT.indexArray[i].ni++;
            }
        }
    }
    fout.open("倒排索引表.txt", ios::out);
    if (!fout) {
        cout << "打不开文件－－倒排索引表.txt\n";
        exit(0);
    }
    char s[100];
    for (int i = 0; i < TD.dictionarySize; i++) { //把特征词id，词频矩阵，ni输出到文件－倒排索引表.txt
        sprintf(s, "%3d", IT.indexArray[i].termID);
        fout << s;
        //fout << IT.indexArray[i].termID << "        ";
        for (int j = 0; j < IT.documentSize; j++) {
            sprintf(s, "%5d", IT.indexArray[i].TF[j]);
            fout << s;
            //fout << IT.indexArray[i].TF[j] << "     ";
        }
        sprintf(s, "%5d", IT.indexArray[i].ni);
        fout << s << endl;
        //fout << IT.indexArray[i].ni << endl;
    }
    fout.close();
}

//类、向量空间模型
class vectorSpaceModel {
public:
    double vector[dictionaryMaxSize][documentMaxSize]; //向量矩阵
    void createVSM(); //向量空间模型生成
    double *getVector(int j); //获取第j个特征向量
    void printVector(int j); //打印第j个特征向量
}VSM;

//向量空间模型生成
void vectorSpaceModel::createVSM() {
    char s[100];
    double IDF[dictionaryMaxSize];
    for (int i = 0; i < TD.dictionarySize; i++) { //计算idfi ＝ log(N/ni)
        IDF[i] = log((double)IT.documentSize/(double)IT.indexArray[i].ni/log(10));
        //printf("%.20f\n",IDF[i]);
    }
    double TFIDF[dictionaryMaxSize][documentMaxSize];
    double squ[documentMaxSize]; //向量元素平方和
    double mod[documentMaxSize]; //向量模
    
    for (int j = 0; j < IT.documentSize; j++) {
        squ[j] = 0;
        for (int i = 0; i < TD.dictionarySize; i++) {
            TFIDF[i][j] = (double)IT.indexArray[i].TF[j] * IDF[i]; //计算tfidf[i][j] = f[j] * log(N/ni)
            squ[j] += TFIDF[i][j] * TFIDF[i][j]; //计算向量元素平方和
        }
        mod[j] = sqrt(squ[j]); //计算向量模
    }
    //生成文件－－向量空间模型.txt
    fout.open("向量空间模型.txt", ios::out);
    if (!fout) {
        cout << "打不开文件－－向量空间模型.txt!\n";
        exit(0);
    }
    for (int i = 0; i < TD.dictionarySize; i++) { //计算并打印向量空间模型
        for (int j = 0; j < IT.documentSize; j++) {
            VSM.vector[i][j] = TFIDF[i][j]/mod[j]; //计算新的向量元素
            sprintf(s, "%15.10f", VSM.vector[i][j]);
            fout << s;
            //fout <<  "        " << VSM.vector[i][j] << "  ";
        }
        fout << endl;
    }
    fout.close();
    //生成文件--vsm.arff
    fout.open("vms.arff", ios::out);
    if (!fout) {
        cout << "打不开文件--vsm.arff!\n";
        exit(0);
    }
    fout << "@relation" << "        " << "vsm" << endl;
    for (int i = 0; i < TD.dictionarySize; i++) {
        fout << "@attribute" << "   " << "term" << endl;
        if (i < 9) {
            fout << "   ";
        } else if (i < 99)
            fout << "   ";
        fout << "   " << "real" << endl;
    }
    fout << "@data" << endl;
    for (int j = 0; j < IT.documentSize; j++) {
        for (int i = 0; i < TD.dictionarySize; i++) {
            sprintf(s, "%6.6f", VSM.vector[i][j]);
            fout << s;
            if (i < TD.dictionarySize - 1) {
                fout << ',';
            }
            //fout << "       " << VSM.vector[i][j] << "  ";
        }
        fout << endl;
    }
    fout.close();
}

//获取第j个特征向量
double *vectorSpaceModel::getVector(int j) {
    double p[dictionaryMaxSize], *q;
    q = p;
    for (int k = 0; k < dictionaryMaxSize; k++) {
        p[k] = VSM.vector[k][j-1]; //把特征向量存入p
    }
    return q;
}

//打印第j个特征向量
void vectorSpaceModel::printVector(int j) {
    char s[100];
    for (int k = 0; k < TD.dictionarySize; k++) {
        sprintf(s, "%10.10f", VSM.vector[k][j-1]); //打印特征向量
        cout << s << endl;
    }
}

//子操作，定义为外部函数
char *getTerm(char ch) { //提取term的内容
    int i = 0;
    char str[10], *p;
    while (ch != '/') { //把单个字符存入数组，直到碰到'/'
        str[i++] = ch;
        ch = fin.get();
    }
    str[i] = '\0';
    p = str;
    return p;
}

char *getType(char ch) { //提取term的词性
    int i = 0;
    char str[5], *p;
    while (ch != EOF && ch != ' ') { //把单个字符存入数组，直到碰到' '
        str[i++] = ch;
        ch = fin.get();
    }
    str[i] = '\0';
    p = str;
    return p;
}

int main(int argc, const char * argv[])
{
    TD.createDictionary(); //特征词典生成
    IT.createIndexTable(); //倒排索引表生成
    VSM.createVSM(); //向量空间模型生成
    //VSM.printVector(3); //打印第三个向量
    return 0;
}

