#include <iostream>
#include <algorithm>
#include <stack>
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
	this->expr = parse(expr);
	std::cout << "df/dx      : " << this->expr->df(Symbolic("x")) << std::endl;
	std::cout << "df/dy      : " << this->expr->df(Symbolic("y")) << std::endl;
	std::cout << "int(f)dx   : " << this->expr->integrate(Symbolic("x")) << std::endl;
	std::cout << "int(f)dy   : " << this->expr->integrate(Symbolic("y")) << std::endl;
	std::cout << "int(f)dx|x=4     : " << this->expr->integrate(Symbolic("x"))[Symbolic("x")==4] << std::endl;
	std::cout << "int(f)dx|x=4,y=2 : " << this->expr->integrate(Symbolic("x"))[Symbolic("x") == 4, Symbolic("y") == 2] << std::endl;
	std::cout << std::endl;
}

std::shared_ptr<Symbolic> Expr::parse(std::string expr) {
	std::cout << "Original   : " << expr << std::endl;
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

	std::cout << "infix      : ";
	for (int i = 0; i < infix.size(); i++) {
		std::cout << infix.front() << " ";
		infix.push(infix.front());
		infix.pop();
	}
	std::cout << std::endl;

	// -------- Shunting-Yard Algorithm -> post-fix form --------
	// TODO LIST:
	// - (better) Support variables (x, y, theta, etc)
	// ASSUMPTION LIST: (Assumes previous algorithm accounts for these)
	// - if the first char of string is number, rest is a number
	// - No numbers right next to a function (explicit multiplication)
	// - ALL functions must have parentheses
	// -----------------------------------------------------------
	std::queue<std::string> postfix;
	std::stack<std::string> ops;
	while (!infix.empty()) {
		std::string s = infix.front();
		infix.pop();
		// Numbers pushed right into the stack
		if (in_array(NUMBERS, s[0])) {
			postfix.push(s);
			// If function, pushed right into ops stack. Assumed that openning
			// parenthesis will follow
		} else if (in_array(FUNCTIONS, s)) { 
			ops.push(s);
		} else if (in_array(OPERATORS, s[0])) {
			// Should NEVER encounter a function when popping
			while (!ops.empty() 
					&& ops.top()[0] != '('
					&& index_of(OPERATORS, ops.top()[0])/2 >= index_of(OPERATORS, s[0])/2
					&& s[0] != '^') {
				postfix.push(ops.top());
				ops.pop();
			}
			ops.push(s);
		} else if (s[0] == '(') {
			ops.push(s);
		} else if (s[0] == ')') {
			while (ops.top()[0] != '(') {
				postfix.push(ops.top());
				ops.pop();
			}
			ops.pop();
			if (!ops.empty() && in_array(FUNCTIONS, ops.top())) {
				postfix.push(ops.top());
				ops.pop();
			}
		} else {
			postfix.push(s); // Variable
		}
	}
	while (!ops.empty()) {
		postfix.push(ops.top());
		ops.pop();
	}

	std::cout << "postfix    : ";
	for (int i = 0; i < postfix.size(); i++) {
		std::cout << postfix.front() << " ";
		postfix.push(postfix.front());
		postfix.pop();
	}
	std::cout << std::endl;

	// -------- post-fix evaluator -> SymbolicC++ Object Creation --------
	// TODO LIST:
	// - Everything
	// - Optimize (not use tons of symbolic objects)
	// PROBLEMS:
	// - Stack to store Symbolic object AND strings
	// -------------------------------------------------------------------
	std::stack<Symbolic> ss;
	while (!postfix.empty()) {
		std::string s = postfix.front();
		postfix.pop();
		if (in_array(NUMBERS, s[0])) {
			ss.push(Symbolic(std::stod(s)));
		} else if (in_array(OPERATORS, s[0])) {
			Symbolic r = ss.top(), l;
			ss.pop();
			l = ss.top();
			ss.pop();
			// '+','-','*','/','^'
			if		(s[0] == '+') ss.push(l + r);
			else if (s[0] == '-') ss.push(l - r);
			else if (s[0] == '*') ss.push(l * r);
			else if (s[0] == '/') ss.push(l / r);
			else if (s[0] == '^') ss.push(l ^ r);
		} else if (in_array(FUNCTIONS, s)) {
			Symbolic n = ss.top();
			ss.pop();
			// "sin", "cos", "tan", "ln"
			if		(s.compare("sin") == 0)	ss.push(sin(n));
			else if (s.compare("cos") == 0) ss.push(cos(n));
			else if (s.compare("tan") == 0) ss.push(tan(n));
			else if (s.compare("ln") == 0)  ss.push(ln(n));
		} else { // variables
			ss.push(Symbolic(s));
		}
	}
	std::cout << "simplified : " << ss.top() << std::endl;

	
	return std::make_shared<Symbolic>(ss.top());
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
	//Expr e("(12   .   34 .+ (2 * 3 ^ 4) ^ 5)^6"); 


	//Expr f("(1 + (2 ^ 3 * 4 ^ 5) ^ 6)^7");

	//Expr g("sin(1 +    x +     cos(2 ^ ln3 * 4 ^ 5) ^ 6)^7");

	//Expr d("sin(cos(2)/3*3.1415)");

	//Expr n("3+4*2/(1-5)^2^3");

	Expr c("ln(x^24.3*y+7000/(320^90)^50*e^(253*x^5))");
	Expr cc("(5*2*x)^4");
	Expr cd("x^2");

	Symbolic x("x");
	Symbolic v = (x + 1, x + 1, x + 1);
	Symbolic u = (x + 2, x + 3, x + 4);
	std::cout << v << std::endl;
	std::cout << (v % u) << std::endl;

	//Symbolic num1(1);
	//Symbolic num2(2);
	//Symbolic test = num1 ^ num2;

	//std::cout << test << std::endl;

	std::cin.get();

}