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
		Expression(ExpressionType t, bool safe);
		void setValue(numtype v){value = v; bValue = true;}
	public:
		virtual ~Expression();
		
		virtual numtype getValue();
		
		bool hasValue(){
			return bValue;
		}
		
		virtual void evaluate(bool simplify){}
		
		static std::string toString(numtype v);
		virtual std::string toString(){
			if(hasValue()) return toString(value);
			else return "#";
		}
		
		static bool hasVariable(const std::string& name){
			return variables.find(name) != variables.end();
		}
		static Expression* getVariable(const std::string& name){
			return variables[name];
		}
		
		ExpressionType getType(){
			return type;
		}
		
		virtual Expression* clone(){return nullptr;}
		void cloneFrom(Expression* e);
		
		static void init();
		static void clean();
		
		static Expression* getExpression(const std::string& str);
		static Expression* toExpression(const std::pair<int, std::string>& p);
		
		static std::pair<Expression*, FunctionArgument*> getDrawFunction(int index){
			return drawFunctions.at(index);
		}
		
	protected:
		bool bValue = false;
		numtype value = -1.0;
		ExpressionType type;
		static std::map<std::string, Expression*> variables;
		static std::map<std::string, std::pair<Expression*, FunctionArgument*>> functions;
		static std::vector<std::pair<Expression*, FunctionArgument*>> drawFunctions;
		
	private:
		static std::vector<Expression*> allExpressions;
};

class Variable : public Expression{
	public:
		Variable();
		Variable(std::string name);
		virtual ~Variable();
		
		void evaluate(bool simplify) override;
		virtual std::string toString() override;
		
		std::string getName(){
			return name;
		}
		
		virtual Expression* clone() override;
		
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
		
		virtual Expression* clone() override;
};

class FunctionArgument : public Expression{
	public:
		FunctionArgument();
		virtual ~FunctionArgument();
		
		Expression* e = nullptr;
		
		void evaluate(bool simplify) override;
		
		std::string toString() override{
			return "x";
		}
		
		virtual Expression* clone() override;
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
		
		void evaluate(bool simplify) override;
		virtual std::string toString() override;
		
		virtual Expression* clone() override;
		
		static int getPrefuncIndex(const std::string& name);
		static numtype getPrefuncVal(int index, numtype v);
		
		friend class Assign;
		
	protected:
		std::string name;
		Expression* arg = nullptr;
		bool bDefined = false;
};

class Operation : public Expression{
	public:
		Operation(OperationType t);
		virtual ~Operation();
		
		virtual std::string toString() override;
		
		std::array<Expression*, 2> operands;
		
		OperationType getOperationType(){
			return opType;
		}
		
		virtual Expression* clone() override;
		
	private:
		OperationType opType;
};

class Addition : public Operation{
	public:
		Addition();
		virtual ~Addition();
		
		void evaluate(bool simplify) override;
};

class Subtraction : public Operation{
	public:
		Subtraction();
		virtual ~Subtraction();
		
		void evaluate(bool simplify) override;
};

class Multiplication : public Operation{
	public:
		Multiplication();
		virtual ~Multiplication();
		
		void evaluate(bool simplify) override;
};

class Division : public Operation{
	public:
		Division();
		virtual ~Division();
		
		void evaluate(bool simplify) override;
};

class Modulo : public Operation{
	public:
		Modulo();
		virtual ~Modulo();
		
		void evaluate(bool simplify) override;
};

class Power : public Operation{
	public:
		Power();
		virtual ~Power();
		
		void evaluate(bool simplify) override;
};

class Assign : public Operation{
	public:
		Assign();
		virtual ~Assign();
		
		void evaluate(bool simplify) override;
		
	private:
		void replaceVariable(Expression** e, FunctionArgument* fa, const std::string& argName);
};

class expression_exception : public std::exception{
	public:
		expression_exception(const std::string& what_arg, int type = 0){
			s = what_arg;
			this->type = type;
		}
		virtual ~expression_exception(){}
		
		static const int T_NO_VALUE = 1;
		
		virtual const char* what() const noexcept override{
			return s.c_str();
		}
		
		const int get_type() const noexcept{
			return type;
		}
		
	private:
		std::string s;
		int type;
};

#endif // EXPRESSION_H
