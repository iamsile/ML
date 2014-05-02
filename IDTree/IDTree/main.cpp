//
//  main.cpp
//  IDTree
//
//  Created by 陶玮 on 14-4-13.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//
/*
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <math.h>
using namespace std;

#define raw 14
#define col 6

const int dataRaw = raw;
const int dataColumn = col;

struct Node {
    double value; //代表此时yes的概率
    int attrid;
    Node *parentNode;
    vector<Node *> childNode;
};

string dataTable[dataRaw][dataColumn];
map<string, int> str2int;
set<int> S;
set<int> attributes;
string attrName[dataColumn] = {"Day", "Outlook", "Temperature", "Humidity", "Wind", "PlayTennis"};
string attrValue[dataColumn][dataRaw] = {
    {}, //d1，d2这个属性不需要
    {"Sunny", "Overcast", "Rain"},
    {"Hot", "Mild", "Cool"},
    {"High", "Normal"},
    {"Weak", "Strong"},
    {"No", "Yes"}
};
int attrCount[dataColumn] = {14, 3, 3, 2, 2, 2};

void init() {
    ifstream infile("/Users/taowei/Documents/ml/IDTree/data.txt");
    for (int i = 0; i < raw; i++) {
        for (int j = 0; j < col; j++) {
            infile >> dataTable[i][j];
        }
    }
    infile.close();
    for (int i = 1; i <= 5; i++) {
        str2int[attrName[i]] = i;
        for (int j = 0; j < attrCount[i]; j++) {
            str2int[attrValue[i][j]] = j;
        }
    }
    for (int i = 0; i < dataRaw; i++) {
        S.insert(i);
    }
    for (int i = 1; i <= 4; i++) {
        attributes.insert(i);
    }
}

double lg2(double n) {
    return log(n) / log(2);
}

//决策树的基本思路是不断选取产生信息增益最大的属性来划分样例集合，构造决策树。信息增益定义为节点与子结点的信息熵之差
//－（yes／sum）或（no／sum）为子集合中不同性的样例的比例
double Entropy(const set<int> &s) {
    double yes = 0, no = 0, sum = s.size(), ans = 0;
    for (set<int>::iterator it = s.begin(); it != s.end(); it++) {
        string s =dataTable[*it][str2int["PlayTennis"]];
        if (s == "Yes") {
            yes++;
        } else {
            no++;
        }
    }
    if (no == 0 || yes == 0) {
        return 0; //ans = 0
    }
    ans = -yes/sum * lg2(yes/sum) - no/sum * lg2(no/sum);
    return ans;
}

//信息收益可以定义为样本按照某属性划分时造成熵减少的期望，可以区分训练样本中正负样本的能力
double Gain(const set<int> &example, int attrid) {
    int attrcount = attrCount[attrid];
    double ans = Entropy(example); //信息熵之差，当前节点与子节点
    double sum = example.size();
    set<int> *pset = new set<int>[attrcount];
    for (set<int>::iterator it = example.begin(); it != example.end(); it++) {
        pset[str2int[dataTable[*it][attrid]]].insert(*it);
    }
    for (int i = 0; i < attrcount; i++) {
        ans -= pset[i].size() / sum * Entropy(pset[i]);
    }
    return ans;
}


int findBestAttribute(const set<int> &example, const set<int> &attr) {
    double max = 0;
    int k = -1;
    for (set<int>::iterator i = attr.begin(); i != attr.end(); i++) {
        double res = Gain(example, *i);
        if (res > max) {
            max = res;
            k = *i;
        }
    }
    if (k == -1) {
        cout << "FindBestAttribute error!" << endl;
    }
    return k;
}

Node *Id3_solution(set<int> example, set<int> &attributes, Node *parent) {
    Node *now = new Node; //创建树节点
    now->parentNode = parent;
    if (attributes.empty()) //如果此时属性列表已用完，即为空，则返回
        return now;

    //统计一下example，如果都为正或者都为负则表示已经抵达决策树的叶子节点，叶子节点的特征是有childNode为空
    int yes = 0, no = 0, sum = example.size();
    for (set<int>::iterator it = example.begin(); it != example.end(); it++) {
        string s = dataTable[*it][str2int["PlayTennis"]];
        if (s == "Yes") {
            yes++;
        } else {
            no++;
        }
    }
    if (yes == sum || yes == 0) {
        now->value = yes/sum;
        return now;
    }

    //找到最高信息增益的属性并将该属性从attributes集合中删除
    int bestattrID = findBestAttribute(example, attributes);
    now->attrid = bestattrID;
    attributes.erase(attributes.find(bestattrID));

    //将example根据最佳属性的不同属性值分成几个分支，每个分支有即有一个子树
    vector<set<int> > child = vector<set<int> >(attrCount[bestattrID]);
    for (set<int>::iterator i = example.begin(); i != example.end(); i++) {
        int id = str2int[dataTable[*i][bestattrID]];
        child[id].insert(*i);
    }
    for (int i = 0; i < child.size(); i++) {
        Node *res = Id3_solution(child[i], attributes, now);
        now->childNode.push_back(res);
    }
    return now;
}

int main(int argc, const char * argv[])
{
    init();
    Node *root = Id3_solution(S, attributes, NULL);
    return 0;
}
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
using namespace std;

#define MAXLEN 6 //输入每行的数据个数

/*
 多叉树的实现
 1、广以表
 2、父指针表示法，适于经常找父节点的应用
 3、子女链表示法，适于经常找子结点的应用
 4、左长子，右兄弟表示法，实现比较麻烦
 5、每个节点的所有孩子用vector保存
 教训：数据结构的设计很重要，本算法采用5比较适合，同时注意维护剩余样例和剩余属性信息，
 建树时横向遍历靠循环属性的值，纵向遍历靠递归调用
 */

vector<vector<string> > state; //实例集
vector<string> item(MAXLEN); //对应一行实例集
vector<string> attribute_row; //保存首行即属性行数据
string End("end"); //输入结束
string yes("yes");
string no("no");
string blank("");
map<string, vector<string> > map_attribute_values; //存储属性对应的所有的值
int tree_size = 0;
struct Node { //决策树节点
    string attribute; //属性值
    string arrived_value; //到达的属性值
    vector<Node *> childs; //所有的孩子
    Node() {
        attribute = blank;
        arrived_value = blank;
    }
};

Node *root;

void input() {
    string s;
    while (cin >> s, s.compare(End) != 0) {
        item[0] = s;
        for (int i = 1; i < MAXLEN; i++) {
            cin >> item[i];
        }
        state.push_back(item); //注意首行信息也输入进去，即属性
    }
    for (int i = 0; i < MAXLEN; i++) {
        attribute_row.push_back(state[0][i]);
    }
}

//根据数据实例计算属性与值组成的map
void ComputeMapFrom2DVector() {
    unsigned int i,j,k;
    bool exited = false;
    vector<string> values;
    for (i = 1; i < MAXLEN - 1; i++) {
        for (j = 1; j < state.size(); j++) {
            for (k = 0; k < values.size(); k++) {
                if (!values[k].compare(state[j][i]))
                    exited = true;
            }
            if (!exited) {
                values.push_back(state[j][i]); //注意vector的插入都是从前面插入的，注意更新it，始终指向vector头
            }
            exited = false;
        }
        map_attribute_values[state[0][1]] = values;
        values.erase(values.begin(), values.end());
    }
}

//判断样例是否正负性都为label
bool AllTheSameLabel(vector<vector<string> > remain_state, string label) {
    int count = 0;
    for (unsigned int i = 0; i < remain_state.size(); i++) {
        if (!remain_state[i][MAXLEN - 1].compare(label)) {
            count++;
        }
    }
    if (count == remain_state.size() - 1)
        return true;
    return false;
}

//找出样例中占多数的正／负性
string MostCommonLabel(vector<vector<string> > remain_state) {
    int y = 0, n = 0;
    for (unsigned i = 0; i < remain_state.size(); i++) {
        if (!remain_state[i][MAXLEN - 1].compare(yes)) {
            y++;
        } else {
            n++;
        }
    }
    if (y >= n)
        return yes;
    return no;
}

//根据具体属性和值来计算熵
double ComputeEntropy(vector<vector<string> > remain_state, string attribute, string value, bool ifparent) {
    vector<int> count(2,0);
    unsigned int i,j;
    bool done_flag = false; //哨兵值
    for (j = 1; j < MAXLEN; j++) {
        if (done_flag)
            break;
        if (!attribute_row[j].compare(attribute)) {
            for (i = 1; i < remain_state.size(); i++) {
                if ((!ifparent && !remain_state[i][j].compare(value)) || ifparent) { //ifparent记录是否算父节点
                    if (!remain_state[i][MAXLEN - 1].compare(yes))
                        count[0]++;
                    else
                        count[1]++;
                }
            }
            done_flag = true;
        }
    }
    if (count[0] == 0 ||  count[1] == 0) { //全部是正实例或者负实例
        return 0;
    }
    //具体计算熵，根据[+count[0], -count[1]],log2为底通过换底公式换成自然数底数
    double sum = count[0] + count[1];
    double entropy = -count[0] / sum * log(count[0] / sum) / log(2.0) - count[1] / sum * log(count[1] / sum) / log(2.0);
    return entropy;
}

//计算按照属性attribute划分当前剩余实例的信息增益
double ComputeGain(vector<vector<string> > remain_state, string attribute) {
    unsigned int j,k,m;
    //首先求不做划分时的熵
    double parent_entropy = ComputeEntropy(remain_state, attribute, blank, true);
    double children_entropy = 0;
    //然后求做划分后各个值的熵
    vector<string> values = map_attribute_values[attribute];
    vector<double> ratio;
    vector<int> count_values;
    int tempint;
    for (m = 0; m < values.size(); m++) {
        tempint = 0;
        for (k = 1; k < MAXLEN - 1; k++) {
            if (!attribute_row[k].compare(attribute)) {
                for (j = 1; j < remain_state.size(); j++) {
                    if (!remain_state[j][k].compare(values[m])) {
                        tempint++;
                    }
                }
            }
        }
        count_values.push_back(tempint);
    }

    for (j = 0; j < values.size(); j++) {
        ratio.push_back((double)count_values[j] / (double)(remain_state.size() - 1));
    }
    double temp_entropy;
    for (j = 0; j < values.size(); j++) {
        temp_entropy = ComputeEntropy(remain_state, attribute, values[j], false);
        children_entropy += ratio[j] * temp_entropy;
    }
    return (parent_entropy - children_entropy);
}

int FindAttriNumByName(string attri) {
    for (int i = 0; i < MAXLEN; i++) {
        if (!state[0][i].compare(attri)) {
            return i;
        }
    }
    cerr << "can't find the number of attribute" << endl;
    return 0;
}

//计算信息增益，DFS构建决策树
//current_node为当前的节点
//remain_state为剩余待分类的样例
//remain_attribute为剩余还没有考虑的属性
//返回根节点指针
Node *BuildDecisionTreeDFS(Node *p, vector<vector<string> > remain_state, vector<string> remain_attribute) {
//    if (remain_state.size() > 0) {
//        printv(remain_state);
//    }
    if (p == nullptr) {
        p = new Node();
    }
    //先看搜索到树叶的情况
    if (AllTheSameLabel(remain_state, yes)) {
        p->attribute = yes;
        return p;
    }
    if (AllTheSameLabel(remain_state, no)) {
        p->attribute = no;
        return p;
    }
    if (remain_attribute.size() == 0) { //所有的属性均已经考虑完了，还没有分尽
        string label = MostCommonLabel(remain_state);
        p->attribute = label;
        return p;
    }

    double max_gain = 0, temp_gain;
    vector<string>::iterator max_it = remain_attribute.begin();
    vector<string>::iterator it;
    for (it = remain_attribute.begin(); it != remain_attribute.end(); it++) {
        temp_gain = ComputeGain(remain_state, (*it));
        if (temp_gain > max_gain) {
            max_gain = temp_gain;
            max_it = it;
        }
    }

    //下面根据max_it指向的属性来划分当前样例，更新样例集和属性集
    vector<string> new_attribute;
    vector<vector<string> > new_state;
    for (vector<string>::iterator it = remain_attribute.begin(); it != remain_attribute.end(); it++) {
        if ((*it).compare(*max_it)) {
            new_attribute.push_back(*it);
        }
    }
    //确定了最佳划分属性，注意保存
    p->attribute = *max_it;
    vector<string> values = map_attribute_values[*max_it];
    int attribute_num = FindAttriNumByName(*max_it);
    new_state.push_back(attribute_row);
    for (vector<string>::iterator it = values.begin(); it != values.end(); it++) {
        for (unsigned int i = 1; i < remain_state.size(); i++) {
            if (!remain_state[i][attribute_num].compare(*it)) {
                new_state.push_back(remain_state[i]);
            }
        }
        Node *new_node = new Node();
        new_node->arrived_value = *it;
        if (new_state.size() == 0) { //表示当前没有这个分支的样例，当前的new_node为叶子节点
            new_node->attribute = MostCommonLabel(remain_state);
        } else {
            BuildDecisionTreeDFS(new_node, new_state, new_attribute);
        }
        //递归函数返回时即回溯时需要：1、将新节点加入父节点孩子容器，2、清楚new_state容器
        p->childs.push_back(new_node);
        new_state.erase(new_state.begin() + 1, new_state.end()); //注意先清空new_state中的前一个取值的样例，准备遍历下一个取值样例
    }
    return 0;
}

void PrintTree(Node *p, int depth) {
    for (int i = 0; i < depth; i++) {
        cout << "\t"; //按照树的深度先输出tab
    }
    if (!p->arrived_value.empty()) {
        cout << p->arrived_value << endl;
        for (int i = 0; i < depth + 1; i++) {
            cout << "\t"; //按照树的深度先输出tab
        }
    }
    cout << p->attribute << endl;
    for (vector<Node *>::iterator it = p->childs.begin(); it != p->childs.end(); it++) {
        PrintTree(*it, depth + 1);
    }
}

void FreeTree(Node *p) {
    if (p == NULL) {
        return;
    }
    for (vector<Node *>::iterator it = p->childs.begin(); it != p->childs.end(); it++) {
        FreeTree(*it);
    }
    delete p;
    tree_size++;
}

int main() {
    input();
    vector<string> remain_attribute;

    string outlook("outlook");
    string Temperature("Temperature");
    string Humidity("Humidity");
    string Wind("Wind");
    remain_attribute.push_back(outlook);
    remain_attribute.push_back(Temperature);
    remain_attribute.push_back(Humidity);
    remain_attribute.push_back(Wind);
    vector<vector<string> > remain_state;
    for (unsigned int i = 0; i < state.size(); i++) {
        remain_state.push_back(state[i]);
    }
    ComputeMapFrom2DVector();
    root = BuildDecisionTreeDFS(root, remain_state, remain_attribute);
    cout << "the decision tree is :" << endl;
    PrintTree(root, 0);
    FreeTree(root);
    cout << endl;
    cout << "tree_size:" << tree_size << endl;
    return 0;
}
