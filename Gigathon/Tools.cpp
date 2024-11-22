#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Funkcja obliczająca silnię.
unsigned long long factorialFunction(int n) {
    vector<unsigned long long> dp(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
        dp[i] = dp[i - 1] * i;
    }
    return dp[n];
}

// Funkcja mnożąca macierze 2x2.
void multiplyMatrix(int F[2][2], int M[2][2]) {
    int x = F[0][0] * M[0][0] + F[0][1] * M[1][0];
    int y = F[0][0] * M[0][1] + F[0][1] * M[1][1];
    int z = F[1][0] * M[0][0] + F[1][1] * M[1][0];
    int w = F[1][0] * M[0][1] + F[1][1] * M[1][1];

    F[0][0] = x;
    F[0][1] = y;
    F[1][0] = z;
    F[1][1] = w;
}

// Funkcja podnosząca macierz do potęgi.
void powerMatrix(int F[2][2], int n) {
    if (n == 0 || n == 1)
        return;
    int M[2][2] = {{1, 1}, {1, 0}};

    powerMatrix(F, n / 2);
    multiplyMatrix(F, F);

    if (n % 2 != 0)
        multiplyMatrix(F, M);
}

// Funkcja obliczająca n-ty wyraz ciągu Fibonacciego.
int fibFunction(int n) {
    if (n == 0)
        return 0;
    int F[2][2] = {{1, 1}, {1, 0}};
    powerMatrix(F, n - 1);
    return F[0][0];
}

// Funkcja obliczająca największy wspólny dzielnik (NWD).
int gcdFunction(int a, int b) {
    if (b == 0)
        return a;
    return gcdFunction(b, a % b);
}

// Funkcja obliczająca potęgę.
unsigned long long powerFunction(unsigned long long x, unsigned long long y) {
    unsigned long long result = 1;
    while (y > 0) {
        if (y & 1)
            result *= x;
        x *= x;
        y >>= 1;
    }
    return result;
}

// Funkcja obliczająca potęgę modulo.
unsigned long long modularPowerFunction(unsigned long long x, unsigned long long y, unsigned long long mod) {
    unsigned long long result = 1;
    x = x % mod;
    while (y > 0) {
        if (y & 1)
            result = (result * x) % mod;
        x = (x * x) % mod;
        y >>= 1;
    }
    return result;
}

// Funkcja sprawdzająca, czy liczba jest pierwsza.
bool primeFunction(int n) {
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Funkcja wyszukiwania binarnego.
int binarySearch(const vector<int>& arr, int x) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (arr[m] == x)
            return m;
        if (arr[m] < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}

// Funkcja scalająca dwie części tablicy.
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

// Funkcja sortowania przez scalanie.
void mergeSort(vector<int>& arr, int l, int r) {
    if (l >= r)
        return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Iteracyjna funkcja obliczająca największy wspólny dzielnik (NWD).
int gcdIterative(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Funkcja sita Eratostenesa.
vector<bool> sieveOfEratosthenes(int n) {
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i)
                isPrime[j] = false;
        }
    }
    return isPrime;
}

// Rozszerzony algorytm Euklidesa.
int gcdExtended(int a, int b, int* x, int* y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

// Funkcja obliczająca odwrotność modulo.
int modInverse(int a, int m) {
    int x, y;
    int g = gcdExtended(a, m, &x, &y);
    if (g != 1)
        return -1;
    else {
        int res = (x % m + m) % m;
        return res;
    }
}

// Funkcja sprawdzająca, czy liczba jest potęgą dwójki.
bool isPowerOfTwo(int n) {
    return (n && !(n & (n - 1)));
}

// Funkcja obliczająca odwrotność modulo dla potęg.
unsigned long long modInversePowers(unsigned long long a, unsigned long long p) {
    return powerFunction(a, p - 2, p);
}

// Funkcja obliczająca najmniejszą wspólną wielokrotność (NWW).
int lcmFunction(int a, int b) {
    return (a / gcdFunction(a, b)) * b;
}

// Funkcja obliczająca n-ty trójkątny numer.
int triangularNumber(int n) {
    return n * (n + 1) / 2;
}

// Funkcja sprawdzająca, czy liczba jest kwadratem.
bool isPerfectSquare(int n) {
    int sqrtN = static_cast<int>(sqrt(n));
    return (sqrtN * sqrtN == n);
}

// Funkcja obliczająca czynniki pierwsze liczby.
vector<int> primeFactors(int n) {
    vector<int> factors;
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }
    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 2)
        factors.push_back(n);
    return factors;
}

// Funkcja obliczająca liczbę dzielników liczby.
int numberOfDivisors(int n) {
    int count = 0;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i * i == n)
                count++;
            else
                count += 2;
        }
    }
    return count;
}

// Funkcja obliczająca sumę dzielników liczby.
int sumOfDivisors(int n) {
    int sum = 0;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i * i == n)
                sum += i;
            else
                sum += i + (n / i);
        }
    }
    return sum;
}

// Funkcja sprawdzająca, czy liczba jest palindromem.
bool isPalindrome(int n) {
    int original = n, reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }
    return original == reversed;
}

// Funkcja obliczająca medianę tablicy.
double calculateMedian(vector<int>& arr) {
    size_t size = arr.size();
    if (size == 0) {
        throw invalid_argument("The array is empty.");
    }
    sort(arr.begin(), arr.end());
    if (size % 2 == 0) {
        return (arr[size / 2 - 1] + arr[size / 2]) / 2.0;
    } else {
        return arr[size / 2];
    }
}

// Funkcja obliczająca średnią tablicy.
double calculateMean(const vector<int>& arr) {
    if (arr.empty()) {
        throw invalid_argument("The array is empty.");
    }
    double sum = 0;
    for (int num : arr) {
        sum += num;
    }
    return sum / arr.size();
}

// Funkcja sprawdzająca, czy liczba jest doskonała.
bool isPerfectNumber(int n) {
    int sum = 1;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i * i != n)
                sum += i + n / i;
            else
                sum += i;
        }
    }
    return sum == n && n != 1;
}

// Funkcja sprawdzająca, czy ciąg znaków jest palindromem.
bool isPalindromeString(const string& str) {
    int l = 0, r = str.size() - 1;
    while (l < r) {
        if (str[l] != str[r])
            return false;
        l++;
        r--;
    }
    return true;
}

// Funkcja znajdująca maksymalny element w tablicy.
int findMax(const vector<int>& arr) {
    if (arr.empty()) {
        throw invalid_argument("The array is empty.");
    }
    return *max_element(arr.begin(), arr.end());
}

// Funkcja znajdująca minimalny element w tablicy.
int findMin(const vector<int>& arr) {
    if (arr.empty()) {
        throw invalid_argument("The array is empty.");
    }
    return *min_element(arr.begin(), arr.end());
}

// Funkcja odwracająca ciąg znaków.
string reverseString(const string& str) {
    string reversed = str;
    reverse(reversed.begin(), reversed.end());
    return reversed;
}

// Funkcja odwracająca liczbę całkowitą.
int reverseInteger(int n) {
    int reversed = 0;
    while (n != 0) {
        int digit = n % 10;
        reversed = reversed * 10 + digit;
        n /= 10;
    }
    return reversed;
}

// Funkcja sprawdzająca, czy dwa ciągi znaków są anagramami.
bool isAnagram(const string& str1, const string& str2) {
    if (str1.size() != str2.size()) {
        return false;
    }
    unordered_map<char, int> count;
    for (char c : str1) {
        count[c]++;
    }
    for (char c : str2) {
        if (count.find(c) == count.end() || count[c] == 0) {
            return false;
        }
        count[c]--;
    }
    return true;
}

// Funkcja sprawdzająca, czy liczba jest parzysta.
bool isEven(int n) {
    return n % 2 == 0;
}

// Funkcja sprawdzająca, czy liczba jest nieparzysta.
bool isOdd(int n) {
    return n % 2 != 0;
}

// Funkcja obliczająca sumę elementów tablicy.
int sumArray(const vector<int>& arr) {
    int sum = 0;
    for (int num : arr) {
        sum += num;
    }
    return sum;
}

// Funkcja obliczająca iloczyn elementów tablicy.
int productArray(const vector<int>& arr) {
    int product = 1;
    for (int num : arr) {
        product *= num;
    }
    return product;
}

// Funkcja zamieniająca ciąg znaków na wielkie litery.
string toUpperCase(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

// Funkcja zamieniająca ciąg znaków na małe litery.
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Funkcja sprawdzająca, czy liczba jest w ciągu Fibonacciego.
bool isFibonacci(int n) {
    int x1 = 5 * n * n + 4;
    int x2 = 5 * n * n - 4;
    return isPerfectSquare(x1) || isPerfectSquare(x2);
}

// Funkcja generująca ciąg Fibonacciego do n-tego wyrazu.
vector<int> generateFibonacci(int n) {
    vector<int> fibSequence;
    int a = 0, b = 1;
    while (a <= n) {
        fibSequence.push_back(a);
        int next = a + b;
        a = b;
        b = next;
    }
    return fibSequence;
}

// Funkcja sprawdzająca, czy trzy liczby mogą być bokami trójkąta.
bool canFormTriangle(int a, int b, int c) {
    return (a + b > c) && (a + c > b) && (b + c > a);
}

// Funkcja sprawdzająca, czy trzy liczby mogą być bokami trójkąta równoramiennego.
bool isIsoscelesTriangle(int a, int b, int c) {
    return canFormTriangle(a, b, c) && (a == b || a == c || b == c);
}

// Funkcja sprawdzająca, czy trzy liczby mogą być bokami trójkąta równobocznego.
bool isEquilateralTriangle(int a, int b, int c) {
    return canFormTriangle(a, b, c) && (a == b && b == c);
}

// Funkcja sprawdzająca, czy trzy liczby mogą być bokami trójkąta prostokątnego.
bool isRightAngledTriangle(int a, int b, int c) {
    if (!canFormTriangle(a, b, c)) return false;
    int a2 = a * a, b2 = b * b, c2 = c * c;
    return (a2 + b2 == c2) || (a2 + c2 == b2) || (b2 + c2 == a2);
}

// Funkcja obliczająca kąty trójkąta.
vector<double> calculateTriangleAngles(int a, int b, int c) {
    if (!canFormTriangle(a, b, c)) {
        throw invalid_argument("The sides do not form a triangle.");
    }
    double A = acos((b * b + c * c - a * a) / (2.0 * b * c)) * 180.0 / M_PI;
    double B = acos((a * a + c * c - b * b) / (2.0 * a * c)) * 180.0 / M_PI;
    double C = 180.0 - A - B;
    return {A, B, C};
}


// Funkcja porównująca długości dwóch słów.
string compareWordLengths(const string& word1, const string& word2) {
    if (word1.length() > word2.length()) {
        return word1 + " is longer than " + word2 + ".";
    } else if (word1.length() < word2.length()) {
        return word2 + " is longer than " + word1 + ".";
    } else {
        return word1 + " and " + word2 + " has the same length.";
    }
}

// Funkcja obliczająca wartość słowa jako sumę wartości ASCII jego znaków.
int calculateWordValue(const string& word) {
    int value = 0;
    for (char c : word) {
        value += static_cast<int>(c);
    }
    return value;
}

// Funkcja zmieniająca jeden znak w ciągu znaków.
string changeLetter(string str, int index, char newChar) {
    if (index < 0 || index >= str.length()) {
        throw out_of_range("Index out of range.");
    }
    str[index] = newChar;
    return str;
}

// Funkcja przesuwająca litery w ciągu znaków o określoną liczbę miejsc (szyfr Cezara).
string caesarShift(const string& str, int shift) {
    string result = str;
    for (char& c : result) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;
        }
    }
    return result;
}

// Funkcja zwracająca n'tą liczbę tribonacciego.
unsigned long long tribonacciNumber(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;
    return tribonacciNumber(n - 1) + tribonacciNumber(n - 2) + tribonacciNumber(n - 3);
}

// Funkcja zwracająca n'tą liczbę Catalana.
unsigned long long catalanNumber(int n) {
    if (n <= 1) return 1;
    unsigned long long result = 0;
    for (int i = 0; i < n; ++i)
        result += catalanNumber(i) * catalanNumber(n - i - 1);
    return result;
}

// Funkcja zwracająca n'tą liczbę Bella.
unsigned long long bellNumber(int n) {
    vector<vector<unsigned long long>> bell(n + 1, vector<unsigned long long>(n + 1, 0));
    bell[0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        bell[i][0] = bell[i - 1][i - 1];
        for (int j = 1; j <= i; ++j)
            bell[i][j] = bell[i - 1][j - 1] + bell[i][j - 1];
    }
    return bell[n][0];
}

// Funkcja zwracająca n'tą liczbę harmoniczną.
double harmonicNumber(int n) {
    double result = 0.0;
    for (int i = 1; i <= n; ++i)
        result += 1.0 / i;
    return result;
}

// Funkcja zwracająca n'tą liczbę Lucasa.
unsigned long long lucasNumber(int n) {
    if (n == 0) return 2;
    if (n == 1) return 1;
    return lucasNumber(n - 1) + lucasNumber(n - 2);
}

// Funkcja zwracająca n'tą liczbę tetraedralną.
unsigned long long tetrahedralNumber(int n) {
    return n * (n + 1) * (n + 2) / 6;
}

// Funkcja zwracająca n'tą liczbę pentagonalną.
unsigned long long pentagonalNumber(int n) {
    return n * (3 * n - 1) / 2;
}

// Funkcja zwracająca n'tą liczbę heksagonalną.
unsigned long long hexagonalNumber(int n) {
    return n * (2 * n - 1);
}

// Funkcja zwracająca n'tą liczbę heptagonalną.
unsigned long long heptagonalNumber(int n) {
    return n * (5 * n - 3) / 2;
}

// Funkcja zwracająca n'tą liczbę pierwszą.
int nthPrime(int n) {
    if (n == 1) return 2;
    int count = 1;
    int num = 3;
    while (count < n) {
        bool isPrime = true;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) ++count;
        num += 2;
    }
    return num - 2;
}

// Funkcja zwracająca super silnię liczby n.
unsigned long long superFactorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= factorialFunction(i);
    }
    return result;
}

// Function to return the hyperfactorial of a number n.
unsigned long long hyperFactorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= pow(i, i);
    }
    return result;
}

// Function to check if a number is an Armstrong number.
bool isArmstrongNumber(int n) {
    int original = n, sum = 0;
    int numDigits = static_cast<int>(log10(n)) + 1;
    while (n != 0) {
        int digit = n % 10;
        sum += pow(digit, numDigits);
        n /= 10;
    }
    return sum == original;
}

// Function to generate Pascal's Triangle with a given number of rows.
vector<vector<int>> generatePascalsTriangle(int numRows) {
    vector<vector<int>> triangle(numRows);
    for (int i = 0; i < numRows; ++i) {
        triangle[i].resize(i + 1);
        triangle[i][0] = triangle[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
        }
    }
    return triangle;
}

// Function to return the sum of squares of the first n natural numbers.
unsigned long long sumOfSquares(int n) {
    return (n * (n + 1) * (2 * n + 1)) / 6;
}

// Function to return the sum of cubes of the first n natural numbers.
unsigned long long sumOfCubes(int n) {
    unsigned long long sum = (n * (n + 1)) / 2;
    return sum * sum;
}

int main() {
    return 0;
}
