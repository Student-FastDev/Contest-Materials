#include <iostream>
#include <string>

using namespace std;

// Klasa bazowa z szablonem.
template <typename T>
class Base {
public:
    Base(T value) : value(value) {}
    
    void setValue(T value) {
        this->value = value;
    }
    
    T getValue() const {
        return value;
    }
    
    void printValue() const {
        cout << "Value: " << value << endl;
    }

private:
    T value;
};

// Klasa dziedzicząca po Base z typem int.
class IntChild : public Base<int> {
public:
    IntChild(int value) : Base<int>(value) {}
    
    void incrementValue() {
        setValue(getValue() + 1);
    }
};

// Klasa dziedzicząca po Base z typem string.
class StringChild : public Base<string> {
public:
    StringChild(string value) : Base<string>(value) {}
    
    void appendString(const string& str) {
        setValue(getValue() + str);
    }
};

int main() {
    IntChild intChild(10);
    intChild.printValue();
    intChild.incrementValue();
    intChild.printValue();
    
    StringChild stringChild("Hello");
    stringChild.printValue();
    stringChild.appendString(" World");
    stringChild.printValue();
    
    return 0;
}
