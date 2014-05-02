//
//  main.cpp
//  Bayes
//
//  Created by 陶玮 on 14-4-12.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <stdlib.h>
#include <math.h>
using namespace std;

string posting_list[6][10] = {
    {"my", "dog", "has", "flea", "problems", "help", "please", "null"},
    {"maybe", "not", "take", "him", "to", "dog", "park", "stupid", "null"},
    {"my", "dalmation", "is", "so", "cute", "I", "love", "him", "null"},
    {"stop", "posting", "stupid", "worthless", "garbage", "null"},
    {"mr", "licks", "ate", "my", "steak", "how", "to", "stop", "him", "null"},
    {"quit", "buying", "worthless", "dog", "food", "stupid", "null"}
};

int class_vec[6] = {0, 1, 0, 1, 0, 1}; // 1 is abusive, 0 not

class NaiveBayes {
private:
    vector<vector<string> > list_of_posts; //词条向量
    vector<int> list_classes;
    map<string, int> my_vocab_list; //单词列表
    int *return_vec;
    vector<vector<int> > train_mat;
    vector<float> p0vect;
    vector<float> p1vect;
    float p_abusive;

public:
    NaiveBayes() {
        //posting_list --> list-of-posts
        vector<string> vec;
        for (int i = 0; i < 6; i++) {
            vec.clear();
            for (int j = 0; posting_list[i][j] != "null"; j++) {
                vec.push_back(posting_list[i][j]);
            }
            list_of_posts.push_back(vec);
        }

        //class_vec --> list_classes
        for (int i = 0; i < sizeof(class_vec)/sizeof(class_vec[0]); i++) {
            list_classes.push_back(class_vec[i]);
        }
    }

    void create_vocab_list() {
        vector<vector<string> >::iterator it = list_of_posts.begin();
        int index = 1;
        while (it != list_of_posts.end()) {
            vector<string> vec = *it;
            vector<string>::iterator temp = vec.begin();
            while (temp != vec.end()) {
                if (my_vocab_list[*temp] == 0) {
                    my_vocab_list[*temp] = index++; //index is the location of the vocabulary
                }
                temp++;
            }
            it++;
        }

//        map<string, int>::const_iterator IT = my_vocab_list.begin();
//        while (IT != my_vocab_list.end()) {
//            cout << IT->first << " " << IT->second << " ";
//            IT++;
//        }
//        cout << endl;
    }

    //set some one doc to vec with 0 and 1
    void set_of_words_to_vec(int idx) { //把哪些需要的单词转换成向量
        cout << "set of words to vec begin the document id is : " << idx << endl;
        int len = my_vocab_list.size() + 1;
        return_vec = new int[len]();
        fill(return_vec, return_vec + len, 0);
        for (int i = 0; i < len; i++)
            cout << return_vec[i] << " ";

        for (int i = 0; posting_list[idx][i] != "null"; i++) {
            int pos = my_vocab_list[posting_list[idx][i]];
            if (pos != 0)
                return_vec[pos] = 1;
        }
        cout << endl;
    }

    void get_train_matrix() {
        cout << "get train matrix being : " << endl;
        train_mat.clear();
        for (int i = 0; i < 6; i++) {
            set_of_words_to_vec(i);
            vector<int> vec(return_vec, return_vec + my_vocab_list.size() + 1);
            train_mat.push_back(vec);
            delete []return_vec;
        }
    }//get train matrix

    void print() {
        cout << "print the return_vec begin : " << endl;
        vector<vector<int> >::iterator it = train_mat.begin();
        while (it != train_mat.end()) {
            vector<int> vec = *it;
            vector<int>::iterator IT = vec.begin();
            while (IT != vec.end()) {
                cout << *IT << " ";
                IT++;
            }
            cout << endl;
            it++;
        }
    }

    void train_NB0() {
        int num_train_docs = train_mat.size(); //sizeof(posting_lists)/sizeof(posting_lists[0])
        cout << "num_train_docs" << num_train_docs << endl;
        int num_words = train_mat[0].size() - 1;
        //calculator the sum of the abusive classes
        int sum = accumulate(list_classes.begin(), list_classes.end(), 0);

        cout << "sum = " << sum << endl;
        //float p_abusive = (float)sum/(float)num_train_docs;
        p_abusive = (float)sum / (float)num_train_docs;
        cout << "p_abusive = " << p_abusive << endl;

        //vector<float> p0vect(train_mat[0].size(), 1); //the frequency of each word in non-absusive docs
        p0vect.resize(train_mat[0].size(), 1);
        //vector<float> p1vect(train_mat[0].size(), 1); //the frequency of each word in abusive docs
        p1vect.resize(train_mat[0].size(), 1);
        printf("p0num.size() = %d, p1num.size() = %d\n", p0vect.size(), p1vect.size());
        float p0Demon = 2.0; //the total number of words in non-abusive docs
        float p1Demon = 2.0; //the total number of words in abusive docs

        //calculate the p0num, p1num, p0Denom, p1Denom
        for (int i = 0; i < list_classes.size(); i++) {
            if (list_classes[i] == 1) { //abusive doc
                for (int j = 0; j < p1vect.size(); j++) {
                    p1vect[j] += train_mat[i][j];
                    if (train_mat[i][j] == 1) {
                        p1Demon++;
                    }
                }
            } else { //non-abusive doc
                for (int j = 0; j < p0vect.size(); j++) {
                    p0vect[j] += train_mat[i][j];
                    if (train_mat[i][j] == 1) {
                        p0Demon++;
                    }
                }
            }
        }

        for (int i = 0; i < p1vect.size(); i++) {
            p0vect[i] = log(p0vect[i]/p0Demon);
            p1vect[i] = log(p1vect[i]/p1Demon);
        }

        cout << "print the p0vect values : " << endl;
        for (int i = 0; i < p0vect.size(); i++) {
            cout << p0vect[i] << " ";
        }
        cout << "\nprint the p1vect values : " << endl;
        for (int i = 0; i < p1vect.size(); i++) {
            cout << p1vect[i] << " ";
        }
        cout << endl;
    }

    int classify_NB(string *doc_to_classify) {
        return_vec = new int[my_vocab_list.size() + 1]();
        for (int i = 0; doc_to_classify[i] != "null"; i++) {
            int pos = my_vocab_list[doc_to_classify[i]];
            if (pos != 0) {
                return_vec[pos] = 1;
            }
        }//for

        for (int i = 0; i < my_vocab_list.size(); i++) {
            cout << return_vec[i] << " ";
        }
        cout << endl;
        float p1 = inner_product(p1vect.begin() + 1, p1vect.end(), return_vec + 1, 0) + log(p_abusive);
        float p0 = inner_product(p0vect.begin() + 1, p0vect.end(), return_vec + 1, 0) + log(1 - p_abusive);

        cout << "p1 = " << p1 << endl;
        cout << "p2 = " << p0 << endl;

        if (p1 > p0) {
            return 1;
        } else {
            return 0;
        }
    }
};

/*
 transfer func from docs list to vocabulary list
 */


int main(int argc, const char * argv[])
{
    NaiveBayes nby;
    nby.create_vocab_list();
    //nby.set_of_words_to_vec(5);
    nby.get_train_matrix();
    nby.print();
    nby.train_NB0();

    string doc1_to_classify[] = {"love", "my", "dalmation", "null"};
    string doc2_to_classify[] = {"stupid", "garbage", "null"};
    cout << "doc1 classified as : " << nby.classify_NB(doc1_to_classify) << endl;
    cout << "doc2 classified as : " << nby.classify_NB(doc2_to_classify) << endl;
    return 0;
}

