#pragma once
#include <string>
#include <memory>
#include <queue>

class Symbolic;

class Expr {
	// Object methods
private:
	std::shared_ptr<Symbolic> expr;

	std::shared_ptr<Symbolic> parse(std::string expr);
	Expr(Symbolic expr);

public:
	Expr();
	Expr(Expr& o);
	Expr(std::string expr);
	void set(std::string expr);

	Expr at(std::string var, double val);
	double eval_at(std::string var, double val);
	Expr subst(std::string var, Expr f);

	Expr df(std::string var);
	Expr integrate(std::string var);

	Expr operator+(const Expr& o) const;
	Expr operator-(const Expr& o) const;
	Expr operator*(const Expr& o) const;
	Expr operator/(const Expr& o) const;

	std::string to_string();

	// Static methods
private:
	static Symbolic to_symbolic(std::queue<std::string> postfix);
	static double to_double(std::queue<std::string> postfix);
	static double eval(std::string expr);
	static std::string to_string(Symbolic expr);

public:
	static double eval(Symbolic expr);
	static std::queue<std::string> to_infix(std::string str);
	static std::queue<std::string> to_postfix(std::queue<std::string> infix);
	template<class T, class E> static bool in_array(T & arr, E & element);
	template<class T, class E> static int index_of(T & arr, E & element);

	static const std::string const FUNCTIONS[4];
	static const std::string const SPECIAL_NUMBERS[2];
	static const char const PARENTHESES[2];
	static const char const OPERATORS[6];
	static const char const NUMBERS[11];
};
