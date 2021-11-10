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

template <typename T> std::string type_name();
#include "utils.cpp"
#endif