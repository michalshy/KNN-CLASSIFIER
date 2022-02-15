/**@file*/
//
// Created by Michin on 28.11.2021.
//
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <map>
#include <vector>
using namespace std;
#include "functions.h"
/** Funkcja przyporządkowywuje wartości a tablicy argv odpowiednim zmiennym które będą używane w całym programie
 *
 * @param argc ilość argumentów podanych przez użytkownika
 * @param argv tablica z argumentami podanymi przez użytkownika
 * @param addressTrain adres pliku z punktami treningowymi podany przez użytkownika
 * @param addressTest adres pliku z punktami testowymi podany przez użytkownika
 * @param addressOut adres pliku wyjściowego wybranego przez użytkownika
 * @param k ilość najbliższych sąsiadów służąca do dalszej klasyfikacji
 * @return jeśli ilość argumentów jest odpowiednia to po wykonaniu funkcji zwrócone zostaną wypełnione zmienne, jeśli nie to funkcja zwróci błąd
 */
int argInfo(int &argc, char * argv[], string &addressTrain, string &addressTest, string &addressOut, int &k)
{
    if(argc == 9)
    {
        addressTrain = argv[2];
        addressTest = argv[4];
        addressOut = argv[6];
        k = atoi(argv[8]);
        return 0;
    }
    else
    {
        cout<<"Liczba parametrow jest niewystarczajaca, sprobuj uruchomic program z przelacznikami:"<<endl
            <<"-train (sciezka pliku z punktami treningowymi)"<<endl
            <<"-test (sciezka pliku z punktami testowymi)"<<endl
            <<"-out (nazwa pliku z punktami testowymi i ich klasami)"<<endl
            <<"-k (liczba najblizszych sasiadow"<<endl;
        return 1;
    }
}
/** Funkcja odpowiada za prostą walidacje argumentów podanych przez użytkownika
 *
 * Sprawdza najczęstsze błędy związane z niepodaniem rozszerzenia pliku
 * @param addressTest adres pliku z punktami testowymi
 * @param addressTrain adres pliku z punktami treningowymi
 * @param addressOut adres pliku wyjściowego
 * @return funkcja kończy się bez błędu jeśli wszystkie pliki mają rozszerzenie txt, jeśli pojawia się błąd - program przerywa się
 */
int argValidate(const string &addressTest,const string &addressTrain, const string &addressOut)
{
    if(addressTrain.find(".txt")==string::npos)
    {
        cerr << "Plik treningowy zostal podany bez rozszerzenia .txt";
    }
    if(addressTest.find(".txt")==string::npos)
    {
        cerr << "Plik testowy zostal podany bez rozszerzenia .txt";
    }
    if(addressOut.find(".txt")==string::npos)
    {
        cerr << "Plik wyjściowy zostal podany bez rozszerzenia .txt";
    }
    return 0;
}
/** Dodatkowa walidacja argumentu k, który nie może być większy od ilości punktów treningowych
 *
 * @param k ilość k najbliższych sąsiadów
 * @param trainingAmount ilość punktów treningowych
 * @return zwraca błąd i kończy program jeśli k jest większe od ilości punktów treningowych
 */
int additionalValid(int k, int trainingAmount)
{
    if(k>trainingAmount)
    {
        cerr<<"liczba k jest wieksza niz ilosc punktow treningowych";
    }
    return 0;
}
/** Funkcja sczytuje współrzędne punktów treningowych i testowych z odpowiednich plików
 *
 * Po sczytaniu danych zapisywane są one do odpowiednich tablic
 * @param addressTrain adres pliku z punktami treningowymi
 * @param addressTest adres pliku z punktami testowymi
 * @param dimensions liczba wymiarów D w których zapisane są wszystkie punkty
 * @param trainingAmount ilość punktów treningowych
 * @param testAmount ilość punktów testowych
 * @param trainingPoints tablica D wymiarowa zawierająca współrzędne wszystkich punktów treningowych (trainingAmount x dimensions)
 * @param testPoints tablica D wymiarowa zawierająca współrzędne wszystkich punktów testowtch (testAmount x dimensions)
 * @param trainingPointClasses tablica jednowymiarowa stringów zawierająca klasy punktów treningowych
 */
void readData(const string &addressTrain, const string &addressTest, int &dimensions, int &trainingAmount, int &testAmount, double ** &trainingPoints, double ** &testPoints, string * &trainingPointClasses)
{
    int index1 = 0;
    int index2 = 0;
    int count1 = 0;
    int count2 = 0;
    ifstream file1, file2;
    string line1, line2;
    file1.open(addressTrain);
    while(getline(file1, line1))
    {
        istringstream iss(line1);
        if(line1.front()=='%')
        {
            continue;
        }
        else if(index1==0)
        {
            iss>>trainingAmount;
            index1++;
            trainingPointClasses = new string [trainingAmount];
        }
        else if(index1==1)
        {
            iss>>dimensions;
            index1++;
            trainingPoints = new double * [trainingAmount];
            auto *rowTraining = new double [trainingAmount * dimensions];
            for(int i = 0; i<trainingAmount; i++)
            {
                trainingPoints[i]=rowTraining+i*dimensions;
            }
        }
        else
        {
            istringstream cords(line1);
            for(int i=0; i<dimensions; i++)
            {
                cords>>trainingPoints[count1][i];
            }
            cords>>trainingPointClasses[count1];
            count1++;
        }
    }
    file1.close();
    file2.open(addressTest);
    while(getline(file2, line2))
    {
        istringstream iss(line2);
        if(line2.front()=='%' || index2==1)
        {
            index2++;
            continue;
        }
        else if(index2==0)
        {
            iss>>testAmount;
            testPoints = new double * [testAmount];
            auto *rowTest = new double [testAmount * dimensions];
            for(int i = 0; i<testAmount; i++)
            {
                testPoints[i]=rowTest+i*dimensions;
            }
            index2++;
        }
        else
        {
            for(int i=0; i<dimensions; i++)
            {
                iss>>testPoints[count2][i];
            }
            count2++;
        }
    }
    file2.close();
}
/** Funkcja sprawdza odległość każdego punktu testowego do każdego z punktów treningowych za pomocą metody euklidesowej
 *
 * Odległości zapisywane są do tablicy distances która ma wielkość ilość punktów testowych na ilość punktów treningowych
 * @param testPoints tablica zawierająca współrzędne punktów testowych
 * @param trainingPoints tablica zawierające współrzędne punktów treningowych
 * @param distances tablica zawierająca odległości kolejnych punktów testowych od każdego punktu treningowego
 * @param testAmount ilość punktów testowych
 * @param trainAmount ilość punktów treningowych
 * @param dimensions ilość wymiarów D
 */
void checkDistances(double ** &testPoints, double ** &trainingPoints, double ** &distances, int testAmount, int trainAmount, int dimensions)
{
    distances = new double * [testAmount];
    auto *rowDist = new double [testAmount * trainAmount];
    for(int i = 0; i<testAmount; i++)
    {
        distances[i]=rowDist+i*trainAmount;
    }
    double disOfOne;
    double x = 0;
    for(int i=0; i<testAmount; i++)
    {
        for(int j=0; j<trainAmount; j++)
        {
            disOfOne = 0;
            for(int k=0; k<dimensions; k++)
            {
                x = (testPoints[i][k] - trainingPoints[j][k])*(testPoints[i][k] - trainingPoints[j][k]);
                disOfOne+=x;
            }
            distances[i][j]=sqrt(disOfOne);
        }
    }
}
/** Funkcja wypełnia tablicę klas punktów testowych na podstawie k najbliższych puntków treningowych względem kolejnych punktów testowych, klasy zapisywane są do mapy classes
 *
 * Na podstawie ilości wystąpień wybranych klas, ta która ma największą wartość w mapie przyporządkowywana jest do wybranego punktu testowego
 * @param classes mapa do której zapisywane będą klasy k najbliższych punktów treningowych do kolejnych punktów testowych
 * @param distances tablica odległości kolejnych punktów testowych od każdego punktu treningowego
 * @param testPointClasses tablica klas punktów testowych, którą wypełnia ta funkcja
 * @param trainingPointClasses tablica klas punktów treningowych
 * @param testAmount ilość punktów testowych
 * @param k liczba k najbliższych sąsiadów (w tym przypadku najbliższych, względem kolejnych punktów testowych, punktów treningowych)
 * @param trainAmount liczba punktów treningowych
 */
void findTestClasses(map<string, int> &classes, double ** &distances, string * &testPointClasses, string * &trainingPointClasses, int testAmount, int k, int trainAmount)
{
    int min = 0;
    testPointClasses = new string [testAmount];
    for(int i = 0; i<testAmount; i++)
    {
        classes.clear();
        for(int j = 0; j<k; j++)
        {
            min = 0;
            for(int x = 1; x<trainAmount; x++)
            {
                if(distances[i][min]>distances[i][x])
                {
                    min = x;
                }
            }
            distances[i][min] = numeric_limits<double>::max();
            classes[trainingPointClasses[min]]++;
        }
        auto maxValue = max_element(classes.begin(), classes.end(),
                                    [](const pair<string, int>& p1, const pair<string, int>& p2) {
                                        return p1.second < p2.second; })->first;
        testPointClasses[i]=maxValue;
        maxValue.clear();
    }
}
/** Funkcja wypisuje informacje do pliku w sposób podobny do pliku z punktami testowymi, lecz w tym wypadku każdemu z punktów przyporządkwywuje ona klase dobraną w wyniku funkcji findTestClasses()
 *
 * @param address adres pliku do którego zostaną wypisane informacje
 * @param testPointsClasses tablica z klasami punktów testowych
 * @param testPoints tablica z współrzędnymi punktów testowych
 * @param testAmount ilość punktów testowych
 * @param dimensions ilość wymiarów D
 */
void writeToFile(const string &address, string * &testPointsClasses, double ** &testPoints, int testAmount, int dimensions)
{
    int pointNumber = 1;
    ofstream file;
    file.open(address);
    if(file.is_open())
    {
        file << testAmount << "\t%liczba punktow testowych" << endl << dimensions << "\t%liczba wymiarow D" << endl;
        for(int i = 0; i<testAmount; i++)
        {
            for(int j = 0; j<dimensions; j++)
            {
                file << testPoints[i][j] << " ";
            }
            file << testPointsClasses[i] << "\t%punkt " << pointNumber << endl;
            pointNumber++;
        }
    }
    file.close();
}
/** Funkcja usuwa wszystkie tablice zaalokowane wcześniej dynamicznie
 *
 * @param testPointClassess tablica klas punktów testowych
 * @param trainingPointClassess tablica klas punktów treningowych
 * @param testPoints talica współrzędnych punktów testowych
 * @param trainingPoints tablica współrzędnych punktów treningowych
 * @param distances tablica dystansów kolejnych punktów testowych od wszystkich punktów treningowych
 */
void deleteTables(string * &testPointClasses, string * &trainingPointClasses, double ** &testPoints, double ** &trainingPoints, double ** &distances)
{
    delete [] testPointClasses;
    delete [] trainingPointClasses;
    delete [] testPoints[0];
    delete [] testPoints;
    delete [] trainingPoints[0];
    delete [] trainingPoints;
    delete [] distances[0];
    delete [] distances;
}