#include "expression.h"

#include <iostream>
#include <vector>

Expression::Expression(){
	//ctor
}
Expression::~Expression(){
	//dtor
}

Variable::Variable(){
	//ctor
}
Variable::~Variable(){
	//dtor
}

template <int opcount>
Operation<opcount>::Operation(){
	//ctor
}
template <int opcount>
Operation<opcount>::~Operation(){
	//dtor
}

Addition::Addition(){
	//ctor
}
Addition::~Addition(){
	//dtor
}
void Addition::evaluate(){
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() + operands[1]->getValue();
	}
}

Subtraction::Subtraction(){
	//ctor
}
Subtraction::~Subtraction(){
	//dtor
}
void Subtraction::evaluate(){
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() - operands[1]->getValue();
	}
}

Multiplication::Multiplication(){
	//ctor
}
Multiplication::~Multiplication(){
	//dtor
}
void Multiplication::evaluate(){
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() * operands[1]->getValue();
	}
}

Division::Division(){
	//ctor
}
Division::~Division(){
	//dtor
}
void Division::evaluate(){
	m_hasValue = operands[0]->hasValue() && operands[1]->hasValue();
	if(m_hasValue){
		m_value = operands[0]->getValue() / operands[1]->getValue();
	}
}

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
Expression* getExpression(const std::string& str){
	std::string input = str;
	trim(input);
	
	std::cout << input << std::endl;
	
	size_t cBeg = 0;
	std::vector<std::string> parts;
	for(int i = 0; i < (int)input.length(); i++){
		if(input[i] == ' '){
			parts.push_back(input.substr(cBeg, i-cBeg));
			cBeg = i + 1;
		}
	}
	
	std::vector<Operation<2>*> operations;
	
	Expression* e = nullptr;
	Operation<2>* op = nullptr;
	Expression* ePrev = nullptr;
	for(int i = 0; i < (int)parts.size(); i++){
		std::cout << '\"' << parts[i] << '\"' << std::endl;
		if(i % 2 == 0){
			Variable* v = new Variable;
			v->m_hasValue = true;
			v->m_value = std::stod(parts[i]);
			
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
			if(parts[i] == "+"){
				op = new Addition;
			}
			else if(parts[i] == "-"){
				op = new Subtraction;
			}
			else if(parts[i] == "*"){
				op = new Multiplication;
			}
			else if(parts[i] == "/"){
				op = new Division;
			}
			e = op;
			operations.push_back(op);
		}
	}
	
	for(int i = 0; i < (int)operations.size(); i++){
		operations[i]->evaluate();
	}
	
	return e;
}