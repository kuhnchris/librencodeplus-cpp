#include "./librencodeplus.hpp"
#include "./librencodeplus-constants.hpp"
#include "./utils.hpp"
#include <boost/any.hpp>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <string>
#include <typeinfo>

void encode_specific(string data, string &encodedStr) {
  if (data.length() <= STR_FIXED_COUNT) {
    unsigned char c = STR_FIXED_START + data.length();
    encodedStr = encodedStr + c + data;
  } else {
    unsigned char c = data.length();
    encodedStr = encodedStr + c + (unsigned char)':' + data;
  }
  //std::cout << "Encoding string...: " << encodedStr << "\n";
}

void encode_specific(std::string data, string &encodedStr) {
  string d;
  for (char c : data){
    d = d + static_cast<unsigned char>(c);
  }
  if (data.length() <= STR_FIXED_COUNT) {
    unsigned char c = STR_FIXED_START + data.length();
    encodedStr = encodedStr + c + d;
  } else {
    unsigned char c = data.length();
    encodedStr = encodedStr + c + (unsigned char)':' + d;
  }
  //std::cout << "Encoding string...: " << encodedStr << "\n";
}

void encode_specific(std::map<std::string, boost::any> data,
                     string &encodedStr) {
  //std::cout << "encoding map...\n";
  string strAdd;
  for (const auto &p : data) {
    //std::cout << "attempting to convert key..." << type_name<typeof(p.first)>()
      //        << "\n";
    encode_try_all(p.first, strAdd);
    //std::cout << "attempting to convert value..." << type_name<typeof(p.second)>() << "\n";
    encode_try_all(p.second, strAdd);
  }

  encodedStr = encodedStr + CHR_DICT;
  encodedStr = encodedStr + (strAdd);
  encodedStr = encodedStr + CHR_TERM;

//  std::cout << "encoded map: " << encodedStr << "\n";
}

void encode_specific(std::list<boost::any> data, string &encodedStr) {
  //std::cout << "encoding list...\n";
  string strAdd;
  for (auto a = data.begin(); a != data.end(); ++a) {
    //std::cout << "List has entry of type: " << a->type().name() << '\n';
    encode_try_all(*a, strAdd);
  }
  if (data.size() < LIST_FIXED_COUNT) {
    unsigned char c = LIST_FIXED_START + data.size();
    encodedStr = encodedStr + c;
    encodedStr = encodedStr + strAdd;
  } else {
    encodedStr = encodedStr + CHR_LIST;
    encodedStr = encodedStr + strAdd;
    encodedStr = encodedStr + CHR_TERM;
  }
  //std::cout << "encoded list: " << encodedStr << "\n";
}

template <typename T>
bool encode_any(boost::any data, string &encodedStr) {
  if (is_any_a<T>(data)) {
    encode_specific(boost::any_cast<T>(data), encodedStr);
    return true;
  }
  return false;
}

bool encode_try_all(boost::any data, string &encodedStr) {
  if (encode_any<const char *>(data, encodedStr))
    return true;
  if (encode_any<const std::string>(data, encodedStr))
    return true;
  if (encode_any<std::map<std::string, boost::any>>(data, encodedStr))
    return true;
  if (encode_any<std::list<boost::any>>(data, encodedStr))
    return true;
  std::cout << "could not convert data of type '" << data.type().name()
            << "' to anything useful.\n";
  return false;
}

void encode(boost::any data, string &encodedStr) {
  encode_try_all(data, encodedStr);
}
