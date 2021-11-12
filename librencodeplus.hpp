#ifndef _RENCODEPLUSCPP
#define _RENCODEPLUSCPP
#include <string>
#include <map>
#include <list>
#include <boost/any.hpp>


typedef std::basic_string<unsigned char> string;

void encode_specific(string data, string &encodedStr) ;
void encode_specific(std::string data, string &encodedStr) ;
void encode_specific(std::map<std::string, boost::any> data, string &encodedStr) ;
void encode_specific(std::list<boost::any> data, string &encodedStr) ;

template<typename T> bool encode_any(boost::any data, string& encodedStr);
bool encode_try_all(boost::any data, string &encodedStr);

void encode(boost::any data, string &encodedStr);

bool decode(string &inputStr, boost::any &outObject);
bool decode_list(string &inputStr, boost::any &outObject);
bool decode_fixed_list(string &inputStr, boost::any &outObject);
bool decode_string(string &inputStr, boost::any &outObject);
bool decode_fixed_string(string &inputStr, boost::any &outObject);
#endif