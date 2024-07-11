#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <stdexcept>
#include <map>
#include <limits>
#include <cctype>
#include <algorithm>

using namespace std;

map<string, double> expression_results;

bool is_custom_function(const string& token) {
    return token == "min" || token == "max";
}

vector<string> tokenize(const string& expression) {
    vector<string> tokens;
    string token;
    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];
        if (isspace(ch)) {
            continue;
        }
        if (isdigit(ch) || ch == '.') {
            token += ch;
            while (i + 1 < expression.size() && (isdigit(expression[i + 1]) || expression[i + 1] == '.')) {
                token += expression[++i];
            }
            tokens.push_back(token);
            token.clear();
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')' || ch == ',') {
            tokens.push_back(string(1, ch));
        }
    }
    return tokens;
}

int priority(char oper) {
    if (oper == '+' || oper == '-')
        return 1;
    if (oper == '*' || oper == '/')
        return 2;
    if (oper == '^')
        return 3;
    return 0;
}


double operation(double a, double b, char oper) {
    if (oper == '+') 
        return a + b;
    if (oper == '-')
        return a - b;
    if (oper == '*')
        return a * b;
    if (oper == '/')
        return a / b;
    if (oper == '^') 
        return pow(a, b);
    cout << "Invalid operator" << endl;
    return nan("");
}


double get_value(const string& token) {
    if (!token.empty() && isdigit(token[0])) {
        return stod(token);
    }
    else if (expression_results.find(token) != expression_results.end()) {
        return expression_results[token];
    }
    else {
        cout << "Unknown variable or expression: " + token << endl;
        return nan("");
    }
}

void process_operator(stack<double>& values, stack<char>& operators) {
    if (values.size() < 2) throw runtime_error("Invalid expression");
    double value2 = values.top(); values.pop();
    double value1 = values.top(); values.pop();
    char oper = operators.top(); operators.pop();
    values.push(operation(value1, value2, oper));
}


double max_function(const vector<double>& args) {
    if (args.empty()) throw invalid_argument("No arguments provided to max");
    double max_val = args[0];
    for (double arg : args) {
        if (arg > max_val) {
            max_val = arg;
        }
    }
    return max_val;
}

double min_function(const vector<double>& args) {
    if (args.empty()) throw invalid_argument("No arguments provided to min");
    double min_val = args[0];
    for (double arg : args) {
        if (arg < min_val) {
            min_val = arg;
        }
    }
    return min_val;
}


void process_parentheses(stack<double>& values, stack<char>& operators) {
    while (!operators.empty() && operators.top() != '(') {
        process_operator(values, operators);
    }
    if (!operators.empty()) operators.pop();
}


void process_token(const string& token, stack<double>& values, stack<char>& operators, stack<string>& functions) {
    if (!token.empty() && isdigit(token[0])) {
        values.push(get_value(token));
    }
    else if (token == "(") {
        operators.push(token[0]);
    }
    else if (token == ")") {
        process_parentheses(values, operators);
    }
    else if (token.size() == 1 && strchr("+-*/^,", token[0])) {
        while (!operators.empty() && priority(operators.top()) >= priority(token[0])) {
            process_operator(values, operators);
        }
        if (token[0] != ',') {
            operators.push(token[0]);
        }
    }
    else if (is_custom_function(token)) {
        functions.push(token);
        operators.push('(');
    }
}


double shunting_yard(const vector<string>& tokens) {
    stack<double> values;
    stack<char> operators;
    stack<string> functions;

    for (const auto& token : tokens) {
        process_token(token, values, operators, functions);
    }

    while (!operators.empty()) {
        process_operator(values, operators);
    }

    if (values.size() != 1) throw runtime_error("Invalid expression");
    return values.top();
}

// Обчислення результату функції до токенізації
double evaluate_function(const string& function, const string& args_str) {
    vector<string> args_tokens = tokenize(args_str);
    vector<double> args_values;

    string current_arg;
    for (const auto& token : args_tokens) {
        if (token == ",") {
            if (!current_arg.empty()) {
                args_values.push_back(stod(current_arg));
                current_arg.clear();
            }
        }
        else {
            current_arg += token;
        }
    }
    if (!current_arg.empty()) {
        args_values.push_back(stod(current_arg));
    }

    if (function == "max") {
        return max_function(args_values);
    }
    else if (function == "min") {
        return min_function(args_values);
    }
    else {
        throw invalid_argument("Unknown function: " + function);
    }
}


string preprocess_expression(const string& expression) {
    string new_expression = expression;
    size_t pos = new_expression.find("max(");
    while (pos != string::npos) {
        size_t start_pos = pos + 4;
        size_t end_pos = new_expression.find(')', start_pos);
        if (end_pos == string::npos) {
            throw invalid_argument("Invalid function format");
        }
        string args_str = new_expression.substr(start_pos, end_pos - start_pos);
        double result = evaluate_function("max", args_str);
        new_expression.replace(pos, end_pos - pos + 1, to_string(result));
        pos = new_expression.find("max(", pos + 1);
    }

    pos = new_expression.find("min(");
    while (pos != string::npos) {
        size_t start_pos = pos + 4;
        size_t end_pos = new_expression.find(')', start_pos);
        if (end_pos == string::npos) {
            throw invalid_argument("Invalid function format");
        }
        string args_str = new_expression.substr(start_pos, end_pos - start_pos);
        double result = evaluate_function("min", args_str);
        new_expression.replace(pos, end_pos - pos + 1, to_string(result));
        pos = new_expression.find("min(", pos + 1);
    }

    return new_expression;
}

bool continue_program() {
    char input[80];
    while (true) {
        cout << "Do you want to continue? (write 'n' to quit or 'y' to continue): ";
        cin >> input;
        if (strcmp(input, "n") == 0) {
            cout << "Goodbye :) \n";
            return false;
        }
        else if (strcmp(input, "y") == 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }
        else {
            cout << "Invalid input. Write 'n' to quit or 'y' to continue \n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    while (true) {
        string expression;
        cout << "Enter an expression: ";
        getline(cin, expression);

        try {
            string preprocessed_expression = preprocess_expression(expression);
            vector<string> tokens = tokenize(preprocessed_expression);

            double result = shunting_yard(tokens);
            expression_results[expression] = result;
            cout << "Result: " << result << endl;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }

        if (!continue_program()) {
            break;
        }
    }
    return 0;
}
