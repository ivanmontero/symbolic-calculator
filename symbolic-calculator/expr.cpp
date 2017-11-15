#include <iostream>
#include <algorithm>
#include <stack>

#include "expr.h"
#include "symbolicc++.h"

const std::string const Expr::FUNCTION[3] = { "sin", "cos", "tan" };

Expr::Expr(std::string expr) {
	root = parse(expr);	

}

// TODO: Parse into tree
std::shared_ptr<ExprNode> Expr::parse(std::string expr) {
	expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end()); // Removes whitespace
	std::stack<std::shared_ptr<ExprNode>> s;
	s.push(std::make_shared<ExprNode>());


	return std::make_shared<ExprNode>("test"); // CHANGE
}

template<class T, class E> bool Expr::in_array(T & arr, E & element) { 
	return std::find(std::begin(arr), std::end(arr), element) != std::end(arr);
}


//
//If the current token is a '(', add a new node as the left child of the current node, and descend to the left child.
//If the current token is in the list['+', '-', '/', '*'], set the root value of the current node to the operator represented by the current token.Add a new node as the right child of the current node and descend to the right child.
//If the current token is a number, set the root value of the current node to the number and return to the parent.
//If the current token is a ')', go to the parent of the current node.

//
//int main() { Expr e("hx"); }