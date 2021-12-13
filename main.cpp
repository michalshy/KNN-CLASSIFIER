//
// Created by Michin on 28.11.1584.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

int trainingPointsCounter(const string &address)
{
    int a;
    int t=0;
    string line;
    ifstream file;
    file.open(address);
    getline(file, line);
    istringstream iss(line);
    if(iss>>a && line.back()=='N')
    {
        t = a;
    }
    file.close();
    return t;
}

int testPointsCounter(const string &address)
{
    int a;
    int t=0;
    string line;
    ifstream file;
    file.open(address);
    getline(file, line);
    istringstream iss(line);
    if(iss>>a)
    {
        t = a;
    }
    file.close();
    return t;
}

int dimensionsCounter(const string &address)
{
    int a;
    int d=0;
    string line;
    ifstream file;
    file.open(address);
    while(getline(file, line))
    {
        istringstream iss(line);
        if(iss>>a && line.back()=='D')
        {
            d = a;
        }
    }

    file.close();
    return d;
}

void fillTheArray(double arr[],const string &address)
{


}



#include "functions.h"

int main(){

    int dimensions = dimensionsCounter("treningowe.txt");
    int trainingPoints = trainingPointsCounter("treningowe.txt");
    int testPoints = testPointsCounter("testowe.txt");
    string line;
    ifstream file;
    auto**trainP = new double * [trainingPoints];
    auto *rawTrain = new double [trainingPoints * dimensions];
    for(int i = 0; i<trainingPoints; i++)
    {
        trainP[i]=rawTrain+i*dimensions;
    }

    file.open("treningowe.txt");
    int count=0;
    for(int i = 0;i<4;i++)
    {
        file.ignore(256,'\n');
    }
    while(getline(file, line))
    {

        istringstream iss(line);
            for(int i = 0;i<dimensions;i++)
            {
                iss>>trainP[count][i];
                cout<<trainP[count][i]<<endl;
            }
        count++;
    }


    //delete [] raw;
    //delete [] array;
};