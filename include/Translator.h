#pragma once

#include <iostream>
#include <map>
#include <string>
#include "Stack.h"

using namespace std;

char invertBrackets(char ch)
{
	if (ch == '(') return ')';
	else if (ch == '[') return ']';
	else if (ch == '{') return '}';
	return ' ';
}

bool checkBrackets(string& s)
{
	Stack<char> stack;
	size_t len = s.length();
	for (size_t i = 0; i < len; i++)
	{
		if (s[i] == '(' || s[i] == '{' || s[i] == '[')
			stack.push(s[i]);
		else if (s[i] == ')' || s[i] == '}' || s[i] == ']')
			if (!stack.isEmpty() && s[i] == invertBrackets(stack.top()))
				stack.pop();
			else return 0;
	}
	if (stack.isEmpty())
		return 1;
	else return 0;
}

enum types { number, operation, open_bracket, close_bracket };

class Term
{
protected:
	map<char, size_t> pritorities{ {'*', 2}, {'/', 2}, {'+', 1}, {'-', 1} };
	types type;
public:
	types get_type() const noexcept { return type; }
};

class Number : public Term
{
	double value;
public:
	double get_value() const noexcept { return value; }
	Number(double _value) : value(_value) { type = number; }
};

class Operation : public Term
{
	char op;
	size_t priority;
public:
	char get_op() const noexcept { return op; }
	size_t get_priority() const noexcept { return priority; }
	Operation(char _op) : op(_op)
	{
		priority = pritorities[op];
		type = operation;
	}
};

class OpenBracket : public Term
{
	char bracket;
public:
	char get_op() const noexcept { return bracket; }
	OpenBracket() : bracket('(') { type = open_bracket; }
};

class CloseBracket : public Term
{
	char bracket;
public:
	char get_op() const noexcept { return bracket; }
	CloseBracket() : bracket(')') { type = close_bracket; }
};

class Translator
{
	Vector<Term*> terms;
	Vector<Term*> polish_notation;
	string expression;
public:
	Translator(string _expression = "2+2*2") : expression(_expression) { }
	~Translator()
	{
		size_t sz = terms.size();
		for (size_t i = 0; i < sz; ++i)
			delete terms[i];
	}
	void tokenizer()
	{
		string N;
		size_t number_status = 0;
		for (size_t i = 0; i < expression.size(); ++i)
		{
			if (!number_status)
			{
				if (expression[i] == '(')
					terms.push_back(new OpenBracket);
				else if (expression[i] == '*' || expression[i] == '/' || expression[i] == '+' || expression[i] == '-')
					terms.push_back(new Operation(expression[i]));
				else if (expression[i] == ')')
					terms.push_back(new CloseBracket);
				else if (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '.')
				{
					number_status = 1;
					N += expression[i];
				}
				else throw logic_error("Invalid syntax!");
			}
			else
			{
				if (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '.')
					N += expression[i];
				else if (expression[i] == '(')
				{
					number_status = 0;
					terms.push_back(new Number(stod(N)));
					terms.push_back(new OpenBracket);
					N.clear();
				}
				else if (expression[i] == '*' || expression[i] == '/' || expression[i] == '+' || expression[i] == '-')
				{
					number_status = 0;
					terms.push_back(new Number(stod(N)));
					terms.push_back(new Operation(expression[i]));
					N.clear();
				}
				else if (expression[i] == ')')
				{
					number_status = 0;
					terms.push_back(new Number(stod(N)));
					terms.push_back(new CloseBracket);
					N.clear();
				}
				else throw logic_error("Invalid syntax!");
			}
		}
		if (!N.empty())
			terms.push_back(new Number(stod(N)));
	}
	size_t get_terms_size() const noexcept { return terms.size(); }
	size_t get_expression_size() const noexcept { return expression.size(); }
	string get_expression() const noexcept { return expression; }
	void print_expression()
	{
		size_t sz = expression.size();
		for (size_t i = 0; i < sz; ++i)
		{
			cout << expression[i];
		}
		cout << endl;
	}
	void parser()
	{
		map<types, size_t> states{ {number, 0}, {operation, 1}, {open_bracket, 2}, {close_bracket, 3} };
		size_t state;
		Stack<char> stack;
		size_t sz = terms.size();
		state = states[terms[0]->get_type()];
		if (state == 1)
			throw logic_error("Operation can't be the first symbol!");
		if (state == 2)
			stack.push('(');
		if (state == 3)
			throw logic_error("')' can't be the first symbol!");
		for (size_t i = 1; i < sz; ++i)
		{
			if (terms[i]->get_type() == open_bracket)
				stack.push('(');
			else if (terms[i]->get_type() == close_bracket)
				if (!stack.isEmpty())
					stack.pop();
				else throw logic_error("Incorrect brackets!");
			switch (state)
			{
			case 0:
				if (terms[i]->get_type() == open_bracket || terms[i]->get_type() == number)
					throw logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			case 1:
				if (terms[i]->get_type() == operation || terms[i]->get_type() == close_bracket)
					throw logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			case 2:
				if (terms[i]->get_type() == close_bracket || terms[i]->get_type() == operation)
					throw logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			case 3:
				if (terms[i]->get_type() == number || terms[i]->get_type() == open_bracket)
					throw logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			}
		}
		state = states[terms[sz - 1]->get_type()];
		if (state == 1 || state == 2)
			throw logic_error("Invalid syntax!");
		if (!stack.isEmpty())
			throw logic_error("Incorrect brackets!");
	}
	void converter()
	{
		Stack<Term*> st;
		size_t sz = terms.size();
		for (size_t i = 0; i < sz; ++i)
		{
			if (terms[i]->get_type() == number)
				polish_notation.push_back(terms[i]);
			else if (terms[i]->get_type() == operation)
			{
				if (st.isEmpty())
					st.push(terms[i]);
				else
				{
					while (!st.isEmpty() && st.top()->get_type() == operation && \
						static_cast<Operation*>(terms[i])->get_priority() <= static_cast<Operation*>(st.top())->get_priority())
					{
						polish_notation.push_back(st.top());
						st.pop();
					}
					st.push(terms[i]);
				}
			}
			else if (terms[i]->get_type() == open_bracket)
				st.push(terms[i]);
			else if (terms[i]->get_type() == close_bracket)
			{
				while (st.top()->get_type() != open_bracket)
				{
					polish_notation.push_back(st.top());
					st.pop();
				}
				st.pop();
			}
		}
		while (!st.isEmpty())
		{
			polish_notation.push_back(st.top());
			st.pop();
		}
	}
	string get_polish_notation()
	{
		string polish_notation_str;
		size_t sz = polish_notation.size();
		for (size_t i = 0; i < sz; ++i)
		{
			if (polish_notation[i]->get_type() == number)
			{
				string num = to_string(static_cast<Number*>(polish_notation[i])->get_value());
				num.erase(num.find_last_not_of('0') + 1, string::npos);
				num.erase(num.find_last_not_of('.') + 1, string::npos);
				polish_notation_str += num;
			}
			else  polish_notation_str += static_cast<Operation*>(polish_notation[i])->get_op();
		}
		return polish_notation_str;
	}
	void print_polish_notation()
	{
		size_t sz = polish_notation.size();
		for (size_t i = 0; i < sz; ++i)
		{
			if (polish_notation[i]->get_type() == number)
				cout << static_cast<Number*>(polish_notation[i])->get_value();
			else cout << static_cast<Operation*>(polish_notation[i])->get_op();
		}
		cout << endl;
	}
	double calculator()
	{
		types current_type;
		Stack<double> st;
		double right_numb, left_numb;
		size_t sz = polish_notation.size();
		for (size_t i = 0; i < sz; ++i)
		{
			current_type = polish_notation[i]->get_type();
			if (current_type == number)
				st.push(static_cast<Number*>(polish_notation[i])->get_value());
			else
			{
				right_numb = st.top();
				st.pop();
				left_numb = st.top();
				st.pop();
				switch (static_cast<Operation*>(polish_notation[i])->get_op())
				{
				case '*':
					st.push(left_numb * right_numb);
					break;
				case '/':
					if (right_numb == 0)
						throw runtime_error("division by zero!");
					st.push(left_numb / right_numb);
					break;
				case '+':
					st.push(left_numb + right_numb);
					break;
				case '-':
					st.push(left_numb - right_numb);
					break;
				}
			}
		}
		return st.top();
	}
	double calculate()
	{
		this->tokenizer();
		this->parser();
		this->converter();
		cout << this->get_polish_notation() << endl;
		return this->calculator();
	}
};