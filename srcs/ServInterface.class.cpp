#include "../includes/ServInterface.class.hpp"

ServInterface::ServInterface(void){};
ServInterface::~ServInterface(void){};
ServInterface::ServInterface(ServInterface & src){(void)src;};
ServInterface& ServInterface::operator=(const ServInterface & src){
	(void)src;
	return *this;
}
