#include "expression.h"

#include <iostream>
#include <vector>
#include <functional>

std::map<std::string, Expression*> Expression::variables;
std::map<std::string, std::pair<Expression*, FunctionArgument*>> Expression::functions;

enum Type{T_NONE, T_NUMBER, T_OPERATOR, T_BRACKET, T_VARFUNC, T_VARIABLE, T_FUNCTION};

Expression::Expression(int t){
	type = t;
}
Expression::~Expression(){
	//dtor
}
numtype Expression::getValue(){
	if(!hasValue()) throw expression_exception("Expression has no value!");
	return value;
}
bool Expression::hasValue(){
	return bValue;
}

Variable::Variable() : Expression(ET_VARIABLE){
	//ctor
}
Variable::Variable(std::string name) : Expression(ET_VARIABLE), name(name){
	
}
Variable::~Variable(){
	//dtor
}
void Variable::evaluate(){
	if(Expression::variables.find(name) != Expression::variables.end()){
		Expression* e = Expression::variables[name];
		e->evaluate();
		bValue = e->hasValue();
		value = e->getValue();
	}
	else throw expression_exception(name + " has no assigned value!");
}
std::string Variable::toString(){
	return name;
}

Constant::Constant() : Expression(ET_CONSTANT){
	
}
Constant::Constant(numtype value) : Expression(ET_CONSTANT){
	bValue = true;
	this->value = value;
}
Constant::~Constant(){
	
}
std::string Constant::toString(){
	return std::to_string(value);
}

template <int opcount>
Operation<opcount>::Operation(int t) : Expression(ET_OPERATION){
	opType = t;
	operands[0] = nullptr;
	operands[1] = nullptr;
}
template <int opcount>
Operation<opcount>::~Operation(){
	delete operands[0];
	delete operands[1];
}
template <int opcount>
std::string Operation<opcount>::toString(){
	std::string s;
	switch(opType){
		case OP_ADD:
			s = "+";
			break;
		case OP_SUB:
			s = "-";
			break;
		case OP_MUL:
			s = "*";
			break;
		case OP_DIV:
			s = "/";
			break;
		case OP_ASSIGN:
			s = "=";
			break;
		default:
			s = "@";
			break;
	}
	if(operands[0] == nullptr || operands[1] == nullptr) return s;
	else return std::string("(") + operands[0]->toString() + std::string(")") + s + std::string("(") + operands[1]->toString() + std::string(")");
}

Addition::Addition() : Operation<2>(OP_ADD){
	//ctor
}
Addition::~Addition(){
	//dtor
}
void Addition::evaluate(){
	operands[0]->evaluate();
	operands[1]->evaluate();
	bValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(bValue){
		value = operands[0]->getValue() + operands[1]->getValue();
	}
}

Subtraction::Subtraction() : Operation<2>(OP_SUB){
	//ctor
}
Subtraction::~Subtraction(){
	//dtor
}
void Subtraction::evaluate(){
	operands[0]->evaluate();
	operands[1]->evaluate();
	bValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(bValue){
		value = operands[0]->getValue() - operands[1]->getValue();
	}
}

Multiplication::Multiplication() : Operation<2>(OP_MUL){
	//ctor
}
Multiplication::~Multiplication(){
	//dtor
}
void Multiplication::evaluate(){
	operands[0]->evaluate();
	operands[1]->evaluate();
	bValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(bValue){
		value = operands[0]->getValue() * operands[1]->getValue();
	}
}

Division::Division() : Operation<2>(OP_DIV){
	//ctor
}
Division::~Division(){
	//dtor
}
void Division::evaluate(){
	operands[0]->evaluate();
	operands[1]->evaluate();
	bValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(bValue){
		value = operands[0]->getValue() / operands[1]->getValue();
	}
}

Assign::Assign() : Operation<2>(OP_ASSIGN){
	//ctor
}
Assign::~Assign(){
	//dtor
}
void Assign::replaceVariable(Expression** e, FunctionArgument* fa, const std::string& argName){
	if((*e)->getType() == ET_VARIABLE){
		Variable* v = ((Variable*)*e);
		if(v->getName() == argName){
			delete v;
			*e = fa;
		}
	}
	
	if((*e)->getType() == ET_OPERATION){
		Operation<2>* o = ((Operation<2>*)*e);
		replaceVariable(&o->operands[0], fa, argName);
		replaceVariable(&o->operands[1], fa, argName);
	}
}
void Assign::evaluate(){
	if(operands[0]->getType() == ET_VARIABLE){
		auto it = Expression::variables.find(((Variable*)operands[0])->name);
		if(it == Expression::variables.end()){
			Expression::variables[((Variable*)operands[0])->name] = operands[1];
		}
		else throw expression_exception(((Variable*)operands[0])->name + " already has value");
		
	}
	else if(operands[0]->getType() == ET_FUNCTION){
		Function* f = ((Function*)operands[0]);
		std::string argName;
		if(f->arg->getType() == ET_VARIABLE){
			argName = ((Variable*)f->arg)->getName();
			delete f->arg;
			f->arg = nullptr;
			FunctionArgument* fa = new FunctionArgument;
			
			replaceVariable(&operands[1], fa, argName);
			
			auto it = Expression::functions.find(f->name);
			if(it == Expression::functions.end()){
				Expression::functions[f->name] = std::pair<Expression*, FunctionArgument*>(operands[1], fa);
			}
			else throw expression_exception(f->name + " already has value");
		}
		/*if(f->arg->getType() == ET_FUNCTION_ARGUMENT){
			
		}*/
		else throw expression_exception("Function argument must be a variable");
	}
	else throw expression_exception(std::string("Cannot assign expression ") + operands[0]->toString());
}

Function::Function(std::string name) : Expression(ET_FUNCTION), name(name){
	
}
Function::~Function(){
	
}
void Function::evaluate(){
	auto f = Expression::functions.find(name);
	
	if(f != Expression::functions.end()){
		f->second.second->e = arg;
		f->second.first->evaluate();
		bValue = f->second.first->hasValue();
		value = f->second.first->getValue();
	}
	else throw expression_exception(name + " is not defined");
}
void Function::setArgument(Expression* arg){
	this->arg = arg;
}
std::string Function::toString(){
	return name + std::string("(") + arg->toString() + std::string(")");
}

bool isNumber(const std::string& str){
	bool hasDecimal = false;
	for(int i = 0; i < (int)str.length(); i++){
		if(str[i] >= '0' && str[i] <= '9') continue;
		else if(i == 0 && str[i] == '-') continue;
		else if(str[i] == '.'){
			if(hasDecimal) return false;
			else hasDecimal = true;
		}
		else return false;
	}
	return true;
}

int getPriority(Expression* o){
	if(o->getType() != ET_OPERATION) return -1;
	switch(((Operation<2>*)o)->getOperationType()){
		case OP_ASSIGN:
			return 0;
		case OP_ADD:
		case OP_SUB:
			return 1;
		case OP_MUL:
		case OP_DIV:
			return 2;
	}
	return -1;
}
int getPriority(const std::string& str){
	if(str == "=") return 0;
	if(str == "+" || str == "-") return 1;
	if(str == "*" || str == "/") return 2;
	if(str == "^") return 3;
	else return -1;
}
int getPriority(const std::pair<int, std::string>& p){
	if(p.first != T_OPERATOR) throw expression_exception(p.second + " is not an operator");
	return getPriority(p.second);
}

Expression* Expression::toExpression(const std::pair<int, std::string>& p){
	if(p.first == T_NUMBER){
		return new Constant(std::stod(p.second));
	}
	else if(p.first == T_BRACKET){
		return Expression::getExpression(p.second.substr(1, p.second.length()-2));
	}
	else if(p.first == T_VARIABLE){
		return new Variable(p.second);
	}
	else if(p.first == T_FUNCTION){
		return new Function(p.second);
	}
	else if(p.first == T_OPERATOR){
		     if(p.second == "+") return new Addition;
		else if(p.second == "-") return new Subtraction;
		else if(p.second == "*") return new Multiplication;
		else if(p.second == "/") return new Division;
		else if(p.second == "=") return new Assign;
		else throw expression_exception(p.second + ": No such operation");
	}
	else throw expression_exception(p.second + ": not convertable to expression");
}

static Expression* evalVec(const std::vector<Expression*>& expressions, int start, int last){
	Expression* ls = expressions[start-1];
	Expression* rs;
	
	int inc = 2;
	for(int i = start; i < last; i += inc){
		inc = 2;
		int priority = getPriority(expressions[i]);
		if(priority == -1) throw std::runtime_error(expressions[i]->toString() + ": Not an operation");
		
		if(i+2 < last && getPriority(expressions[i+2]) > priority){
			int nLast = last;
			// Find the end of right operand
			for(int j = i+2; j < last; j += 2){
				int p = getPriority(expressions[j]);
				if(p <= priority){
					nLast = j;
					break;
				}
			}
			
			rs = evalVec(expressions, i+2, nLast);
			inc = nLast-i;
		}
		else if(i+1 < last){
			rs = expressions[i+1];
		}
		else throw std::runtime_error("Missing right operand");
		
		Operation<2>* op = (Operation<2>*)expressions[i];
		
		//std::cout << ls->m_value << expressions[i] << rs->m_value << std::endl;
		op->operands[0] = ls;
		op->operands[1] = rs;
		ls = op;
	}
	
	return ls;
}

static std::vector<Expression*> convertVec(const std::vector<std::pair<int, std::string>>& parts){
	std::vector<Expression*> expressions;
	
	for(int i = 0; i < (int)parts.size(); i++){
		Expression* e = Expression::toExpression(parts[i]);
		
		if(i > 0 && parts[i].first == T_BRACKET && parts.at(i-1).first == T_FUNCTION){
			((Function*)expressions.at(i-1))->setArgument(e);
			continue;
		}
		
		expressions.push_back(e);
	}
	
	return expressions;
}

static std::vector<std::pair<int, std::string>> toVec(const std::string& str){
	int type = 0, oldType = 0;
	
	int beg = 0;
	bool bBracket = false;
	int cBracket = 0;
	std::vector<std::pair<int, std::string>> parts;
	for(int i = 0; i < (int)str.length(); i++){
		char c = str[i];
		
		oldType = type;
		
		if(!bBracket){
			if(((c >= '0' && c <= '9') || c == '.') && oldType != T_VARFUNC){
				type = T_NUMBER;
			}
			else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '='){
				type = T_OPERATOR;
			}
			else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){
				type = T_VARFUNC;
			}
			else if(c == ' '){
				type = T_NONE;
			}
		}
		if(c == '('){
			cBracket++;
			bBracket = true;
			type = T_BRACKET;
		}
		else if(c == ')'){
			cBracket--;
			if(cBracket == 0) bBracket = false;
		}
		
		if(type != oldType){
			if(oldType != T_NONE){
				addPart:
				parts.push_back({oldType, str.substr(beg, i-beg)});
			}
			beg = i;
		}
		if(i == (int)str.length()-1){
			i++;
			oldType = type;
			goto addPart;
		}
	}
	
	if(cBracket > 0) throw expression_exception("Unclosed bracket!");
	else if(cBracket < 0) throw expression_exception("Unopened bracket!");
	
	for(int i = 0; i < (int)parts.size(); i++){
		if(parts[i].first == T_VARFUNC){
			if(i < (int)parts.size()-1 && parts[i+1].first == T_BRACKET){
				parts[i].first = T_FUNCTION;
			}
			else{
				parts[i].first = T_VARIABLE;
			}
		}
	}
	
	return parts;
}

Expression* Expression::getExpression(const std::string& str){
	if(str.empty()) throw expression_exception("Input was empty.");
	
	std::vector<std::pair<int, std::string>> parts = toVec(str);
	//for(auto a : parts) std::cout << a.first << " : " << a.second << std::endl;
	std::vector<Expression*> expressions = convertVec(parts);
	
	//std::cout << str << ": ";
	//for(int i = 0; i < (int)expressions.size(); i++) std::cout << expressions[i]->getType() << std::endl;
	//std::cout << std::endl;
	
	Expression* e = evalVec(expressions, 1, (int)expressions.size());
	
	return e;
}