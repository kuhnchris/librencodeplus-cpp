#include "./librencodeplus-constants.hpp"
#include "./librencodeplus.hpp"
#include "./utils.hpp"
#include <boost/any.hpp>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <new>
#include <string>
#include <typeinfo>

void decode_fixed_string(string &inputStr, boost::any &outObject){
    int len = inputStr[0] - STR_FIXED_START;
    outObject = inputStr.substr(1,len);
    inputStr = inputStr.substr(len+1);
}


void decode_string(string &inputStr, boost::any &outObject){

}

void decode_fixed_list(string &inputStr, boost::any &outObject) {
  int listLen = inputStr[0] - LIST_FIXED_START;
  inputStr = inputStr.substr(1);
  boost::any innerObj;
  std::list<boost::any> l = std::list<boost::any>();
  for (int i = 0; i < listLen - LIST_FIXED_START; i++) {
    decode(inputStr, innerObj);
    l.push_back(innerObj);
    innerObj = nullptr;
  }
  outObject = l;
}
void decode_list(string &inputStr, boost::any &outObject){
    inputStr = inputStr.substr(1);
    boost::any innerObj;
    std::list<boost::any> l = std::list<boost::any>();
    while(inputStr[0] != CHR_TERM){
        decode(inputStr, innerObj);
        l.push_back(innerObj);
        innerObj = nullptr;
    }
    outObject = l;
}

void decode(string &inputStr, boost::any &outObject) {
  switch (inputStr[0]) {
  case CHR_LIST:
    decode_list(inputStr, outObject);
  default:
    if (STR_FIXED_START < inputStr[0] &&
        inputStr[0] < (STR_FIXED_START + STR_FIXED_COUNT)) {
      decode_fixed_string(inputStr, outObject);
      return;
    }
    if (LIST_FIXED_START < inputStr[0] &&
        inputStr[0] < (LIST_FIXED_START + LIST_FIXED_COUNT - 1)) {
      decode_fixed_list(inputStr, outObject);
      return;
    }
  }
}