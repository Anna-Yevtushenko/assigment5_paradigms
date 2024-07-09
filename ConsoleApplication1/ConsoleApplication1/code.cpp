#include <iostream>
#include <string>
#include <cmath>
#include <cstring>

using namespace std;

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    return a / b;
}

double pow_function(double a, double b) {
    return pow(a, b);
}

double abs_function(double a, double b) {
    return fabs(a); // fabs використовується для float/double , abs для int
}

double max_function(double a, double b) {
    return max(a, b);
}

double min_function(double a, double b) {
    return min(a, b);
}

bool continue_program() {
    char input[20];
    while (true) {
        cout << "Do you want to continue? (write 'n' to quit or 'y' to continue): ";
        cin >> input;
        if (strcmp(input, "n") == 0) {
            cout << "Goodbye :) \n";
            return false;
        }
        else if (strcmp(input, "y") == 0) {
            return true;
        }
        else {
            cout << "Invalid input. Write 'n' to quit or 'y' to continue \n";
        }
    }
}

int main() {
    while (true) {
        double a;
        double b;
        string operation;

        cout << "Enter two numbers: ";
        cin >> a >> b;

        cout << "Enter operation (+, -, *, /, pow, abs, max, min): ";
        cin >> operation;

        double result;
        bool valid_operation = true;

        switch (operation[0]) {
        case '+':
            result = add(a, b);
            break;
        case '-':
            result = subtract(a, b);
            break;
        case '*':
            result = multiply(a, b);
            break;
        case '/':
            result = divide(a, b);
            break;
        default:
            valid_operation = false;
        }

        if (!valid_operation) {
            if (operation == "pow") {
                result = pow_function(a, b);
                valid_operation = true;
            }
            else if (operation == "abs") {
                result = abs_function(a, b);
                valid_operation = true;
            }
            else if (operation == "max") {
                result = max_function(a, b);
                valid_operation = true;
            }
            else if (operation == "min") {
                result = min_function(a, b);
                valid_operation = true;
            }
            else {
                cout << "Unknown operation!" << endl;
                valid_operation = false;
            }
        }

        if (valid_operation) {
            cout << "Result: " << result << endl;
        }

        if (!continue_program()) {
            break; 
        }
    }

    return 0;
}
