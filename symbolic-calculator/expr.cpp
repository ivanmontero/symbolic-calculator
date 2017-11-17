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
	std::cout << "Original: " << expr << std::endl;
	this->root = parse(expr);	
}

// TODO: Support decimal point
// STD::STRING::END IS ONE PAST LAST CHARACTER

// COMPLETELY DIFFERENT ALGORITHM:
// Take string
// Format into a space-seperated in-fix expression
// utilize the shunting-yard algoritm to convert the expression to post-fix 
// Create expression tree
std::shared_ptr<ExprNode> Expr::parse(std::string expr) {
	// -------- String preparation -> in-fix form --------
	// TODO LIST:
	// - Support variables (x, y, theta, etc)
	// - Support side-by-side input (Ex: 4x instead of 4*x)
	//		-- Requires check of numbers and variables
	// - Place parentheses around bare functions (ex: sinx)
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
			//TODO: Test if special character
		

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
	/*while (!infix.empty()) {
		std::cout << infix.front() << " ";
		infix.pop();
	}
	std::cout << std::endl;*/

	// -------- Shunting-Yard Algorithm -> post-fix form --------
	// Assume if the first char of string is number, rest is a number? Yes.
	// Assume the prep properly placed multiplication symbols.
	//	- No numbers right next to a function?
	//	- ASSUMES all functions must have parentheses
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
				&& index_of(OPERATORS, ops.top()[0]) >= index_of(OPERATORS, s[0]) 
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
	}
	while (!ops.empty()) {
		postfix.push(ops.top());
		ops.pop();
	}

	while (!postfix.empty()) {
		std::cout << postfix.front() << " ";
		postfix.pop();
	}
	std::cout << std::endl;

	// shunting yard straight to symbolic
	// Numbers as doubles
	return std::shared_ptr<ExprNode>();
}

//std::shared_ptr<ExprNode> Expr::parse(std::string expr) {
//	expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end()); // Removes whitespace
//	std::stack<std::shared_ptr<ExprNode>> s;
//	std::shared_ptr<ExprNode> root = std::make_shared<ExprNode>();
//	s.push(root);
//	std::shared_ptr<ExprNode> curr = root;
//	for (std::string::iterator it = expr.begin(); it != expr.end(); it++) {
//		if (*it == '(') {
//			curr->left = std::make_shared<ExprNode>();
//			s.push(curr);
//			curr = curr->left;
//		}
//		else if (in_array(OPERATORS, *it)) {
//			if (curr->data.empty()) {	// In case it encounters an edge
//				curr->data = *it;
//				curr->right = std::make_shared<ExprNode>();
//				s.push(curr);
//				curr = curr->right;
//			}
//			else {
//				// Two cases: 
//				// * curr is top
//				// * curr is in middle
//				if (s.size() <= 1) {
//					std::shared_ptr<ExprNode> temp = std::make_shared<ExprNode>(std::string(1, *it));
//					temp->left = root;
//					if (!s.empty()) s.pop();
//					temp->right = std::make_shared<ExprNode>();
//					s.push(temp);
//					curr = temp->right;
//					root = temp;
//				}
//				else {
//					//std::cout << "entered" << std::endl;
//					std::shared_ptr<ExprNode> temp = std::make_shared<ExprNode>(std::string(1, *it));
//					temp->left = curr->right;
//					//s.pop();
//					temp->right = std::make_shared<ExprNode>();
//					curr->right = temp;
//					s.push(temp);
//					curr = temp->right;
//					//curr = temp->right;
//					//root = temp;
//				}
//			}
//		}
//		else if (in_array(NUMBERS, *it)) {
//			//curr->data = (int)(*it - '0'); // To number
//			curr->data = *it;
//			curr = s.top();
//			s.pop();
//		}
//		else if (*it == ')') {
//			if (!s.empty()) {
//				curr = s.top();
//				s.pop();
//			}
//		}
//		else {
//			std::cerr << "invalid token encountered" << std::endl;
//		}
//		print(root);
//		std::cout << std::endl;
//	}
//	std::cout << std::endl << std::endl;
//
//	return root;
//}

void Expr::print() {
	print(root);
	std::cout << std::endl;
}

void Expr::print(std::shared_ptr<ExprNode> root) {
	if (root) {
		//TODO: Fix enclose
		//      * Find out how to see if a "shared_ptr" is null
		// WORKS!!!
		bool enclose = root->left.get() != 0 && root->right.get() != 0
			&& !root->left->data.empty() && !root->right->data.empty();

		if(enclose) std::cout << "(";
		if (root->left) print(root->left);
		std::cout << root->data;
		if (root->right) print(root->right);
		if (enclose) std::cout << ")";
	}
}

template<class T, class E> bool Expr::in_array(T & arr, E & element) { 
	return std::find(std::begin(arr), std::end(arr), element) != std::end(arr);
}

template<class T, class E> int Expr::index_of(T & arr, E & element) {
	return std::distance(std::begin(arr), std::find(std::begin(arr), std::end(arr), element));
}

//
//If the current token is a '(', add a new node as the left child of the current node, and descend to the left child.
//If the current token is in the list['+', '-', '/', '*'], set the root value of the current node to the operator represented by the current token.Add a new node as the right child of the current node and descend to the right child.
//If the current token is a number, set the root value of the current node to the number and return to the parent.
//If the current token is a ')', go to the parent of the current node.

//testing purposes
int main() { 

	// Trouble case
	Expr e("(12   .   34 .+ (2 * 3 ^ 4) ^ 5)^6"); 


	Expr f("(1 + (2 ^ 3 * 4 ^ 5) ^ 6)^7");

	Expr g("sin(1 +    x +     cos(2 ^ ln3 * 4 ^ 5) ^ 6)^7");

	Expr d("sin(cos(2)/3*3.1415)");
	

	//std::cout << (int)('5' - '0') << std::endl;

	//Symbolic x("x");
	////Symbolic f = x + 1;
	//f = sin(f ^ 2);
	//std::cout << df(f, x) << std::endl;

	std::cin.get();

}