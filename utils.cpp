/*https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/81886*/
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <boost/any.hpp>

template <typename T> bool is_any_a(boost::any a) {
  try {
    T i = boost::any_cast<T>(a);
  } catch (boost::bad_any_cast e) {
    // std::cout << "error, no " << type_name<T>() << "\n";
    return false;
  }
  return true;
}

template <typename T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}