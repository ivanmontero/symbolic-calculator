#pragma once
#include <string>
#include <memory>

class Symbolic;

struct ExprNode {
	std::string data;
	std::shared_ptr<ExprNode> left;
	std::shared_ptr<ExprNode> right;

	ExprNode() : data() {}
	ExprNode(std::string data) : data(data) {}
};

class Expr {
public:
	Expr(std::string expr);

	void print();
	
private:
	std::shared_ptr<Symbolic> expr;
	std::shared_ptr<ExprNode> root;
	std::shared_ptr<ExprNode> parse(std::string expr);

	void print(std::shared_ptr<ExprNode> root);

	template<class T, class E> bool in_array(T & arr, E & element);
	template<class T, class E> int index_of(T & arr, E & element);
public:
	static const std::string const FUNCTIONS[4];
	static const std::string const SPECIAL_NUMBERS[2];
	static const char const PARENTHESES[2];
	static const char const OPERATORS[5];
	static const char const NUMBERS[11];
};
