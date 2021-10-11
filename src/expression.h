#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <array>
#include <exception>
#include <map>
#include <utility>
#include <vector>

typedef double numtype;

enum ExpressionType{
	ET_NONE, ET_CONSTANT, ET_VARIABLE, ET_OPERATION, ET_FUNCTION, ET_FUNCTION_ARGUMENT
};

enum OperationType{
	OP_NONE, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW, OP_MOD, OP_ASSIGN
};

class FunctionArgument;

class Expression{
	protected:
		Expression(int t, bool safe);
		void setValue(numtype v){value = v; bValue = true;}
	public:
		virtual ~Expression();
		
		virtual numtype getValue();
		virtual bool hasValue();
		
		virtual void evaluate(){};
		virtual std::string toString(){return "";};
		
		static Expression* getVariable(const std::string& name){
			return variables[name];
		}
		
		int getType(){
			return type;
		}
		
		static void init();
		static void clean();
		
		static Expression* getExpression(const std::string& str);
		static Expression* toExpression(const std::pair<int, std::string>& p);
		
	protected:
		bool bValue = false;
		numtype value = -1.0;
		int type;
		static std::map<std::string, Expression*> variables;
		static std::map<std::string, std::pair<Expression*, FunctionArgument*>> functions;
		
	private:
		static std::vector<Expression*> allExpressions;
};

class Variable : public Expression{
	public:
		Variable();
		Variable(std::string name);
		virtual ~Variable();
		
		void evaluate() override;
		virtual std::string toString() override;
		
		std::string getName(){
			return name;
		}
		
		static int getBuiltInVariableIndex(const std::string& name);
		static numtype getBuiltInVariable(int index);
		
		friend class Assign;
		
	private:
		std::string name;
};

class Constant : public Expression{
	public:
		Constant();
		Constant(numtype value, bool safe = false);
		virtual ~Constant();
		
		virtual std::string toString() override;
		
		void setValue(numtype value){
			bValue = true;
			this->value = value;
		}
		void resetValue(){
			bValue = false;
		}
};

class FunctionArgument : public Expression{
	public:
		FunctionArgument();
		virtual ~FunctionArgument();
		
		Expression* e = nullptr;
		
		void evaluate() override{
			e->evaluate();
			bValue = e->hasValue();
			value = e->getValue();
		}
		std::string toString() override{
			return "FUNCTION ARGUMENT";
		}
};
class Function : public Expression{
	public:
		Function(std::string name);
		virtual ~Function();
		
		void setArgument(Expression* var);
		Expression* getArgument(){
			return arg;
		}
		std::string getName(){
			return name;
		}
		
		void evaluate() override;
		virtual std::string toString() override;
		
		static int getPrimfuncIndex(const std::string& name);
		static numtype getPrimfuncVal(int index, numtype v);
		
		friend class Assign;
		
	protected:
		std::string name;
		Expression* arg = nullptr;
};

template <int opcount>
class Operation : public Expression{
	public:
		Operation(int t);
		virtual ~Operation();
		
		virtual std::string toString() override;
		
		std::array<Expression*, opcount> operands;
		
		int getOperationType(){
			return opType;
		}
	private:
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

class Modulo : public Operation<2>{
	public:
		Modulo();
		virtual ~Modulo();
		
		void evaluate() override;
};

class Power : public Operation<2>{
	public:
		Power();
		virtual ~Power();
		
		void evaluate() override;
};

class Assign : public Operation<2>{
	public:
		Assign();
		virtual ~Assign();
		
		void evaluate() override;
		
	private:
		void replaceVariable(Expression** e, FunctionArgument* fa, const std::string& argName);
};

class expression_exception : public std::exception{
	public:
		expression_exception(const std::string& what_arg){
			s = what_arg;
		}
		virtual ~expression_exception(){}
		
		virtual const char* what() const noexcept override{
			return s.c_str();
		}
		
	private:
		std::string s;
};

#endif // EXPRESSION_H
