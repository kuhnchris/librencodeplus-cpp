#ifndef _RENCODEPLUSCPP
#define _RENCODEPLUSCPP
#include <string>
#include <map>
#include <list>
#include <boost/any.hpp>


typedef std::basic_string<unsigned char> string;

void encode_specific(string data, string &encodedStr) ;
void encode_specific(std::string data, string &encodedStr) ;
template<typename T> bool is_any_a(boost::any a);
void encode_specific(std::map<std::string, boost::any> data, string &encodedStr) ;
template<typename T> bool encode_any(boost::any data, string& encodedStr);
//void encode_specific(boost::any data, string& encodedStr);
bool encode_try_all(boost::any data, string &encodedStr);
void encode(boost::any data, string &encodedStr);
void encode_specific(std::list<boost::any> data, string &encodedStr) ;

#endif