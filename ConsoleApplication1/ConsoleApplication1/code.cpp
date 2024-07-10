#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <stdexcept>
#include <map>
#include <limits>
#include <cctype> 

using namespace std;
map<string, double> variables;

vector<string> tokenize(const string& expression) {
	vector<string> tokens;
	string token;
	for (size_t i = 0; i < expression.size(); ++i) {
		char ch = expression[i];
		if (isspace(ch)) { // Skip spaces
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
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')') {
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
	//throw invalid_argument("Invalid operator");
	cout << "Invalid operator";
}

double get_value(const string& token) {
	if (!token.empty() && isdigit(token[0])) {
		return stod(token);
	} else if (variables.find(token) != variables.end()) {
		return variables[token];
	} else {
		cout << "Unknown variable: " + token << endl;
		return nan(""); 
	}
}

void process_operator(stack<double>& values, stack<char>& operators) {
	if (values.size() < 2) throw runtime_error("Invalid expression");
	double b = values.top(); values.pop();
	double a = values.top(); values.pop();
	char oper = operators.top(); operators.pop();
	values.push(operation(a, b, oper));
}


void process_parentheses(stack<double>& values, stack<char>& operators) {
	while (!operators.empty() && operators.top() != '(') {
		process_operator(values, operators);
	}
	if (!operators.empty()) operators.pop(); // Remove the '(' from stack
}

void process_token(const string& token, stack<double>& values, stack<char>& operators) {
	if (!token.empty() && isdigit(token[0])) {
		values.push(get_value(token));
	} else if (token == "(") {
		operators.push(token[0]);
	} else if (token == ")") {
		process_parentheses(values, operators);
	} else if (token.size() == 1 && strchr("+-*/^", token[0])) {
		while (!operators.empty() && priority(operators.top()) >= priority(token[0])) {
			process_operator(values, operators);
		}
		operators.push(token[0]);
	}
}

double shunting_yard(const vector<string>& tokens) {
	stack<double> values;
	stack<char> operators;

	for (const auto& token : tokens) {
		process_token(token, values, operators);
	}

	while (!operators.empty()) {
		process_operator(values, operators);
	}

	if (values.size() != 1) throw runtime_error("Invalid expression");
	return values.top();
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
			cin.ignore();

			return true;
		}
		else {
			cout << "Invalid input. Write 'n' to quit or 'y' to continue \n";
			cin.clear();
			cin.ignore();
		}
	}
}

int main() {
	while (true) {
		string expression;
		cout << "Enter an expression: ";
		getline(cin, expression);
		vector<string> tokens = tokenize(expression);
		cout << "Tokens: ";
		for (const auto& token : tokens) {
			cout << token << " ";
		}
		cout << endl;
		try {
			if (tokens.size() > 2 && tokens[1] == "=") {
				string variable = tokens[0];
				vector<string> expression_tokens(tokens.begin() + 2, tokens.end());
				double result = shunting_yard(expression_tokens);
				variables[variable] = result;
				cout << variable << " = " << result << endl;
			}
			else {
				double result = shunting_yard(tokens);
				cout << "Result: " << result << endl;
			}
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