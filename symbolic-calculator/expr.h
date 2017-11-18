#pragma once
#include <string>
#include <memory>

class Symbolic;

class Expr {
	// Object methods
public:
	Expr(std::string expr);
private:
	std::shared_ptr<Symbolic> expr;

	// Static methods
public:
	static std::shared_ptr<Symbolic> parse(std::string expr);
	template<class T, class E> static bool in_array(T & arr, E & element);
	template<class T, class E> static int index_of(T & arr, E & element);

	static const std::string const FUNCTIONS[4];
	static const std::string const SPECIAL_NUMBERS[2];
	static const char const PARENTHESES[2];
	static const char const OPERATORS[5];
	static const char const NUMBERS[11];
};
