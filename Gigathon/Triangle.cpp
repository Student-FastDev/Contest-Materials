#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

class Triangle {
protected:
    double a, b, c;

public:
    Triangle(double a, double b, double c) : a(a), b(b), c(c) {
        if (!isValid()) {
            throw invalid_argument("Cannot form a triangle with given sides.");
        }
    }

    bool isValid() const {
        return (a + b > c) && (a + c > b) && (b + c > a);
    }

    virtual void draw() const = 0;

    string getType() const {
        if (a == b && b == c) {
            return "Equilateral";
        } else if (a == b || b == c || a == c) {
            return "Isosceles";
        } else if (isRightTriangle()) {
            return "Right";
        } else {
            return "Scalene";
        }
    }

    bool isRightTriangle() const {
        return (abs(a * a + b * b - c * c) < 1e-9) ||
               (abs(a * a + c * c - b * b) < 1e-9) ||
               (abs(b * b + c * c - a * a) < 1e-9);
    }

    void printAngles() const {
        double A = acos((b * b + c * c - a * a) / (2 * b * c)) * 180 / M_PI;
        double B = acos((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI;
        double C = 180 - A - B;
        cout << "Angles: A=" << A << " B=" << B << " C=" << C << endl;
    }
};

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(double side) : Triangle(side, side, side) {}

    void draw() const override {
        cout << "  /\\\n /  \\\n/____\\\n";
    }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(double a, double b, double c) : Triangle(a, b, c) {
        if (!isRightTriangle()) {
            throw invalid_argument("Not a right triangle.");
        }
    }

    void draw() const override {
        cout << "|\n|\\\n| \\\n|__\\\n";
    }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(double a, double b, double c) : Triangle(a, b, c) {
        if (!(a == b || b == c || a == c)) {
            throw invalid_argument("Not an isosceles triangle.");
        }
    }

    void draw() const override {
        cout << "  /\\\n /  \\\n/____\\\n";
    }
};

class ScaleneTriangle : public Triangle {
public:
    ScaleneTriangle(double a, double b, double c) : Triangle(a, b, c) {}

    void draw() const override {
        cout << "  /\\\n /  \\\n/____\\\n";
    }
};

int main() {
    try {
        Triangle* t1 = new EquilateralTriangle(5);
        cout << "Type: " << t1->getType() << endl;
        t1->printAngles();
        t1->draw();
        cout << endl;

        Triangle* t2 = new RightTriangle(3, 4, 5);
        cout << "Type: " << t2->getType() << endl;
        t2->printAngles();
        t2->draw();
        cout << endl;

        Triangle* t3 = new IsoscelesTriangle(5, 5, 8);
        cout << "Type: " << t3->getType() << endl;
        t3->printAngles();
        t3->draw();
        cout << endl;

        Triangle* t4 = new ScaleneTriangle(3, 4, 6);
        cout << "Type: " << t4->getType() << endl;
        t4->printAngles();
        t4->draw();
        cout << endl;

        delete t1;
        delete t2;
        delete t3;
        delete t4;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    return 0;
}
