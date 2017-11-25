#pragma once
#include <string>
#include <memory>
#include <queue>

#include "symbolicc++.h"

namespace Expr {
	Symbolic parse(std::string expr);
	double eval(std::string expr);
	double eval(Symbolic s);

	std::queue<std::string> to_infix(std::string str);
	std::queue<std::string> to_postfix(std::queue<std::string> infix);
	Symbolic to_symbolic(std::queue<std::string> postfix);
	double to_double(std::queue<std::string> postfix);
	template<class T, class E> bool in_array(T & arr, E & element);
	template<class T, class E> int index_of(T & arr, E & element);

	extern const std::string const FUNCTIONS[4];
	extern const std::string const SPECIAL_NUMBERS[2];
	extern const char const PARENTHESES[2];
	extern const char const OPERATORS[6];
	extern const char const NUMBERS[11];
};
