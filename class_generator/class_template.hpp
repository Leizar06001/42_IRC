#ifndef CLASSNAME_HPP
# define CLASSNAME_HPP

#include <string>

class ClassName {

	private:


	public:
		ClassName(void);
		~ClassName(void);
		ClassName(ClassName & src);
		ClassName &operator=(const ClassName & src);

};

#endif
