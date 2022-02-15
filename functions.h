/**@file*/
//
// Created by Michin on 28.11.2021.
//
#ifndef PROJEKT_FUNCTIONS_H
#define PROJEKT_FUNCTIONS_H
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;
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
int argInfo(int &argc, char * argv[], string &addressTrain, string &addressTest, string &addressOut, int &k);
/** Funkcja odpowiada za prostą walidacje argumentów podanych przez użytkownika
 *
 * Sprawdza najczęstsze błędy związane z niepodaniem rozszerzenia pliku
 * @param addressTest adres pliku z punktami testowymi
 * @param addressTrain adres pliku z punktami treningowymi
 * @param addressOut adres pliku wyjściowego
 * @return funkcja kończy się bez błędu jeśli wszystkie pliki mają rozszerzenie txt, jeśli pojawia się błąd - program przerywa się
 */
int argValidate(const string &addressTest,const string &addressTrain, const string &addressOut);
/** Dodatkowa walidacja argumentu k, który nie może być większy od ilości punktów treningowych
 *
 * @param k ilość k najbliższych sąsiadów
 * @param trainingAmount ilość punktów treningowych
 * @return zwraca błąd i kończy program jeśli k jest większe od ilości punktów treningowych
 */
int additionalValid(int k, int trainingAmount);
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
void readData(const string &addressTrain, const string &addressTest, int &dimensions, int &trainingAmount, int &testAmount, double ** &trainingPoints, double ** &testPoints, string * &trainingPointClasses);
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
void checkDistances(double ** &testPoints, double ** &trainingPoints, double ** &distances, int testAmount, int trainAmount, int dimensions);
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
void findTestClasses(map<string, int> &classes, double ** &distances, string * &testPointClasses, string * &trainingPointClasses, int testAmount, int k, int trainAmount);
/** Funkcja wypisuje informacje do pliku w sposób podobny do pliku z punktami testowymi, lecz w tym wypadku każdemu z punktów przyporządkwywuje ona klase dobraną w wyniku funkcji findTestClasses()
 *
 * @param address adres pliku do którego zostaną wypisane informacje
 * @param testPointsClasses tablica z klasami punktów testowych
 * @param testPoints tablica z współrzędnymi punktów testowych
 * @param testAmount ilość punktów testowych
 * @param dimensions ilość wymiarów D
 */
void writeToFile(const string &address, string * &testPointsClasses, double ** &testPoints, int testAmount, int dimensions);
/** Funkcja usuwa wszystkie tablice zaalokowane wcześniej dynamicznie
 *
 * @param testPointClassess tablica klas punktów testowych
 * @param trainingPointClassess tablica klas punktów treningowych
 * @param testPoints talica współrzędnych punktów testowych
 * @param trainingPoints tablica współrzędnych punktów treningowych
 * @param distances tablica dystansów kolejnych punktów testowych od wszystkich punktów treningowych
 */
void deleteTables(string * &testPointClasses, string * &trainingPointClasses, double ** &testPoints, double ** &trainingPoints, double ** &distances);
#endif //PROJEKT_FUNCTIONS_H
