#include "expression.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

std::map<std::string, Expression*> Expression::variables;
std::map<std::string, std::pair<Expression*, FunctionArgument*>> Expression::functions;
std::vector<std::pair<Expression*, FunctionArgument*>> Expression::drawFunctions;
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
	{"e", 2.71828182846},
	{"pi", 3.14159265359},
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
	if(!hasValue()) throw expression_exception("Expression has no value", expression_exception::T_NO_VALUE);
	return value;
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
	drawFunctions.clear();
	
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
void Variable::evaluate(bool simplify){
	int index = getBuiltInVariableIndex(name);
	if(index > -1){
		setValue(getBuiltInVariable(index));
		return;
	}
	
	if(Expression::variables.find(name) != Expression::variables.end()){
		Expression* e = Expression::variables[name];
		e->evaluate(simplify);
		if(e->hasValue()) setValue(e->getValue());
		return;
	}
	
	if(!simplify) throw expression_exception(name + " has no assigned value", expression_exception::T_NO_VALUE);
}

std::string Variable::toString(){
	if(hasValue()) return Expression::toString();
	else if(Expression::variables.find(name) == Expression::variables.end()){
		return name;
	}
	else return Expression::variables[name]->toString();
}
int Variable::getBuiltInVariableIndex(const std::string& name){
	int n = sizeof(prevars) / sizeof(prevar);
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
	return Expression::toString();
}

FunctionArgument::FunctionArgument() : Expression(ET_FUNCTION_ARGUMENT){
	
}
FunctionArgument::~FunctionArgument(){
	
}
void FunctionArgument::evaluate(bool simplify){
	if(e == nullptr) return;
	e->evaluate(simplify);
	bValue = e->hasValue();
	value = e->getValue();
}

Function::Function(std::string name) : Expression(ET_FUNCTION), name(name){
	
}

Function::~Function(){
	
}
void Function::evaluate(bool simplify){
	int index = getPrimfuncIndex(name);
	if(index > -1){
		arg->evaluate(simplify);
		numtype v = arg->getValue();
		setValue(getPrimfuncVal(index, v));
		return;
	}
	
	auto f = Expression::functions.find(name);
	if(f != Expression::functions.end()){
		f->second.second->e = arg;
		f->second.first->evaluate(simplify);
		setValue(f->second.first->getValue());
		return;
	}

	if(!simplify) throw expression_exception(name + " is not defined");
}

void Function::setArgument(Expression* arg){
	this->arg = arg;
}
std::string Function::toString(){
	if(hasValue()) return Expression::toString();
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

}
template <int opcount>
std::string Operation<opcount>::toString(){
	if(hasValue()) return Expression::toString();
	try{
		std::string opname;
		switch(opType){
			case OP_ADD:
				opname = "+";
				break;
			case OP_SUB:
				opname = "-";
				break;
			case OP_MUL:
				opname = "*";
				break;
			case OP_DIV:
				opname = "/";
				break;
			case OP_ASSIGN:
				opname = "=";
				break;
			case OP_POW:
				opname = "^";
				break;
			case OP_MOD:
				opname = "%";
				break;
			default:
				opname = "@";
				break;
		}
		if(operands[0] == nullptr || operands[1] == nullptr) return opname;	
		
		bool brackets[2]{false, false};
		int tprio = getPriority(this);
		for(int i = 0; i < 2; i++){
			int prio = getPriority(operands[i]);
			if(prio == -1) continue;
			int tot = ((Operation<2>*)operands[i])->getOperationType();
			if(prio < tprio || (prio == tprio && (tot == OP_SUB || tot == OP_DIV || tot == OP_MOD))) brackets[i] = true;
		}
		if(operands[0]->hasValue()) brackets[0] = false;
		if(operands[1]->hasValue()) brackets[1] = false;
		std::string str = "";
		if(brackets[0]) str += "(";
		if(operands[0]->getType() == ET_FUNCTION){
			str += ((Function*)operands[0])->getName() + std::string("(x)");
		}
		else{
			str += operands[0]->toString();
		}
		if(brackets[0]) str += ")";
		str += opname;
		if(brackets[1]) str += "(";
		str += operands[1]->toString();
		if(brackets[1]) str += ")";
		return str;
	}
	catch(const std::exception& err){
		//return std::string("#") + std::string(err.what()) + std::string("#");
		return "?";
	}
}

Addition::Addition() : Operation<2>(OP_ADD){
	//ctor
}
Addition::~Addition(){
	//dtor
}
void Addition::evaluate(bool simplify){
	operands[0]->evaluate(simplify);
	operands[1]->evaluate(simplify);
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
void Subtraction::evaluate(bool simplify){
	operands[0]->evaluate(simplify);
	operands[1]->evaluate(simplify);
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
void Multiplication::evaluate(bool simplify){
	operands[0]->evaluate(simplify);
	operands[1]->evaluate(simplify);
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
void Division::evaluate(bool simplify){
	operands[0]->evaluate(simplify);
	operands[1]->evaluate(simplify);
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
void Modulo::evaluate(bool simplify){
	operands[0]->evaluate(simplify);
	operands[1]->evaluate(simplify);
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
void Power::evaluate(bool simplify){
	operands[0]->evaluate(simplify);
	operands[1]->evaluate(simplify);
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
void Assign::evaluate(bool simplify){
	if(operands[0] == nullptr){
		if(!simplify) throw expression_exception("Left hand side cannot be null");
		return;
	}
	else if(operands[1] == nullptr){
		if(!simplify) throw expression_exception("Right hand side cannot be null");
		return;
	}
	
	std::vector<Expression*> checkList;
	checkList.push_back(operands[1]);
	while(!checkList.empty()){
		if(checkList.back()->getType() == ET_OPERATION){
			Operation<2>* o = (Operation<2>*)checkList.at(checkList.size()-1);
			checkList.erase(checkList.end()-1);
			checkList.push_back(o->operands[0]);
			checkList.push_back(o->operands[1]);
		}
		else if(checkList.back()->getType() == ET_VARIABLE && operands[0]->getType() == ET_VARIABLE){
			Variable* v = (Variable*)checkList.at(checkList.size()-1);
			checkList.erase(checkList.end()-1);
			if(v->getName() == ((Variable*)operands[0])->getName()){
				if(!simplify) throw expression_exception("Cannot recursively assign variable");
				return;
			}
		}
		else if(checkList.back()->getType() == ET_FUNCTION){
			Function* f = (Function*)checkList.at(checkList.size()-1);
			checkList.erase(checkList.end()-1);
			
			if(operands[0]->getType() == ET_FUNCTION && f->getName() == ((Function*)operands[0])->getName()){
				if(!simplify) throw expression_exception("Cannot recursively assign function");
				return;
			}
			if(f->getArgument()->getType() == ET_VARIABLE && operands[0]->getType() == ET_VARIABLE && ((Variable*)f->getArgument())->getName() == ((Variable*)operands[0])->getName()){
				if(!simplify) throw expression_exception("Cannot assign value that depends on itself");
				return;
			}
		}
		else{
			checkList.erase(checkList.end()-1);
		}
	}
	
	if(operands[0]->getType() == ET_VARIABLE){
		Variable* v = ((Variable*)operands[0]);
		if(Variable::getBuiltInVariableIndex(v->name) > -1){
			if(!simplify) throw expression_exception(v->name + " is a built-in variable and cannot be reassigned");
			return;
		}
		
		if(v->getName() == "y"){
			Function* f = new Function("y");
			Variable* a = new Variable("x");
			f->setArgument(a);
			operands[0] = f;
			goto addFunc;
		}
		
		//Expression::variables[((Variable*)operands[0])->name] = operands[1];
		auto it = Expression::variables.find(v->name);
		if(it == Expression::variables.end()){
			if(v->name == "y"){
				throw expression_exception("Trying to assign y");
			}
			Expression::variables[v->name] = operands[1];
			if(simplify) operands[1]->evaluate(true);
			else{
				try{
					operands[1]->evaluate(false);
				}
				catch(const expression_exception& ex){
					if(ex.get_type() != expression_exception::T_NO_VALUE){
						throw ex;
					}
				}
				catch(const std::exception& e){
					throw e;
				}
			}
		}
		else{
			if(simplify) throw expression_exception(v->name + " already has value");
			return;
		}
		
	}
	else if(operands[0]->getType() == ET_FUNCTION){
		addFunc:
		Function* f = ((Function*)operands[0]);
		if(!f->bDefined){
			std::string argName;
			if(f->arg->getType() == ET_VARIABLE){
				argName = ((Variable*)f->arg)->getName();
				
				if(Function::getPrimfuncIndex(f->name) > -1){
					if(!simplify) throw expression_exception(f->name + " is a built-in function and cannot be reassigned");
					return;
				}
				
				f->arg = nullptr;
				FunctionArgument* fa = new FunctionArgument;
				replaceVariable(&operands[1], fa, argName);
				
				if(f->getName() == "y"){
					Expression::drawFunctions.push_back(std::pair<Expression*, FunctionArgument*>(operands[1], fa));
					//operands[1]->evaluate(simplify);
				}
				else{
					auto it = Expression::functions.find(f->name);
					if(it == Expression::functions.end()){
						Expression::functions[f->name] = std::pair<Expression*, FunctionArgument*>(operands[1], fa);
						//operands[1]->evaluate(simplify);
					}
					else{
						if(simplify) throw expression_exception(f->name + " is already defined");
						return;
					}
				}
			}
			/*if(f->arg->getType() == ET_FUNCTION_ARGUMENT){
				
			}*/
			else{
				if(!simplify) throw expression_exception("Function argument must be a variable");
				return;
			}
			
			f->bDefined = true;
		}
	}
	else{
		if(!simplify) throw expression_exception(std::string("Cannot assign expression ") + operands[0]->toString());
		return;
	}
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

std::string Expression::toString(numtype v){
	std::string str = std::to_string(v);
	if(str.find('.') != std::string::npos){
		for(int i = (int)str.size()-1; i >= 0; i--){
			bool bCut = false;
			if(str[i] != '0') bCut = true;
			if(str[i] == '.') i--;
			if(bCut){
				str = str.substr(0, i+1);
				break;
			}
		}
	}
	return str;
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
		if(priority == -1) throw expression_exception(expressions[i]->toString() + ": Not an operation");
		
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
		else throw expression_exception("Missing right operand");
		
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
	
	Multiplication* negatives = nullptr;
	Multiplication* negativeTop = nullptr;
	for(int i = 0; i < (int)parts.size(); i++){
		
		Expression* e;
		if((i == 0 || parts[i-1].first == T_OPERATOR) && parts[i].second == "-"){
			Multiplication* m = new Multiplication;
			m->operands[0] = new Constant(-1.0);
			if(negatives == nullptr){
				negatives = m;
			}
			else{
				negativeTop->operands[1] = m;
			}
			negativeTop = m;
			continue;
		}
		else{
			e = Expression::toExpression(parts[i]);
			if(negatives != nullptr){
				negativeTop->operands[1] = e;
				e = negatives;
				negatives = nullptr;
			}
		}
		
		if(i > 0 && parts[i].first == T_BRACKET && parts[i-1].first == T_FUNCTION){
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
		
		bool shouldAdd = false;
		if(type == T_OPERATOR || type != oldType){
			shouldAdd = true;
		}
		
		if(shouldAdd){
			addPart:
			if(oldType != T_NONE){
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
	
	for(int i = 1; i < (int)parts.size(); i++){
		if((parts[i].first == T_OPERATOR && parts[i].second != "-") && parts[i-1].first == T_OPERATOR){
			throw expression_exception("Input cannot contain two successive operations");
		}
	}
	
	if((int)parts.size() == 1 && parts[0].first == T_OPERATOR){
		throw expression_exception("Input cannot be a single operation");
	}
	
	return parts;
}

Expression* Expression::getExpression(const std::string& str){
	if(str.empty()) throw expression_exception("Input was empty");
	
	std::vector<std::pair<int, std::string>> parts = toVec(str);

	std::vector<Expression*> expressions = convertVec(parts);
	
	Expression* e = evalVec(expressions, 1, (int)expressions.size());
	
	e->evaluate(true);
	
	return e;
}