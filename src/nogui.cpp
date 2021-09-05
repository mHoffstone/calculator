#include <iostream>
#include <string>
#include <vector>

#include "expression.h"

int main(){
	std::cout << "Hello World" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	
	try{
		Expression* e = getExpression(input);
		std::cout << e->getValue() << std::endl;
	}
	catch(std::exception& err){
		std::cout << "Exception:" << std::endl;
		std::cout << err.what() << std::endl;
	}
	
	
	/*Variable v1;
	v1.m_hasValue = true;
	v1.m_value = 4.0;
	
	Variable v2;
	v2.m_hasValue = true;
	v2.m_value = 8.0;
	
	Addition a;
	a.operands[0] = &v1;
	a.operands[1] = &v2;
	
	Variable v3;
	v3.m_hasValue = true;
	v3.m_value = 2.0;
	
	Subtraction s;
	s.operands[0] = &a;
	s.operands[1] = &v3;
	
	a.evaluate();
	s.evaluate();
	
	std::cout << s.getValue() << std::endl;*/
}