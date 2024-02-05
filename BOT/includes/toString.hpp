#ifndef TOSTRING_HPP
#define TOSTRING_HPP

#include <sstream>
#include <string>

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

#endif
