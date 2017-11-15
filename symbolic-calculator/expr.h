#pragma once
#include <string>
#include <memory>

class Symbolic;

struct ExprNode {
	std::string data;
	std::shared_ptr<ExprNode> left;
	std::shared_ptr<ExprNode> right;

	ExprNode() : data(), left(), right() {}
	ExprNode(std::string data) : data(data), left(), right() {}
};

class Expr {
public:
	Expr(std::string expr);
	
private:
	std::shared_ptr<ExprNode> root;
	std::shared_ptr<Symbolic> expr;
	std::shared_ptr<ExprNode> parse(std::string expr);

	template<class T, class E> bool in_array(T & arr, E & element);
public:
	static const std::string const FUNCTION[3];
};
