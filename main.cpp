/**@file*/
//
// Created by Michin on 28.11.1584.
//
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;
#include "functions.h"
/** Główna funkcja programu
 *
 * @param argc ilość argumentów podanych przez użytkownika
 * @param argv tablica z argumentami
 */
int main(int argc, char *argv[]){
    int k;
    string addressOut, addressTrain, addressTest;
    if(argInfo(argc, argv, addressTrain, addressTest, addressOut, k)) return 1;
    if(argValidate(addressTest, addressTrain, addressOut)) return 1;
    int dimensions, trainingAmount, testAmount;
    double ** trainingPoints;
    double ** testPoints;
    double ** distances;
    string * trainingPointClasses;
    map<string, int> classes;
    string * testPointClasses;
    readData(addressTrain, addressTest, dimensions, trainingAmount, testAmount, trainingPoints, testPoints, trainingPointClasses);
    additionalValid(k, trainingAmount);
    checkDistances(testPoints, trainingPoints, distances, testAmount, trainingAmount, dimensions);
    findTestClasses(classes, distances, testPointClasses, trainingPointClasses, testAmount, k, trainingAmount);
    writeToFile(addressOut, testPointClasses, testPoints, testAmount, dimensions);
    deleteTables(testPointClasses, trainingPointClasses, testPoints, trainingPoints, distances);
};