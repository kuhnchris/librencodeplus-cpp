#ifndef UTILSCPP
#define UTILSCPP
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <boost/any.hpp>

template<typename T> std::string type_name();
template<typename T> bool is_any_a(boost::any a);

#include "utils.cpp"
#endif