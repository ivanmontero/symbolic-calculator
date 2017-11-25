#include <iostream>
#include "expr.h"

int main() {
	Expr f("2*-x*(e^x)^2");
	Expr g("x^3");
	Expr fg = (f * g).df("x");

	std::cout << fg.to_string() << std::endl;

	std::cin.get();
}