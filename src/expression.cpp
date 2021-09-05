#include "expression.h"

#include <iostream>
#include <vector>
#include <stdexcept>

Expression::Expression(int t){
	type = t;
}
Expression::~Expression(){
	//dtor
}
double Expression::getValue(){
	if(!hasValue()) throw std::runtime_error("Expression has no value!");
	return m_value;
}
bool Expression::hasValue(){
	return m_hasValue;
}

Variable::Variable() : Expression(ET_VARIABLE){
	//ctor
}
Variable::~Variable(){
	//dtor
}

template <int opcount>
Operation<opcount>::Operation(int t) : Expression(ET_OPERATION){
	opType = t;
}
template <int opcount>
Operation<opcount>::~Operation(){
	delete operands[0];
	delete operands[1];
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
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() + operands[1]->getValue();
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
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() - operands[1]->getValue();
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
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() * operands[1]->getValue();
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
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() / operands[1]->getValue();
	}
}

#if 0
void trim(std::string& str){
	while(str.find("  ") != std::string::npos){
		int pos = str.find("  ");
		str = str.replace(str.begin()+pos, str.begin()+pos+2, 1, ' ');
	}
	bool lastWasSpace = false;
	bool lastWasOperator = false;
	for(int i = 0; i < (int)str.length(); i++){
		char c = str[i];
		if(lastWasOperator){
			if(str[i] != ' '){
				str.insert(str.begin()+i, ' ');
				i++;
			}
			lastWasOperator = false;
		}
		else if(c == '+' || c == '-' || c == '*' || c == '/'){
			if(!lastWasSpace){
				str.insert(str.begin()+i, ' ');
				i++;
			}
			lastWasOperator = true;
		}
		lastWasSpace = str[i] == ' ';
	}
	if(str[0] == ' ') str = str.substr(1);
	if(str[str.length()-1] != ' ') str += ' ';
}

static Expression* evalVec(std::vector<std::string>& parts, int start){
	//std::vector<Operation<2>*> operations;
	
	int skip = 1;
	
	Expression* e = nullptr;
	Operation<2>* op = nullptr;
	Expression* ePrev = nullptr;
	for(int i = 0; i < (int)parts.size(); i += skip){
		skip = 1;
		std::cout << '\"' << parts[i] << '\"' << std::endl;
		if(i % 2 == 0){
			Expression* v;
			if(parts[i][0] == '('){
				v = getExpression(parts[i].substr(1, parts[i].length()-2));
			}
			else{
				v = new Variable;
				v->m_hasValue = true;
				v->m_value = std::stod(parts[i]);
			}
			
			if(i == 0){
				ePrev = v;
				if(parts.size() == 1){
					e = v;
				}
			}
			else{
				op->operands[0] = ePrev;
				op->operands[1] = v;
				ePrev = op;
			}
		}
		else{
			int c = parts[i];
			
			// Priority rules
			if(c == '+' || c == '-'){
				if(i+2 < (int)parts.size()){
					if(parts[i+2] == '*' || parts[i+2] == '/'){
						skip = 3;
						evalVec(parts, i+1);
						continue;
					}
				}
			}
			
			/*
			if(c == "+"){
				op = new Addition;
			}
			else if(c == "-"){
				op = new Subtraction;
			}
			else if(c == "*"){
				op = new Multiplication;
			}
			else if(c == "/"){
				op = new Division;
			}
			*/
			e = op;
			//operations.push_back(op);
		}
	}
	
	/*for(int i = 0; i < (int)operations.size(); i++){
		operations[i]->evaluate();
	}*/
	e->evaluate();
	
	return e;
}
#endif

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
int getPriority(const std::string& str){
	if(str == "+" || str == "-") return 0;
	if(str == "*" || str == "/") return 1;
	if(str == "^") return 2;
	else return -1;
}

static Expression* toExpr(const std::string& str){
	if(isNumber(str)){
		Variable* v = new Variable;
		v->m_hasValue = true;
		v->m_value = std::stod(str);
		return v;
	}
	else if(str[0] == '('){
		return getExpression(str.substr(1, str.length()-2));
	}
	else throw std::runtime_error(str + ": not an operand!");
}

static Expression* evalVec(const std::vector<std::string>& parts, int start, int last){
	Expression* ls = nullptr;
	Expression* rs = nullptr;
	
	ls = toExpr(parts[start-1]);
	
	int inc = 2;
	for(int i = start; i < last; i += inc){
		inc = 2;
		int priority = getPriority(parts[i]);
		if(priority == -1) throw std::runtime_error(parts[i] + ": Not an operation");
		
		if(i+2 < last && getPriority(parts[i+2]) > priority){
			int nLast = last;
			// Find the end of right operand
			for(int j = i+2; j < last; j += 2){
				int p = getPriority(parts[j]);
				if(p <= priority){
					nLast = j;
					break;
				}
			}
			
			rs = evalVec(parts, i+2, nLast);
			inc = nLast-i;
		}
		else if(i+1 < last){
			rs = toExpr(parts[i+1]);
		}
		else throw std::runtime_error(parts[i] + "Missing right operand.");
		
		Operation<2>* op;
		if(parts[i] == "+") op = new Addition;
		else if(parts[i] == "-") op = new Subtraction;
		else if(parts[i] == "*") op = new Multiplication;
		else if(parts[i] == "/") op = new Division;
		
		std::cout << ls->m_value << parts[i] << rs->m_value << std::endl;
		op->operands[0] = ls;
		op->operands[1] = rs;
		ls = op;
		
	}
	
	return ls;
}

Expression* getExpression(const std::string& str){
	if(str.empty()) throw std::runtime_error("Input was empty.");

	int mode = 0, oldMode = 0; // 0 none; 1 digit; 2 operation; 3 brackets
	int beg = 0;
	bool bBracket = false;
	int cBracket = 0;
	std::vector<std::string> parts;
	for(int i = 0; i < (int)str.length(); i++){
		char c = str[i];
		
		oldMode = mode;
		
		if(!bBracket){
			if((c >= '0' && c <= '9') || c == '.'){
				mode = 1;
			}
			else if(c == '+' || c == '-' || c == '*' || c == '/'){
				mode = 2;
			}
			else if(c == ' '){
				mode = 0;
			}
		}
		if(c == '('){
			//if(!bBracket) bBeg = i;
			cBracket++;
			bBracket = true;
			mode = 3;
		}
		else if(c == ')'){
			cBracket--;
			if(cBracket == 0){
				bBracket = false;
				//i++;
				//parts.push_back(str.substr(bBeg, i+1-bBeg));
				//std::cout << '\"' << str.substr(bBeg, i+1-bBeg) << '\"' << std::endl;
				
			}
		}
		
		if(mode != oldMode){
			//std::cout << beg << ", " << i << std::endl;
			if(oldMode != 0){
				addPart:
				parts.push_back(str.substr(beg, i-beg));
				std::cout << '\"' << str.substr(beg, i-beg) << '\"' << std::endl;
			}
			beg = i;
		}
		if(i == (int)str.length()-1){
			i++;
			goto addPart;
		}
	}
	std::cout << std::endl;
	
	if(cBracket > 0) throw std::runtime_error("Unclosed bracket!");
	else if(cBracket < 0) throw std::runtime_error("Unopened bracket!");
	
	Expression* e = evalVec(parts, 1, (int)parts.size());
	e->evaluate();
	
	return e;
}