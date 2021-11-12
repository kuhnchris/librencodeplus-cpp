#include "./librencodeplus-constants.hpp"
#include "./librencodeplus.hpp"
#include "./utils.hpp"
#include <boost/any.hpp>
#include <boost/type_index.hpp>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <new>
#include <string>

void decode_fixed_string(string &inputStr, boost::any &outObject) {
  std::cout << "Extracting fixed string..."
            << "\n";
  int len = inputStr[0] - STR_FIXED_START;
  std::string f =
      reinterpret_cast<const char *>(inputStr.substr(1, len).c_str());
  outObject = f;
  inputStr = inputStr.substr(len + 1);
}

void decode_string(string &inputStr, boost::any &outObject) {}

void decode_fixed_list(string &inputStr, boost::any &outObject) {
  int listLen = inputStr[0] - LIST_FIXED_START;
  inputStr = inputStr.substr(1);
  boost::any innerObj;
  std::list<boost::any> l = std::list<boost::any>();

  for (int i = 0; i < listLen; i++) {
    decode(inputStr, innerObj);
    l.push_back(innerObj);
    innerObj = nullptr;
  }
  outObject = l;
}

void decode_dict(string &inputStr, boost::any &outObject) {
  inputStr = inputStr.substr(1);
  boost::any innerObjK;
  boost::any innerObjV;
  std::map<std::string, boost::any> l = std::map<std::string, boost::any>();
  // std::map<std::string, boost::any>::iterator it = l.begin();
  while (inputStr[0] != CHR_TERM) {
    try {
      std::cout << "Reading key-value pair for dict..."
                << "\n";
      decode(inputStr, innerObjK);
      decode(inputStr, innerObjV);
      std::cout << "Adding to dictionary..."
                << "\n";
      std::string kStr = boost::any_cast<std::string>(innerObjK);
      l.insert(std::pair<std::string, boost::any>(kStr, innerObjV));
      innerObjK = nullptr;
      innerObjV = nullptr;
    } catch (boost::bad_any_cast e) {

      std::cerr << "Could not convert key. Expected: std::string/char*, got: "
                << boost::typeindex::type_index(innerObjK.type()).pretty_name()
                << "\n" 
                << "Error: " << e.what()
                << "\n";
    }
  }
  outObject = l;
}

void decode_list(string &inputStr, boost::any &outObject) {
  inputStr = inputStr.substr(1);
  boost::any innerObj;
  std::list<boost::any> l = std::list<boost::any>();
  while (inputStr[0] != CHR_TERM) {
    decode(inputStr, innerObj);
    l.push_back(innerObj);
    innerObj = nullptr;
  }
  outObject = l;
}

void decode(string &inputStr, boost::any &outObject) {
  switch (inputStr[0]) {
  case CHR_LIST:
    std::clog << "found a list header char"
              << "\n";
    decode_list(inputStr, outObject);
    break;
  case CHR_DICT:
    std::clog << "found a dictionary header char"
              << "\n";
    decode_dict(inputStr, outObject);
    break;
  default:
    if (STR_FIXED_START < inputStr[0] &&
        inputStr[0] < (STR_FIXED_START + STR_FIXED_COUNT)) {
      std::clog << "found a fixed string"
                << "\n";
      decode_fixed_string(inputStr, outObject);
      return;
    } else if (LIST_FIXED_START < inputStr[0] &&
               inputStr[0] < (LIST_FIXED_START + LIST_FIXED_COUNT - 1)) {
      std::clog << "found a fixed list"
                << "\n";
      decode_fixed_list(inputStr, outObject);
      return;
    } else {
      std::cerr << "Could not interpret first character '" << inputStr[0]
                << "' as anything useful. :-(\n";
      return;
    }
  }
}