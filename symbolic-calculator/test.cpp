#include <iostream>
#include "expr.h"

# define M_PI	3.14159265358979323846

int main() {
	Expr f("cos(t)");
	std::cout<< f.eval_at("t", M_PI / 2);

	std::cin.get();
}