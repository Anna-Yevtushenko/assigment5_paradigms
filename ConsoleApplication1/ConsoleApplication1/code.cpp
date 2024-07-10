#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <regex>
#include <cmath>
#include <stdexcept>
#include <map>
#include <limits>

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
	return fabs(a);
}

double max_function(double a, double b) {
	return max(a, b);
}

double min_function(double a, double b) {
	return min(a, b);
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
		cout << "Enter an expression (e.g., 3 + 5 or a = 3 + 5): ";
		getline(cin, expression);

		vector<string> tokens = tokenize(expression);

		cout << "Tokens: ";
		for (const auto& token : tokens) {
			cout << token << " ";
		}
		cout << endl;


		//	switch (operation[0]) {
		//	case '+':
		//		result = add(a, b);
		//		break;
		//	case '-':
		//		result = subtract(a, b);
		//		break;
		//	case '*':
		//		result = multiply(a, b);
		//		break;
		//	case '/':
		//		result = divide(a, b);
		//		break;
		//	default:
		//		valid_operation = false;
		//	}
		//	if (!valid_operation) {
		//		if (operation == "pow") {
		//			result = pow_function(a, b);
		//			valid_operation = true;
		//		}
		//		else if (operation == "abs") {
		//			result = abs_function(a, b);
		//			valid_operation = true;
		//		}
		//		else if (operation == "max") {
		//			result = max_function(a, b);
		//			valid_operation = true;
		//		}
		//		else if (operation == "min") {
		//			result = min_function(a, b);
		//			valid_operation = true;
		//		}
		//		else {
		//			cout << "Unknown operation!" << endl;
		//			valid_operation = false;
		//		}
		//	}

		//	if (valid_operation) {
		//		cout << "Result: " << result << endl;
		//	}

		//	if (!continue_program()) {
		//		break;
		//	}
		//}

		return 0;
	}
}