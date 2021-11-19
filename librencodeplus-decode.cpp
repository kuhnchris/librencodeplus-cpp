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

bool decode_fixed_int(string &inputStr, boost::any &outObject) {
  try {
    int len = inputStr[0] - INT_POS_FIXED_START;
#ifdef DEBUG
    std::cout << "Extracting fixed int..."
              << "\n";
#endif
    unsigned int f = len;
#ifdef DEBUG
    std::cout << "Extracted: " << f << "\n";
#endif
    outObject = f;
    inputStr = string(&inputStr[1], inputStr.size() - 1);
  } catch (boost::bad_any_cast c) {
    std::cout << "Extracting fixed int from rencode failed."
              << "\n";
    outObject = -1;
    return false;
  }
  return true;
}
bool decode_fixed_neg_int(string &inputStr, boost::any &outObject) {
  try {
    int len = inputStr[0] - INT_NEG_FIXED_START * -1;
#ifdef DEBUG
    std::cout << "Extracting fixed neg int..."
              << "\n";
#endif
    int f = len;
#ifdef DEBUG
    std::cout << "Extracted: " << f << "\n";
#endif
    outObject = f;
    inputStr = string(&inputStr[1], inputStr.size() - 1);
  } catch (boost::bad_any_cast c) {
    std::cout << "Extracting fixed neg int from rencode failed."
              << "\n";
    outObject = -1;
    return false;
  }
  return true;
}

bool decode_int_1(string &inputStr, boost::any &outObject) {
  try {
#ifdef DEBUG
    std::cout << "Extracting int (1 byte)..."
              << "\n";
#endif
    unsigned int f = inputStr[1];
#ifdef DEBUG
    std::cout << "Extracted: " << f << "\n";
#endif
    outObject = f;
    inputStr = inputStr.substr(2);
  } catch (boost::bad_any_cast c) {
    std::cout << "Extracting int1 from rencode failed."
              << "\n";
    return false;
  }
  return true;
}

bool decode_int_2(string &inputStr, boost::any &outObject) {
  try {
#ifdef DEBUG
    std::cout << "Extracting int (2 byte)..."
              << "\n";
#endif
    long long f = (inputStr[2] << 8) + (inputStr[3]);
#ifdef DEBUG
    std::cout << "Extracted: " << f << "\n";
#endif
    outObject = f;
    inputStr = inputStr.substr(3);
  } catch (boost::bad_any_cast c) {
    std::cout << "Extracting int2 from rencode failed."
              << "\n";
    return false;
  }
  return true;
}

bool decode_int_4(string &inputStr, boost::any &outObject) {
  try {
#ifdef DEBUG
    std::cout << "Extracting int (4 byte)..."
              << "\n";
#endif
    long long f = (inputStr[1] << 24) + (inputStr[2] << 16) +
                  (inputStr[3] << 8) + (inputStr[4]);
#ifdef DEBUG
    std::cout << "Extracted: " << f << "\n";
#endif
    outObject = f;
    inputStr = inputStr.substr(5);
  } catch (boost::bad_any_cast c) {
    std::cout << "Extracting int4 from rencode failed."
              << "\n";
    return false;
  }
  return true;
}

bool decode_fixed_string(string &inputStr, boost::any &outObject) {
  try {
#ifdef DEBUG
    std::cout << "Extracting fixed string..."
              << "\n";
#endif
    int len = inputStr[0] - STR_FIXED_START;
    std::string f =
        reinterpret_cast<const char *>(inputStr.substr(1, len).c_str());
#ifdef DEBUG
    std::cout << "Extracted: " << f << "\n";
#endif
    outObject = f;
    inputStr = inputStr.substr(len + 1);
  } catch (boost::bad_any_cast c) {
    std::cout << "Extracting fixed string from rencode failed."
              << "\n";
    outObject = "";
    return false;
  }
  return true;
}

bool decode_string(string &inputStr, boost::any &outObject) { return false; }

bool decode_fixed_list(string &inputStr, boost::any &outObject) {
  int listLen = inputStr[0] - LIST_FIXED_START;
  inputStr = inputStr.substr(1);
  boost::any innerObj;
  std::list<boost::any> l = std::list<boost::any>();

  for (int i = 0; i < listLen; i++) {
    if (!decode(inputStr, innerObj)) {
      outObject = l;
      return false;
    }

    l.push_back(innerObj);
    innerObj = nullptr;
  }
  outObject = l;
  return true;
}

bool decode_dict(string &inputStr, boost::any &outObject) {
  inputStr = inputStr.substr(1);
  boost::any innerObjK;
  boost::any innerObjV;
  std::map<std::string, boost::any> l = std::map<std::string, boost::any>();
  while (inputStr[0] != CHR_TERM) {
    try {
#ifdef DEBUG
      std::cout << "Reading key-value pair for dict..."
                << "\n";
#endif
      if (!decode(inputStr, innerObjK)) {
        outObject = l;
        return false;
      }
      if (!decode(inputStr, innerObjV)) {

        outObject = l;
        return false;
      }
#ifdef DEBUG
      std::cout << "Adding to dictionary..."
                << "\n";
#endif
      std::string kStr;
      try {
        kStr = boost::any_cast<std::string>(innerObjK);
      } catch (boost::bad_any_cast e) {

        std::cerr
            << "Could not convert key. Expected: std::string/char*, got: "
            << boost::typeindex::type_index(innerObjK.type()).pretty_name()
            << "\n"
            << "Error: " << e.what() << "\n";
        outObject = l;
        return false;
      }
      try {
        l.insert(std::pair<std::string, boost::any>(kStr, innerObjV));
      } catch (boost::bad_any_cast e) {

        std::cerr
            << "Could not convert value. Expected: boost::any, got: "
            << boost::typeindex::type_index(innerObjK.type()).pretty_name()
            << "\n"
            << "Error: " << e.what() << "\n";
        outObject = l;
        return false;
      }
      innerObjK = nullptr;
      innerObjV = nullptr;
    } catch (boost::bad_any_cast e) {

      std::cerr << "Additional Dictionary error. "
                << "\n"
                << "Error: " << e.what() << "\n";
      outObject = l;
      return false;
    }
  }
  if (inputStr.size() > 0)
    inputStr = inputStr.substr(1);
  outObject = l;
  return true;
}

bool decode_fixed_dict(string &inputStr, boost::any &outObject) {
  int cnt = inputStr[0] - DICT_FIXED_START;
  inputStr = inputStr.substr(1);

  boost::any innerObjK;
  boost::any innerObjV;
  std::map<std::string, boost::any> l = std::map<std::string, boost::any>();
  for (int i = 0; i < cnt; i++) {
    try {
#ifdef DEBUG
      std::cout << "Reading key-value pair for fixed dict..."
                << "\n";
#endif
      if (!decode(inputStr, innerObjK)) {
        outObject = l;
        return false;
      }
      if (!decode(inputStr, innerObjV)) {
        outObject = l;
        return false;
      }
#ifdef DEBUG
      std::cout << "Adding to fixed dictionary..."
                << "\n";
#endif
      std::string kStr = boost::any_cast<std::string>(innerObjK);
      l.insert(std::pair<std::string, boost::any>(kStr, innerObjV));
      innerObjK = nullptr;
      innerObjV = nullptr;
    } catch (boost::bad_any_cast e) {

      std::cerr << "Could not convert key. Expected: std::string/char*, got: "
                << boost::typeindex::type_index(innerObjK.type()).pretty_name()
                << "\n"
                << "Error: " << e.what() << "\n";
      outObject = l;
      return false;
    }
  }
  outObject = l;
  return true;
}

bool decode_list(string &inputStr, boost::any &outObject) {
  inputStr = inputStr.substr(1);
  boost::any innerObj;
  std::list<boost::any> l = std::list<boost::any>();
  while (inputStr[0] != CHR_TERM) {
    if (!decode(inputStr, innerObj)){
      outObject = l;
      return false;
    }
    l.push_back(innerObj);
    innerObj = nullptr;
  }
  outObject = l;
  return true;
}

bool decode(string &inputStr, boost::any &outObject) {
#ifdef DEBUG
  std::cout << "inputStr still has a size() of: " << inputStr.size() << "\n";
#endif
  switch (inputStr[0]) {
  case CHR_TRUE:
#ifdef DEBUG
    std::cout << "found const 'true'"
              << "\n";
#endif
    outObject = true;
    inputStr = inputStr.substr(1);
    return true;
  case CHR_FALSE:
#ifdef DEBUG
    std::cout << "found const 'false'"
              << "\n";
#endif
    outObject = false;
    inputStr = inputStr.substr(1);
    return true;
  case CHR_INT1:
#ifdef DEBUG
    std::cout << "found a int1 header char"
              << "\n";
#endif
    if (!decode_int_1(inputStr, outObject))
      return false;
    return true;
  case CHR_INT2:
#ifdef DEBUG
    std::cout << "found a int2 header char"
              << "\n";
#endif
    if (!decode_int_2(inputStr, outObject))
      return false;
    return true;
  case CHR_INT4:
#ifdef DEBUG
    std::cout << "found a int4 header char"
              << "\n";
#endif
    if (!decode_int_4(inputStr, outObject))
      return false;
    return true;
  case CHR_LIST:
#ifdef DEBUG
    std::cout << "found a list header char"
              << "\n";
#endif
    if (!decode_list(inputStr, outObject))
      return false;
    return true;
  case CHR_DICT:
#ifdef DEBUG
    std::cout << "found a dictionary header char"
              << "\n";
#endif
    if (!decode_dict(inputStr, outObject))
      return false;
    return true;
  default:

    if (DICT_FIXED_START <= inputStr[0] &&
        inputStr[0] <= (DICT_FIXED_START + DICT_FIXED_COUNT)) {
#ifdef DEBUG
      std::cout << "found a fixed +int"
                << "\n";
#endif
      if (!decode_fixed_dict(inputStr, outObject))
        return false;
      return true;
    }

    if (INT_POS_FIXED_START <= inputStr[0] &&
        inputStr[0] <= (INT_POS_FIXED_START + INT_POS_FIXED_COUNT)) {
#ifdef DEBUG
      std::cout << "found a fixed +int"
                << "\n";
#endif
      if (!decode_fixed_int(inputStr, outObject))
        return false;
      return true;
    }
    if (INT_NEG_FIXED_START <= inputStr[0] &&
        inputStr[0] <= (INT_NEG_FIXED_START + INT_NEG_FIXED_COUNT)) {
#ifdef DEBUG
      std::cout << "found a fixed -int"
                << "\n";
#endif
      if (!decode_fixed_neg_int(inputStr, outObject))
        return false;
      return true;
    }

    if (STR_FIXED_START <= inputStr[0] &&
        inputStr[0] < (STR_FIXED_START + STR_FIXED_COUNT)) {
#ifdef DEBUG
      std::cout << "found a fixed string"
                << "\n";
#endif
      if (!decode_fixed_string(inputStr, outObject))
        return false;
      return true;
    } else if (LIST_FIXED_START <= inputStr[0] &&
               inputStr[0] <= (LIST_FIXED_START + LIST_FIXED_COUNT - 1)) {
#ifdef DEBUG
      std::cout << "found a fixed list"
                << "\n";
#endif
      if (!decode_fixed_list(inputStr, outObject))
        return false;
      return true;
    } else {
      std::cerr << "Could not interpret first character '" << inputStr[0]
                << "(0x" << std::hex << static_cast<unsigned int>(inputStr[0])
                << "," << std::dec << static_cast<unsigned int>(inputStr[0])
                << ")"
                << "' as anything useful. :-(\n";

      inputStr = inputStr.substr(1);
      return false;
    }
  }
  return false;
}