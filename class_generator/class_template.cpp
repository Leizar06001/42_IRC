#include "../includes/ClassName.class.hpp"

ClassName::ClassName(void){};
ClassName::~ClassName(void){};
ClassName::ClassName(ClassName & src){(void)src;};
ClassName& ClassName::operator=(const ClassName & src){
	(void)src;
	return *this;
}
