#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class Literator {
private:
    string str1, str2; // Przechowuje dwa ciągi znaków

public:
    // Wczytuje dwa ciągi znaków od użytkownika
    void readStrings() {
        cout << "Enter first string: ";
        getline(cin, str1);
        cout << "Enter second string: ";
        getline(cin, str2);
    }

    // Zwraca dłuższy z dwóch ciągów znaków
    string longerString() const {
        return (str1.length() >= str2.length()) ? str1 : str2;
    }

    // Zamienia wszystkie litery w ciągu znaków na wielkie litery
    void toUpperCase(string &str) const {
        for (char &c : str) {
            if (islower(c)) {
                c = toupper(c);
            }
        }
    }

    // Zamienia wszystkie litery w ciągu znaków na małe litery
    void toLowerCase(string &str) const {
        for (char &c : str) {
            if (isupper(c)) {
                c = tolower(c);
            }
        }
    }

    // Oblicza sumę wartości ASCII wszystkich znaków w ciągu znaków
    int calculateValue(const string &str) const {
        int sum = 0;
        for (char c : str) {
            sum += static_cast<int>(c);
        }
        return sum;
    }

    // Zwraca ciąg znaków o większej sumie wartości ASCII
    string heavierString() const {
        return (calculateValue(str1) >= calculateValue(str2)) ? str1 : str2;
    }

    // Zamienia wszystkie wystąpienia danego znaku na inny znak w ciągu znaków
    void replaceChar(string &str, char oldChar, char newChar) const {
        for (char &c : str) {
            if (c == oldChar) {
                c = newChar;
            }
        }
    }

    // Zamienia znak na danej pozycji w ciągu znaków na inny znak
    void replaceCharAt(string &str, int index, char newChar) const {
        if (index >= 0 && index < str.length()) {
            str[index] = newChar;
        }
    }

    // Wypisuje oba ciągi znaków
    void printStrings() const {
        cout << "String 1: " << str1 << endl;
        cout << "String 2: " << str2 << endl;
    }
};

int main() {
    Literator literator;
    literator.readStrings(); // Wczytuje ciągi znaków od użytkownika

    cout << "Longer string: " << literator.longerString() << endl; // Wypisuje dłuższy ciąg znaków
    cout << endl;
    
    string str1Copy = literator.longerString();
    literator.toUpperCase(str1Copy); // Zamienia dłuższy ciąg znaków na wielkie litery
    cout << "Uppercase: " << str1Copy << endl;
    cout << endl;

    literator.toLowerCase(str1Copy); // Zamienia dłuższy ciąg znaków na małe litery
    cout << "Lowercase: " << str1Copy << endl;
    cout << endl;

    // Wypisuje sumę wartości ASCII dla obu ciągów znaków
    cout << "Value of string 1: " << literator.calculateValue(literator.longerString()) << endl;
    cout << "Value of string 2: " << literator.calculateValue(literator.heavierString()) << endl;
    cout << endl;
    
    cout << "Heavier string: " << literator.heavierString() << endl; // Wypisuje cięższy ciąg znaków
    cout << endl;

    literator.replaceChar(str1Copy, 'a', 'z'); // Zamienia wszystkie 'a' na 'z' w dłuższym ciągu znaków
    cout << "Replaced 'a' with 'z': " << str1Copy << endl;
    cout << endl;

    literator.replaceCharAt(str1Copy, 2, 'X'); // Zamienia znak na pozycji 2 na 'X' w dłuższym ciągu znaków
    cout << "Replaced char at index 2 with 'X': " << str1Copy << endl;
    cout << endl;

    literator.printStrings(); // Wypisuje oba ciągi znaków

    return 0;
}
