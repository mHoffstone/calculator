#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <array>

enum ExpressionType{
	ET_NONE, ET_VARIABLE, ET_OPERATION
};

enum OperationType{
	OP_NONE, OP_ADD, OP_SUB, OP_MUL, OP_DIV
};

class Expression{
	public:
		Expression(int t);
		virtual ~Expression();
		
		virtual double getValue();
		virtual bool hasValue();
		
		virtual void evaluate(){};
		
	public:
		bool m_hasValue;
		double m_value;
		int type;
};

class Variable : public Expression{
	public:
		Variable();
		virtual ~Variable();
		
		std::string name;
};

template <int opcount>
class Operation : public Expression{
	public:
		Operation(int t);
		virtual ~Operation();
		
		std::array<Expression*, opcount> operands;
		int opType;
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
