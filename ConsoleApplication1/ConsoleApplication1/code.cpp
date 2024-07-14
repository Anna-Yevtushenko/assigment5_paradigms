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
map<string, double> expression_results_map = {
    {"a", 1.0}};

bool is_custom_function(const string& token) {
    return token == "min" || token == "max" || token == "abs" || token == "pow";
}

vector<string> tokenize(const string& expression) {
    vector<string> tokens;
    string token;
    for ( int i = 0; i < expression.size(); ++i) {
        char ch = expression[i];
        if (ch == ' ') { 
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
        else if (isalpha(ch)) {
            token += ch;
            while (i + 1 < expression.size() && (isalpha(expression[i + 1]) || isdigit(expression[i + 1]))) {
                token += expression[++i];
            }
            tokens.push_back(token);
            token.clear();
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
    else if (expression_results_map.find(token) != expression_results_map.end()) { //check that key is exist
        return expression_results_map[token];
    }
    else {
        cout << "Unknown variable or expression: " << token << endl;
        return nan("");//bcs we need return double
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
    if (args.empty()) {
        cout << "Error: No arguments provided to max" << endl;
        return nan(""); 
    }
    double max_val = args[0];
    for (double arg : args) { //range-based for loop
        if (arg > max_val) {
            max_val = arg;
        }
    }
    return max_val;
}

double min_function(const vector<double>& args) {
    if (args.empty()) {
        cout << "Error: No arguments provided to min" << endl;
        return nan(""); 
    }
    double min_val = args[0];
    for (double arg : args) {
        if (arg < min_val) {
            min_val = arg;
        }
    }
    return min_val;
}

double pow_function(const vector<double>& args) {
    if (args.size() != 2) {
        cout << "Error: pow function requires exactly 2 arguments" << endl;
        return nan("");
    }
    return pow(args[0], args[1]);
}

double abs_function(const std::vector<double>& args) {
    if (args.size() != 1) {
        cout << "Error: abs function requires exactly 1 argument" << endl;
        return nan("");
    }
    return abs(args[0]);
}

void process_parentheses(stack<double>& values, stack<char>& operators) {
    while (!operators.empty() && operators.top() != '(') {
        process_operator(values, operators);
    }
    if (!operators.empty()) operators.pop();
}

void process_token(const string& token, stack<double>& values, stack<char>& operators, stack<string>& functions) {
    if (!token.empty() && (isdigit(token[0]) || isalpha(token[0]))) {
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

double evaluate_function(const string& function, const string& args_str) { //для обробки внутрішніх функцій 
    vector<string> argument_tokenization = tokenize(args_str);
    vector<double> argument_values;
    string current_argument_expression; 
    for (const auto& token : argument_tokenization) {
        if (token == ",") {
            if (!current_argument_expression.empty()) {
                double arg_val = shunting_yard(tokenize(current_argument_expression));
                argument_values.push_back(arg_val);
                current_argument_expression.clear();
            }
        }
        else {
            current_argument_expression += token;
        }
    }
    if (!current_argument_expression.empty()) {
        double arg_val = shunting_yard(tokenize(current_argument_expression)); 
        argument_values.push_back(arg_val);
    }
    if (function == "max") {
        return max_function(argument_values);
    }
    else if (function == "min") {
        return min_function(argument_values);
    }
    else if (function == "abs") {
        return abs_function(argument_values);
    }
    else if (function == "pow") {
        return pow_function(argument_values);
    }
    else {
        throw invalid_argument("Unknown function: " + function);
    }
}

void process_functions(stack<size_t>& func_positions, string& new_expression, const vector<string>& functions) {
    for (int i = 0; i < new_expression.size(); ++i) {
        if (new_expression[i] == '(') {
            func_positions.push(i);
        } 
        else if (new_expression[i] == ')') {
            if (!func_positions.empty()) {
                int start_pos = func_positions.top();
                func_positions.pop();
                int end_pos = i;
                string substr_before_parenthesis = new_expression.substr(0, start_pos);
                for (const auto& func : functions) {
                    if (substr_before_parenthesis.size() >= func.size() &&
                        substr_before_parenthesis.substr(substr_before_parenthesis.size() - func.size()) == func) {
                        //повертає підрядок починаючи з позиції 6 там де внутрішня функція
                        string arguments_between_parenthesis = new_expression.substr(start_pos + 1, end_pos - start_pos - 1);
                        double result = evaluate_function(func, arguments_between_parenthesis);
                        int func_start = start_pos - func.size();//початок функції 
                        int func_length = end_pos - func_start + 1;
                        new_expression.replace(func_start, func_length, to_string(result));

                        i = func_start + to_string(result).size() - 1;
                        break;
                    }
                }
            }
        }
    }
}

string evaluate_and_replace_functions(const string& expression, vector<string>& functions) {
    string new_expression = expression;
    stack<size_t> func_positions;
    process_functions(func_positions, new_expression, functions);
    return new_expression;
}

string preprocess_expression(const string& expression) {
    string new_expression = expression;
    vector<string> functions = { "max", "min", "abs", "pow" };
    bool has_functions = true;
    while (has_functions) {
        string processed_expression = evaluate_and_replace_functions(new_expression, functions);
        if (processed_expression == new_expression) {
            has_functions = false;
        } 
        else {
            new_expression = processed_expression;
        }
    }
    return new_expression;
}

void removeSpaces(string &str) {
    string result;
    for (char c : str) {
        if (!isspace(c)) {
            result += c; 
        }
    }
    str = result; 
}


double evaluate_expression(const string& expression) {
    string preprocessed_expression = preprocess_expression(expression);
    vector<string> tokens = tokenize(preprocessed_expression);
    return shunting_yard(tokens);
}


/*bool continue_program() {
char input[80];
while (true) {
cout << "do you want to continue? (write 'n' to quit or 'y' to continue): ";
cin >> input;
if (strcmp(input, "n") == 0) {
cout << "goodbye :) \n";
return false;
}
else if (strcmp(input, "y") == 0) {
cin.clear();
cin.ignore();
return true;
}
else {
cout << "invalid input. write 'n' to quit or 'y' to continue \n";
cin.clear();
cin.ignore();
}
}
}*/



int main() {
    string expression;
    cout << "Enter an expression: ";
    getline(cin, expression);
    int equal_index = expression.find('=');
    if (equal_index != string::npos) {
        string key_name = expression.substr(0, equal_index);
        removeSpaces(key_name);
        string value = expression.substr(equal_index + 1);
        try {
            double result = evaluate_expression(value);
            expression_results_map[key_name] = result;
            cout << key_name << " = " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    } else {
        try {
            double result = evaluate_expression(expression);
            cout << "Result: " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    return 0;
}
