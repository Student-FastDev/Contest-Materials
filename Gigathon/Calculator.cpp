#include <iostream>
#include <sstream>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Funkcja do obliczania wartości wyrażenia matematycznego.
double evaluateExpression(const string& expression);

// Funkcja obliczająca silnię (factorial).
double factorialFunction(double value) {
    if (value < 0) throw runtime_error("Factorial of a negative number is undefined");
    if (value != floor(value)) throw runtime_error("Factorial is defined only for integers");
    double result = 1;
    for (int i = 1; i <= static_cast<int>(value); i++) {
        result *= i;
    }
    return result;
}

// Funkcja obliczająca największy wspólny dzielnik (NWD) dwóch liczb.
double gcdFunction(double a, double b) {
    if (a != floor(a) || b != floor(b)) throw runtime_error("GCD is defined only for integers");
    while (b != 0) {
        double tempVariable = fmod(a, b);
        a = b;
        b = tempVariable;
    }
    return abs(a);
}

// Funkcja sprawdzająca, czy znak jest operatorem.
bool isOperator(char character) {
    return character == '+' || character == '-' || character == '*' || character == '/' || character == '%' ||
           character == '^' || character == '&' || character == '|' || character == '!' || character == '<' || 
           character == '>' || character == '=' || character == '~';
}

// Funkcja zwracająca priorytet operatora.
int getOperatorPrecedence(char operatorChar) {
    if (operatorChar == '!' || operatorChar == '~') return 5; // Unary NOT i bitowy NOT.
    if (operatorChar == '^') return 4; // Potęgowanie.
    if (operatorChar == '*' || operatorChar == '/' || operatorChar == '%') return 3; // Mnożenie, dzielenie, modulo.
    if (operatorChar == '+' || operatorChar == '-') return 2; // Dodawanie i odejmowanie
    if (operatorChar == '<' || operatorChar == '>' || operatorChar == '=' || operatorChar == '&') return 1; // Operatory bitowe i porównania.
    if (operatorChar == '&' || operatorChar == '|') return 0; // Operatory logiczne (AND, OR).
    return 0;
}

// Funkcja stosująca operator binarny na dwóch liczbach.
double applyBinaryOperator(double numberA, double numberB, char operatorChar) {
    switch (operatorChar) {
        case '+': return numberA + numberB;
        case '-': return numberA - numberB;
        case '*': return numberA * numberB;
        case '/':
            if (numberB == 0) throw runtime_error("Division by zero");
            return numberA / numberB;
        case '%':
            if (numberB == 0) throw runtime_error("Modulo by zero");
            return static_cast<int>(numberA) % static_cast<int>(numberB);
        case '^': return pow(numberA, numberB);
        case '&': return static_cast<int>(numberA) & static_cast<int>(numberB);
        case '|': return static_cast<int>(numberA) | static_cast<int>(numberB);
        case '<': return static_cast<int>(numberA) << static_cast<int>(numberB);
        case '>': return static_cast<int>(numberA) >> static_cast<int>(numberB);
        case '=': return numberA == numberB ? 1 : 0;
        case '~': return static_cast<int>(~static_cast<int>(numberA));
        default:
            throw invalid_argument("Unknown operator");
    }
}

// Funkcja stosująca operator unarny.
double applyUnaryOperator(double value, char operatorChar) {
    switch (operatorChar) {
        case '!': return value == 0 ? 1 : 0;
        case '~': return ~static_cast<int>(value);
        default:
            throw invalid_argument("Unknown unary operator");
    }
}

// Funkcja konwertująca stopnie na radiany.
double toRadians(double degreesValue) {
    return degreesValue * M_PI / 180.0;
}

// Funkcja obliczająca n-ty wyraz ciągu Fibonacciego za pomocą potęgowania macierzy.
double fibFunction(double value) {
    if (value != floor(value) || value < 0) throw runtime_error("Fibonacci is defined only for non-negative integers");
    int n = static_cast<int>(value);
    if (n == 0) return 0;
    if (n == 1) return 1;

    vector<vector<long long>> F = {{1, 1}, {1, 0}};
    vector<vector<long long>> result = {{1, 0}, {0, 1}}; // Macierz jednostkowa

    auto multiply = [](const vector<vector<long long>>& a, const vector<vector<long long>>& b) {
        vector<vector<long long>> c(2, vector<long long>(2));
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    c[i][j] += a[i][k] * b[k][j];
        return c;
    };

    while (n > 0) {
        if (n % 2 == 1)
            result = multiply(result, F);
        F = multiply(F, F);
        n /= 2;
    }

    return result[0][1];
}

// Funkcja sprawdzająca, czy liczba jest pierwsza.
double primeFunction(double value) {
    if (value != floor(value) || value < 2) return 0;
    int n = static_cast<int>(value);
    for (int i = 2; i <= sqrt(n); ++i) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Funkcja stosująca funkcję unarną.
double applyUnaryFunction(const string& functionName, double value) {
    if (functionName == "sqrt") {
        if (value < 0) throw runtime_error("Square root of a negative number");
        return sqrt(value);
    } else if (functionName == "log") {
        if (value <= 0) throw runtime_error("Logarithm of a non-positive number");
        return log10(value);
    } else if (functionName == "ln") {
        if (value <= 0) throw runtime_error("Natural log of a non-positive number");
        return log(value);
    } else if (functionName == "exp") {
        return exp(value);
    } else if (functionName == "sin") {
        return sin(toRadians(value));
    } else if (functionName == "cos") {
        return cos(toRadians(value));
    } else if (functionName == "tan") {
        return tan(toRadians(value));
    } else if (functionName == "sinh") {
        return sinh(value);
    } else if (functionName == "cosh") {
        return cosh(value);
    } else if (functionName == "tanh") {
        return tanh(value);
    } else if (functionName == "abs") {
        return fabs(value);
    } else if (functionName == "ceil") {
        return ceil(value);
    } else if (functionName == "floor") {
        return floor(value);
    } else if (functionName == "round") {
        return round(value);
    } else if (functionName == "factorial") {
        return factorialFunction(value); // Wywołanie funkcji silni.
    } else if (functionName == "fib") {
        return fibFunction(value); // Wywołanie funkcji Fibonacciego.
    } else if (functionName == "prime") {
        return primeFunction(value); // Wywołanie funkcji sprawdzającej pierwszość liczby.
    }
    throw invalid_argument("Unknown function: " + functionName);
}

// Funkcja stosująca funkcję dwuargumentową.
double applyBinaryFunction(const string& functionName, double a, double b) {
    if (functionName == "gcd") {
        return gcdFunction(a, b); // Wywołanie funkcji NWD.
    }
    throw invalid_argument("Unknown binary function: " + functionName);
}

// Funkcja sortująca argumenty.
vector<double> sortFunction(const vector<double>& args) {
    vector<double> sortedArgs = args;
    sort(sortedArgs.begin(), sortedArgs.end());
    return sortedArgs;
}

// Funkcja obliczająca wartość wyrażenia z `sort`.
vector<double> parseSortFunction(const string& expression) {
    if (expression.find('(') == string::npos || expression.back() != ')') {
        throw invalid_argument("Function 'sort' must have arguments enclosed in parentheses");
    }

    string innerExpression = expression.substr(expression.find('(') + 1, expression.length() - expression.find('(') - 2);
    stringstream ss(innerExpression);
    vector<double> args;
    string token;

    while (getline(ss, token, ',')) {
        args.push_back(evaluateExpression(token)); // Wywołujemy `evaluateExpression` dla każdego argumentu.
    }

    return sortFunction(args); // Sortujemy wyniki.
}

// Funkcja obliczająca medianę.
double medianFunction(const vector<double>& args) {
    if (args.empty()) throw runtime_error("Median of an empty list is undefined");
    vector<double> sortedArgs = args;
    sort(sortedArgs.begin(), sortedArgs.end());
    size_t size = sortedArgs.size();
    if (size % 2 == 0) {
        return (sortedArgs[size / 2 - 1] + sortedArgs[size / 2]) / 2.0;
    } else {
        return sortedArgs[size / 2];
    }
}

// Funkcja obliczająca średnią.
double meanFunction(const vector<double>& args) {
    if (args.empty()) throw runtime_error("Mean of an empty list is undefined");
    double sum = accumulate(args.begin(), args.end(), 0.0);
    return sum / args.size();
}

// Funkcja obliczająca wartość wyrażenia z `median` lub `mean`.
double parseMedianOrMeanFunction(const string& expression, const string& functionName) {
    if (expression.find('(') == string::npos || expression.back() != ')') {
        throw invalid_argument("Function '" + functionName + "' must have arguments enclosed in parentheses");
    }

    string innerExpression = expression.substr(expression.find('(') + 1, expression.length() - expression.find('(') - 2);
    stringstream ss(innerExpression);
    vector<double> args;
    string token;

    while (getline(ss, token, ',')) {
        args.push_back(evaluateExpression(token)); // Wywołujemy `evaluateExpression` dla każdego argumentu.
    }

    if (functionName == "median") {
        return medianFunction(args); // Obliczamy medianę.
    } else if (functionName == "mean") {
        return meanFunction(args); // Obliczamy średnią.
    }

    throw invalid_argument("Unknown function: " + functionName);
}

double evaluateExpression(const string& expression) {
    // Stos do przechowywania wartości liczbowych.
    stack<double> valuesStack;
    // Stos do przechowywania operatorów i nawiasów.
    stack<char> operatorsStack;
    // Inicjalizacja strumienia, który umożliwia analizowanie wyrażenia.
    stringstream ss(expression);
    // Zmienna do przechowywania aktualnie odczytanego tokenu (znaku).
    char token;
    char prevToken = '\0'; // Zmienna do przechowywania poprzedniego tokenu
    
    // Rozpoczęcie pętli przetwarzającej wyrażenie znak po znaku.
    while (ss >> ws >> token) {
        // Sprawdzamy, czy token to liczba (cyfra lub kropka).
        if (isdigit(token) || token == '.') {
            // Wstawiamy token z powrotem do strumienia, aby odczytać pełną liczbę.
            ss.putback(token);
            double number;
            ss >> number;  // Odczytujemy liczbę zmiennoprzecinkową.
            valuesStack.push(number);  // Wkładamy liczbę na stos wartości.
    
            // Jeżeli następny znak to nawias lub litera (może to być funkcja), dodajemy operator mnożenia.
            if (ss.peek() == '(' || isalpha(ss.peek())) {
                operatorsStack.push('*');
            }
        }
        // Obsługa funkcji (np. sqrt, log).
        else if (isalpha(token)) {
            string functionName;
            functionName += token;  // Dodajemy pierwszą literę funkcji.
    
            // Odczytujemy kolejne litery funkcji, jeżeli są.
            while (ss.peek() != EOF && isalpha(ss.peek())) {
                ss >> token;
                functionName += token;
            }
    
            // Sprawdzamy, czy po nazwie funkcji występuje '('.
            if (ss.peek() != '(') {
                throw invalid_argument("Function '" + functionName + "' must be followed by '('");
            }
            ss.get();  // Odczytujemy '('.
    
            string innerExpression;
            int parenthesesCount = 1;
    
            // Odczytujemy argumenty funkcji, obsługując nawiasy.
            while (parenthesesCount > 0) {
                if (ss.peek() == EOF) throw runtime_error("Mismatched parentheses in function");
                ss >> token;
                if (token == '(') parenthesesCount++;  // Zwiększamy licznik nawiasów otwierających.
                if (token == ')') parenthesesCount--;  // Zmniejszamy licznik nawiasów zamykających.
                if (parenthesesCount > 0) innerExpression += token;  // Przechowujemy zawartość nawiasów.
            }
    
            double result;
            // Jeśli to funkcja 'gcd', wykonujemy odpowiednie obliczenia.
            if (functionName == "gcd") {
                size_t comma = innerExpression.find(',');
                if (comma == string::npos) throw invalid_argument("Function 'gcd' expects two arguments");
    
                double arg1 = evaluateExpression(innerExpression.substr(0, comma));  // Pierwszy argument.
                double arg2 = evaluateExpression(innerExpression.substr(comma + 1));  // Drugi argument.
                result = applyBinaryFunction("gcd", arg1, arg2);  // Wywołanie funkcji NWD.
            } else {
                // Dla innych funkcji wykonujemy obliczenie z jednym argumentem.
                double argument = evaluateExpression(innerExpression);
                result = applyUnaryFunction(functionName, argument);  // Wywołanie funkcji unarnej.
            }
    
            valuesStack.push(result);  // Wkładamy wynik na stos wartości.
    
            // Jeżeli po funkcji występuje nawias lub litera, traktujemy to jako operator mnożenia.
            if (ss.peek() == '(' || isalpha(ss.peek())) {
                operatorsStack.push('*');
            }
        }
        // Obsługa nawiasów otwierających.
        else if (token == '(') {
            // Sprawdzamy, czy poprzedni token był liczbą lub zamykającym nawiasem, aby dodać operator mnożenia.
            if (isdigit(prevToken) || prevToken == ')') {
                operatorsStack.push('*');
            }
            operatorsStack.push(token);  // Wkładamy nawias otwierający na stos operatorów.
        }
        // Obsługa nawiasów zamykających.
        else if (token == ')') {
            // Wykonujemy operacje do momentu napotkania nawiasu otwierającego.
            while (!operatorsStack.empty() && operatorsStack.top() != '(') {
                double numberB = valuesStack.top(); valuesStack.pop();
                double numberA = valuesStack.top(); valuesStack.pop();
                char operatorChar = operatorsStack.top(); operatorsStack.pop();
                valuesStack.push(applyBinaryOperator(numberA, numberB, operatorChar));
            }
            // Sprawdzamy, czy nawiasy są dobrze sparowane.
            if (operatorsStack.empty() || operatorsStack.top() != '(') {
                throw runtime_error("Mismatched parentheses");
            }
            operatorsStack.pop();  // Usuwamy nawias otwierający ze stosu.
        }
        // Obsługa operatorów unarnych (np. !, ~)
        else if (token == '!' || token == '~') {
            double value = valuesStack.top();
            valuesStack.pop();
            valuesStack.push(applyUnaryOperator(value, token));  // Stosujemy operator unarny.
        }
        // Obsługa operatorów binarnych.
        else if (isOperator(token)) {
            // Sprawdzamy priorytet operatorów i wykonujemy operacje, jeśli to konieczne.
            while (!operatorsStack.empty() && getOperatorPrecedence(operatorsStack.top()) >= getOperatorPrecedence(token)) {
                double numberB = valuesStack.top(); valuesStack.pop();
                double numberA = valuesStack.top(); valuesStack.pop();
                char operatorChar = operatorsStack.top(); operatorsStack.pop();
                valuesStack.push(applyBinaryOperator(numberA, numberB, operatorChar));
            }
            operatorsStack.push(token);  // Wkładamy operator na stos.
        } else {
            throw invalid_argument("Invalid character in expression");  // Zgłaszamy błąd, jeśli token jest niepoprawny.
        }
        prevToken = token; // Aktualizujemy poprzedni token.
    }

    // Wykonujemy wszystkie pozostałe operacje na końcu, jeżeli jakieś zostały na stosie.
    while (!operatorsStack.empty()) {
        double numberB = valuesStack.top(); valuesStack.pop();
        double numberA = valuesStack.top(); valuesStack.pop();
        char operatorChar = operatorsStack.top(); operatorsStack.pop();
        valuesStack.push(applyBinaryOperator(numberA, numberB, operatorChar));
    }

    // Sprawdzamy, czy na stosie pozostała tylko jedna wartość, która będzie wynikiem.
    if (valuesStack.size() != 1) {
        throw runtime_error("Error in expression evaluation");
    }

    // Zwracamy wynik (jedyną pozostałą wartość na stosie).
    return valuesStack.top();
}

int main() {
    string input;
    getline(cin, input);

    try {
        double result = evaluateExpression(input);
        cout << result << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
