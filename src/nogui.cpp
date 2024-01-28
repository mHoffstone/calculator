#include <iostream>
#include <string>
#include <vector>

#include "expression.h"

int main(){
	for(int i = 0; i > -1; i++){
		std::string input;
		std::getline(std::cin, input);
		
		try{
			Expression* e = Expression::getExpression(input);
			e->evaluate(true);
			e->evaluate(false);
			if(e->hasValue()){
				std::cout << e->getValue() << std::endl;
			}
			else std::cout << "No value" << std::endl;
		}
		catch(std::exception& err){
			std::cout << "Exception:" << std::endl;
			std::cout << err.what() << std::endl;
		}
	}
}