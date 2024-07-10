#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <regex>
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

		else if (isalpha(ch)) {
			token += ch;
			while (i + 1 < expression.size() && isalpha(expression[i + 1])) {
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
		cout << "Enter an expression (3 + 5: ";
		getline(cin, expression);

		vector<string> tokens = tokenize(expression);

		cout << "Tokens: ";
		for (const auto& token : tokens) {
			cout << token << " ";
		}
		cout << endl;


		return 0;
	}
}