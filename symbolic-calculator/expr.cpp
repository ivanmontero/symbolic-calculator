#include <iostream>
#include <algorithm>
#include <stack>
//#include <vector>
#include <queue>
#include <stack>

#include "expr.h"
#include "symbolicc++.h"

const std::string const Expr::FUNCTIONS[4] = { "sin", "cos", "tan", "ln" };
const char const Expr::PARENTHESES[2] = { '(', ')' };
// PRECEDENCE = index / 2
const char const Expr::OPERATORS[5] = { '+','-','*','/','^' }; 
const char const Expr::NUMBERS[11] = { '0','1','2','3','4','5','6','7','8','9', '.'};

Expr::Expr(std::string expr) {
	parse(expr);
}

// STD::STRING::END IS ONE PAST LAST CHARACTER

// COMPLETELY DIFFERENT ALGORITHM:
// Take string
// Format into a space-seperated in-fix expression
// utilize the shunting-yard algoritm to convert the expression to post-fix 
// Create expression tree
void Expr::parse(std::string expr) {
	// -------- String preparation -> in-fix form --------
	// TODO LIST:
	// - Support variables (x, y, theta, etc)
	// - Properly placed multiplication symbols.
	//		- Support side-by-side input (Ex: 4x instead of 4*x)
	//			-- Requires check of numbers and variables
	//		- Place parentheses around bare functions (ex: sinx)
	// 
	// ---------------------------------------------------
	// Removes whitespace
	expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
	// Produces infix vector
	std::queue<std::string> infix;
	for (std::string::iterator it = expr.begin(); it != expr.end();) {
		// Numbers - can be more than one digit
		// TODO: SUPPORT FOR PI OR E
		if (in_array(NUMBERS, *it)) {
			std::string n;
			while (it != expr.end() && in_array(NUMBERS, *it)) {
				// Makes sure that there isn't multiple decimal points. 
				// Multiple after first encountered disregarded.
				if (*it != '.' || n.find('.') == std::string::npos)
					n.append(std::string(1, *it));
				it++;
			}
			infix.push(n);
			it--;
		} else if (in_array(OPERATORS, *it) || in_array(PARENTHESES, *it)) {
			infix.push(std::string(1, *it));
		} else {
			// var to avoid unnessessary checks.
			bool found = false;
			// Test if function
			for (std::string f : FUNCTIONS) {
				// Makes sure theres enough room left in expr to test
				if (std::distance(it, expr.end()) >= f.size()) {
					// Check for equivalence
					if (f.compare(std::string(it, it + f.size())) == 0) {
						infix.push(f);
						it += f.size() - 1;
						found = true;
						break;
					}
				}
			}
			//TODO: Test if special character (e, pi, etc)
		

			// Treat "unknown characters" as variables
			if (!found) {
				infix.push(std::string(1, *it));
			}

			//if (i >= sizeof(FUNCTIONS) / sizeof(std::string))
			//	std::cerr << "INVALID CHARACTER ENCOUNTERED" << std::endl;
		}
		if (it != expr.end())
			it++;
	}

	std::cout << "infix    : ";
	for (int i = 0; i < infix.size(); i++) {
		std::cout << infix.front() << " ";
		infix.push(infix.front());
		infix.pop();
	}
	std::cout << std::endl;

	// -------- Shunting-Yard Algorithm -> post-fix form --------
	// TODO LIST:
	// - (better) Support variables (x, y, theta, etc)
	// - 
	// ASSUMPTION LIST: (Assumes previous algorithm accounts for these)
	// - if the first char of string is number, rest is a number
	// - No numbers right next to a function (explicit multiplication)
	// - ALL functions must have parentheses
	// ---------------------------------------------------

	// Assume ? Yes.
	// Assume the prep 
	//	- 
	//	- 
	std::queue<std::string> postfix;
	std::stack<std::string> ops;
	while (!infix.empty()) {
		std::string s = infix.front();
		infix.pop();
		// Numbers pushed right into the stack
		if (in_array(NUMBERS, s[0])) postfix.push(s);
		// If function, pushed right into ops stack. Assumed that openning
		// parenthesis will follow
		else if (in_array(FUNCTIONS, s)) ops.push(s);
		else if (in_array(OPERATORS, s[0])) {
			// Should NEVER encounter a function when popping
			while (!ops.empty() 
					&& ops.top()[0] != '('
					&& index_of(OPERATORS, ops.top()[0])/2 >= index_of(OPERATORS, s[0])/2
					&& s[0] != '^') {
				postfix.push(ops.top());
				ops.pop();
			}
			ops.push(s);
		} else if (s[0] == '(')
			ops.push(s);
		else if (s[0] == ')') {
			while (ops.top()[0] != '(') {
				postfix.push(ops.top());
				ops.pop();
			}
			ops.pop();
			if (!ops.empty() && in_array(FUNCTIONS, ops.top())) {
				postfix.push(ops.top());
				ops.pop();
			}
		}
		else
			postfix.push(s); // Variable
	}
	while (!ops.empty()) {
		postfix.push(ops.top());
		ops.pop();
	}

	std::cout << "postfix  : ";
	for (int i = 0; i < postfix.size(); i++) {
		std::cout << postfix.front() << " ";
		postfix.push(postfix.front());
		postfix.pop();
	}
	std::cout << std::endl << std::endl;

	// shunting yard straight to symbolic
	// Numbers as doubles
}

template<class T, class E> bool Expr::in_array(T & arr, E & element) { 
	return std::find(std::begin(arr), std::end(arr), element) != std::end(arr);
}

template<class T, class E> int Expr::index_of(T & arr, E & element) {
	return std::distance(std::begin(arr), std::find(std::begin(arr), std::end(arr), element));
}

//testing purposes
int main() { 

	// Trouble case
	Expr e("(12   .   34 .+ (2 * 3 ^ 4) ^ 5)^6"); 


	Expr f("(1 + (2 ^ 3 * 4 ^ 5) ^ 6)^7");

	Expr g("sin(1 +    x +     cos(2 ^ ln3 * 4 ^ 5) ^ 6)^7");

	Expr d("sin(cos(2)/3*3.1415)");

	Expr n("3+4*2/(1-5)^2^3");
	Expr c("ln(x^243*y+7000/(320^90)^50*e^(253*x^5))");

	std::cin.get();

}
