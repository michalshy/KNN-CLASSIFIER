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

#include "functions.h"

int main(){

    const int trainingPoints = trainingPointsCounter("treningowe.txt");
    const int dimensions = dimensionsCounter("treningowe.txt");
    cout<<trainingPoints<<" "<<dimensions<<endl;

    auto * coordinates = new double[trainingPoints * dimensions];
    string line;
    ifstream file;
    double a;
    file.open("treningowe.txt");
    while(getline(file, line))
    {
        istringstream iss(line);
        if(iss>>a)
        {
            for(int i=0; i<trainingPoints; i++)
            {

                for(int j=0; j<dimensions; j++)
                {
                    coordinates[i * dimensions + j]=a;
                }
            }
        }
    }
    cout<<coordinates[dimensions * 0 + 0]<<" "<<coordinates[dimensions * 4 + 4]<<" "<<coordinates[dimensions * 19 + 4];




    return 0;

};