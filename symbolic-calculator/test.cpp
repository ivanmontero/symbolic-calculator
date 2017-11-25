#include <iostream>
#include "expr.h"

int main() {
	Expr f("2*-x*(e^x)^2");
	Expr g("x^3");
	Expr fog = f.subst("x", g);

	std::cout << fog.to_string() << std::endl;

	std::cout << fog.at("x", 1).to_string();

	std::cin.get();
}