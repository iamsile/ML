//
//  main.cpp
//  logistic
//
//  Created by 陶玮 on 14-5-2.
//  Copyright (c) 2014年 陶玮. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <fstream>
using namespace std;

double x[110][5] = {0.0};
double y[110] = {0};
double newtheta[110], theta[110];

int main(int argc, const char * argv[])
{
    fstream infile;
    infile.open("/Users/taowei/Documents/ml/logistic/testSet.txt");
    int i, j;
    for (i = 0; !infile.eof(); i++) {
        x[i][0] = 1.0;
        for (j = 1; j < 3; j++) {
            infile >> x[i][j];
        }
        infile >> y[i];
    }
    int n = i;
    double alpha = 0.0000001;
    double eps = 0.00001;
    bool flag = false;
    double sum = 0.0;
    double diff = 0.0;
    theta[0] = 30.0;
    theta[1] = 30.0;
    while (!flag) {
        for (i = 0; i < n; i++) {
            newtheta[i] = theta[i];
        }
        for (i = 0; i < n; i++) {
            sum = 0.0;
            for (j = 0; j < 3; j++)
                sum += newtheta[j] * x[i][j];
            diff = y[i] - sum;
            diff = alpha * diff;
            for (j = 0; j < 3; j++) {
                newtheta[j] += diff * x[i][j];
            }
        }
        flag = true;
        for (i = 0; i < 3; i++)
            flag = flag && fabs(theta[i] - newtheta[i]) < eps;
        for (i = 0; i < 3; i++)
            theta[i] = newtheta[i];
    }
    for (i = 0; i < 3; i++) {
        cout << theta[i] << ' ';
    }
    cout << endl;
//    for (int i = 0; i < 10; i++) {
//        for (int j = 0; j < 3; j++) {
//            cout << x[i][j] << ' ';
//        }
//        cout << endl;
//    }
//    std::cout << "Hello, World!\n";

    return 0;
}

