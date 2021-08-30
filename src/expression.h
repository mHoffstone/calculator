#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <array>

class Expression{
	public:
		Expression();
		virtual ~Expression();
		
		virtual double getValue(){return m_value;}
		virtual bool hasValue(){return m_hasValue;}
		
		virtual void evaluate() = 0;
		
	public:
		bool m_hasValue;
		double m_value;
};

class Variable : public Expression{
	public:
		Variable();
		virtual ~Variable();
		
		virtual void evaluate() override{}
		
		std::string name;
};

template <int opcount>
class Operation : public Expression{
	public:
		Operation();
		virtual ~Operation();
		
		std::array<Expression*, opcount> operands;
};

class Addition : public Operation<2>{
	public:
		Addition();
		virtual ~Addition();
		
		void evaluate() override;
};

class Subtraction : public Operation<2>{
	public:
		Subtraction();
		virtual ~Subtraction();
		
		void evaluate() override;
};

class Multiplication : public Operation<2>{
	public:
		Multiplication();
		virtual ~Multiplication();
		
		void evaluate() override;
};

class Division : public Operation<2>{
	public:
		Division();
		virtual ~Division();
		
		void evaluate() override;
};

Expression* getExpression(const std::string& str);

#endif // EXPRESSION_H
