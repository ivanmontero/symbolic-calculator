#include <iostream>
#include <algorithm>
#include <stack>

#include "expr.h"
#include "symbolicc++.h"

const std::string const Expr::FUNCTIONS[3] = { "sin", "cos", "tan" };
const char const Expr::OPERATORS[5] = { '+','-','*','/','^' };
const char const Expr::NUMBERS[10] = { '0','1','2','3','4','5','6','7','8','9' };

Expr::Expr(std::string expr) {
	this->root = parse(expr);	

}

// TODO: Parse into tree
// Version 1: numbers, parentheses, operators
// Version 2: functions 
// Version 3: Operator precedence
std::shared_ptr<ExprNode> Expr::parse(std::string expr) {
	expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end()); // Removes whitespace
	std::stack<std::shared_ptr<ExprNode>> s;
	std::shared_ptr<ExprNode> root = std::make_shared<ExprNode>();
	s.push(root);
	std::shared_ptr<ExprNode> curr = root;
	for (std::string::iterator it = expr.begin(); it != expr.end(); it++) {
		std::cout << std::endl;
		if (*it == '(') {
			curr->left = std::make_shared<ExprNode>();
			s.push(curr);
			curr = curr->left;
		} else if (in_array(OPERATORS, *it)) {
			if (curr->data.empty()) {	// In case it encounters an edge
				curr->data = *it;
				curr->right = std::make_shared<ExprNode>();
				s.push(curr);
				curr = curr->right;
			} else {
				// Two cases: 
				// * curr is top
				// * curr is in middle
				if (s.size() <= 1) {
					std::shared_ptr<ExprNode> temp = std::make_shared<ExprNode>(std::string(1, *it));
					temp->left = root;
					if(!s.empty()) s.pop();
					temp->right = std::make_shared<ExprNode>();
					s.push(temp);
					curr = temp->right;
					root = temp;
				} else {
					//std::cout << "entered" << std::endl;
					std::shared_ptr<ExprNode> temp = std::make_shared<ExprNode>(std::string(1, *it));
					temp->left = curr->right;
					//s.pop();
					temp->right = std::make_shared<ExprNode>();
					curr->right = temp;
					s.push(temp);
					curr = temp->right;
					//curr = temp->right;
					//root = temp;
				}
			}
		} else if (in_array(NUMBERS, *it)) {
			//curr->data = (int)(*it - '0'); // To number
			curr->data = *it;
			curr = s.top();
			s.pop();
		} else if (*it == ')') {
			if (!s.empty()) {
				curr = s.top();
				s.pop();
			}
		} else {
			std::cerr << "invalid token encountered" << std::endl;
		}
		print(root);
	}

	return root;
}

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


//
//If the current token is a '(', add a new node as the left child of the current node, and descend to the left child.
//If the current token is in the list['+', '-', '/', '*'], set the root value of the current node to the operator represented by the current token.Add a new node as the right child of the current node and descend to the right child.
//If the current token is a number, set the root value of the current node to the number and return to the parent.
//If the current token is a ')', go to the parent of the current node.

//testing purposes
int main() { 

	// Trouble case
	Expr e("(1 + (2 * 3 ^ 4) ^ 5)^6"); 


	Expr f("(1 + (2 ^ 3 * 4 ^ 5) ^ 6)^7");

	//std::cout << (int)('5' - '0') << std::endl;

	/*Symbolic x("x");
	Symbolic f = x + 1;
	f = x * f;
	f = ((f) ^ 2) * x;*/


	std::cin.get();

}