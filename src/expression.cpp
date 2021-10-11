#include "expression.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

std::map<std::string, Expression*> Expression::variables;
std::map<std::string, std::pair<Expression*, FunctionArgument*>> Expression::functions;
std::vector<Expression*> Expression::allExpressions;

enum Type{T_NONE, T_NUMBER, T_OPERATOR, T_BRACKET, T_VARFUNC, T_VARIABLE, T_FUNCTION};

struct primfunc{
	std::string name;
	numtype (*f)(numtype);
};
static const primfunc primfuncs[]{
	{"sqrt", std::sqrt},
	{"sin", std::sin},
	{"cos", std::cos},
	{"tan", std::tan},
	{"abs", std::abs},
	{"floor", std::floor},
	{"ceil", std::ceil},
	{"ln", std::log},
	{"log", std::log10},
};

struct prevar{
	std::string name;
	numtype v;
};
static const prevar prevars[]{
	{"e", 2.71828},
	{"pi", 3.14159},
};

Expression::Expression(int t, bool safe = false){
	type = t;
	if(!safe) allExpressions.push_back(this);
}
Expression::~Expression(){
	//dtor
	//std::cout << this << " was deleted!" << std::endl;
}
numtype Expression::getValue(){
	if(!hasValue()) throw expression_exception("Expression has no value");
	return value;
}
bool Expression::hasValue(){
	return bValue;
}
void Expression::init(){
	
}
void Expression::clean(){
	for(int i = 0; i < (int)allExpressions.size(); i++){
		if(allExpressions[i] == nullptr) continue;
		int n = std::count(allExpressions.begin(), allExpressions.end(), allExpressions[i]);
		if(n > 1){
			allExpressions[i] = nullptr;
		}
		else if(n == 1){
			delete allExpressions[i];
		}
	}
	allExpressions.clear();
	
	variables.clear();
	functions.clear();
	
	init();
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
	int index = getBuiltInVariableIndex(name);
	if(index > -1){
		Constant* c = new Constant(getBuiltInVariable(index));
		setValue(c->getValue());
		return;
	}
	
	if(Expression::variables.find(name) != Expression::variables.end()){
		Expression* e = Expression::variables[name];
		e->evaluate();
		setValue(e->getValue());
	}
	else throw expression_exception(name + " has no assigned value");
}
std::string Variable::toString(){
	return name;
}
int Variable::getBuiltInVariableIndex(const std::string& name){
	int n = sizeof(prevars) / sizeof(prevars);
	for(int i = 0; i < n; i++){
		if(prevars[i].name == name) return i;
	}
	return -1;
}
numtype Variable::getBuiltInVariable(int index){
	return prevars[index].v;
}

Constant::Constant() : Expression(ET_CONSTANT){
	
}
Constant::Constant(numtype value, bool safe) : Expression(ET_CONSTANT, safe){
	bValue = true;
	this->value = value;
}
Constant::~Constant(){
	
}
std::string Constant::toString(){
	return std::to_string(value);
}

FunctionArgument::FunctionArgument() : Expression(ET_FUNCTION_ARGUMENT){
	
}
FunctionArgument::~FunctionArgument(){
	
}

Function::Function(std::string name) : Expression(ET_FUNCTION), name(name){
	
}
Function::~Function(){
	
}
void Function::evaluate(){
	int index = getPrimfuncIndex(name);
	if(index > -1){
		arg->evaluate();
		numtype v = arg->getValue();
		setValue(getPrimfuncVal(index, v));
		return;
	}
	
	auto f = Expression::functions.find(name);
	if(f != Expression::functions.end()){
		f->second.second->e = arg;
		f->second.first->evaluate();
		setValue(f->second.first->getValue());
		return;
	}

	throw expression_exception(name + " is not defined");
}
void Function::setArgument(Expression* arg){
	this->arg = arg;
}
std::string Function::toString(){
	return name + std::string("(") + arg->toString() + std::string(")");
}
int Function::getPrimfuncIndex(const std::string& name){
	int n = sizeof(primfuncs) / sizeof(primfunc);
	for(int i = 0; i < n; i++){
		if(primfuncs[i].name == name) return i;
	}
	return -1;
}
numtype Function::getPrimfuncVal(int index, numtype v){
	return primfuncs[index].f(v);
}

template <int opcount>
Operation<opcount>::Operation(int t) : Expression(ET_OPERATION){
	opType = t;
	operands[0] = nullptr;
	operands[1] = nullptr;
}
template <int opcount>
Operation<opcount>::~Operation(){
	/*for(int i = 0; i < opcount; i++){
		if(operands[i] != nullptr){
			delete operands[i];
			operands[i] = nullptr;
		}
	}*/
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


Modulo::Modulo() : Operation<2>(OP_MOD){
	//ctor
}
Modulo::~Modulo(){
	//dtor
}
void Modulo::evaluate(){
	operands[0]->evaluate();
	operands[1]->evaluate();
	bValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(bValue){
		value = std::fmod(std::abs(operands[0]->getValue()), operands[1]->getValue());
	}
}


Power::Power() : Operation<2>(OP_POW){
	//ctor
}
Power::~Power(){
	//dtor
}
void Power::evaluate(){
	operands[0]->evaluate();
	operands[1]->evaluate();
	bValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(bValue){
		value = std::pow(operands[0]->getValue(), operands[1]->getValue());
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
			*e = fa;
		}
	}
	
	else if((*e)->getType() == ET_OPERATION){
		Operation<2>* o = ((Operation<2>*)*e);
		replaceVariable(&o->operands[0], fa, argName);
		replaceVariable(&o->operands[1], fa, argName);
	}
	
	else if((*e)->getType() == ET_FUNCTION){
		Function* f = ((Function*)*e);
		replaceVariable(&f->arg, fa, argName);
	}
}
void Assign::evaluate(){
	if(operands[0]->getType() == ET_VARIABLE){
		Variable* v = ((Variable*)operands[0]);
		if(Variable::getBuiltInVariableIndex(v->name) > -1) throw expression_exception(v->name + " is a built-in variable and cannot be reassigned");
		
		//Expression::variables[((Variable*)operands[0])->name] = operands[1];
		auto it = Expression::variables.find(v->name);
		if(it == Expression::variables.end()){
			Expression::variables[v->name] = operands[1];
		}
		else throw expression_exception(v->name + " already has value");
		
	}
	else if(operands[0]->getType() == ET_FUNCTION){
		Function* f = ((Function*)operands[0]);
		std::string argName;
		if(f->arg->getType() == ET_VARIABLE){
			argName = ((Variable*)f->arg)->getName();
			
			if(Function::getPrimfuncIndex(f->name) > -1) throw expression_exception(f->name + " is a built-in function and cannot be reassigned");
			
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
		case OP_MOD:
			return 2;
		case OP_POW:
			return 3;
	}
	return -1;
}
/*
int getPriority(const std::string& str){
	if(str == "=") return 0;
	if(str == "+" || str == "-") return 1;
	if(str == "*" || str == "/" || str == "%") return 2;
	if(str == "^") return 3;
	else return -1;
}
int getPriority(const std::pair<int, std::string>& p){
	if(p.first != T_OPERATOR) throw expression_exception(p.second + " is not an operator");
	return getPriority(p.second);
}
*/

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
		else if(p.second == "%") return new Modulo;
		else if(p.second == "^") return new Power;
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
		#ifdef DBG
		std::cout << " - " << parts[i].second << std::endl;
		#endif
		Expression* e = Expression::toExpression(parts[i]);
		
		if(i > 0 && parts[i].first == T_BRACKET && parts.at(i-1).first == T_FUNCTION){
			((Function*)expressions.back())->setArgument(e);
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
			else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '%' || c == '^'){
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
	
	if(cBracket > 0) throw expression_exception("Unclosed bracket");
	else if(cBracket < 0) throw expression_exception("Unopened bracket");
	
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
	if(str.empty()) throw expression_exception("Input was empty");
	
	std::vector<std::pair<int, std::string>> parts = toVec(str);
	#ifdef DBG
	for(auto a : parts) std::cout << a.first << " : " << a.second << std::endl;
	std::cout << std::endl;
	#endif
	std::vector<Expression*> expressions = convertVec(parts);
	
	#ifdef DBG
	std::cout << str << ": " << std::endl;
	for(int i = 0; i < (int)expressions.size(); i++) std::cout << expressions[i]->getType() << std::endl;
	std::cout << std::endl;
	#endif
	
	Expression* e = evalVec(expressions, 1, (int)expressions.size());
	
	return e;
}