#pragma once
#include <string>
#include <memory>
#include <queue>

class Symbolic;

class Expr {
	// Object methods
public:
	Expr(std::string expr);
private:
	std::shared_ptr<Symbolic> expr;

	// Static methods
public:
	static Symbolic parse(std::string expr);
	static double eval(std::string expr);

	static std::queue<std::string> to_infix(std::string str);
	static std::queue<std::string> to_postfix(std::queue<std::string> infix);
	static Symbolic to_symbolic(std::queue<std::string> postfix);
	static double to_double(std::queue<std::string> postfix);
	template<class T, class E> static bool in_array(T & arr, E & element);
	template<class T, class E> static int index_of(T & arr, E & element);



	static const std::string const FUNCTIONS[4];
	static const std::string const SPECIAL_NUMBERS[2];
	static const char const PARENTHESES[2];
	static const char const OPERATORS[5];
	static const char const NUMBERS[11];
};
