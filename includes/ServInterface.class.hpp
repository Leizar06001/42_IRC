#ifndef SERVINTERFACE_CLASS_HPP
# define SERVINTERFACE_CLASS_HPP

#include <string>

class ServInterface {

	private:


	public:
		ServInterface(void);
		~ServInterface(void);
		ServInterface(ServInterface & src);
		ServInterface &operator=(const ServInterface & src);

};

#endif
