#include "../librencodeplus.hpp"
#include "../utils.hpp"
#include <boost/any.hpp>
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <list>
#include <map>
#include <string>

void outputStrInHex(string str) {

  for (unsigned char c : str) {
    printf("%X|%3d (%c)\n", c, c, c);
  }
  printf("\n");
}

unsigned char *readBinaryData(std::string filename, int &size) {
  char *bvar;
  std::basic_ifstream<char> inF(filename, std::ios::in | std::ios::binary |
                                              std::ios::ate);
  if (inF.is_open()) {
    size = inF.tellg();
    bvar = new char[size];
    inF.seekg(0, std::ios::beg);
    inF.read(bvar, size);
    inF.close();
    return reinterpret_cast<unsigned char *>(bvar);
  } else {
    std::cerr << "Could not open binary file '" << filename << "'\n";
    return nullptr;
  }
}

int main() {
  string ostr;
  boost::any outObjStrBin;

  std::cout << "Reading and decoding string..."
            << "\n";
  int readSize;
  unsigned char *strbindata = readBinaryData("encode-str.bin", readSize);
  if (strbindata != nullptr) {
    string strBin(strbindata);
    decode(strBin, outObjStrBin);
    std::cout << "Decoding done, printing...\n";
    std::string sout = boost::any_cast<std::string>(outObjStrBin);
    std::cout << "Decoded: "
              << "\n";
    outputDataStructure(outObjStrBin, 0);
  } else {
    return 1;
  }

  outObjStrBin.empty();
  std::cout << "Reading and decoding map/dictionary..."
            << "\n";
  strbindata = readBinaryData("encode-map.bin", readSize);
  if (strbindata != nullptr) {
    string strBin(strbindata);
    decode(strBin, outObjStrBin);
    std::cout << "Decoding done, printing...\n";
    std::map<std::string, boost::any> sout =
        boost::any_cast<std::map<std::string, boost::any>>(outObjStrBin);
    std::cout << "Decoded: " << sout.size() << " items in dictionary; \n";
    outputDataStructure(outObjStrBin, 0);
    std::cout << "Checking for key 'hello'...\n";
    auto it = sout.find("hello");
    if (it != sout.end())
      std::cout << "Key: " << it->first
                << "; Value: " << boost::any_cast<std::string>(it->second)
                << "\n";

    std::cout << "Checking for invalid key 'inv'...\n";
    auto it2 = sout.find("inv");
    if (it2 != sout.end())
      std::cout << "Key: " << it2->first
                << "; Value: " << boost::any_cast<std::string>(it2->second)
                << "\n";
    else
      std::cout << "Check OK!"
                << "\n";
  } else {
    return 2;
  }

  outObjStrBin.empty();
  std::cout << "Reading and decoding list..."
            << "\n";
  strbindata = readBinaryData("encode-list.bin", readSize);
  if (strbindata != nullptr) {
    try {
      string strBin(strbindata);
      decode(strBin, outObjStrBin);
      std::cout << "Decoding done, printing...\n";
      std::list<boost::any> b =
          boost::any_cast<std::list<boost::any>>(outObjStrBin);
      std::cout << "Decoded: " << b.size() << " items in list.\n";
      outputDataStructure(outObjStrBin, 0);
      std::cout << "First object should be a string..."
                << "\n";
      std::string s = boost::any_cast<std::string>(b.front());
      std::cout << "Decoded as: " << s << "\n";

      b.pop_front();
      std::cout << "popped queue, new 'first' element should be a dict."
                << "\n";
      std::map<std::string, boost::any> sout =
          boost::any_cast<std::map<std::string, boost::any>>(b.front());
      std::cout << "Decoded. Items: " << sout.size() << " items\n";
      auto it = sout.find("hello");
      if (it != sout.end())
        std::cout << "Key: " << it->first
                  << "; Value: " << boost::any_cast<std::string>(it->second)
                  << "\n";

      std::cout << "Checking for invalid key 'inv'...\n";
      auto it2 = sout.find("inv");
      if (it2 != sout.end())
        std::cout << "Key: " << it2->first
                  << "; Value: " << boost::any_cast<std::string>(it2->second)
                  << "\n";
      else
        std::cout << "Check OK!"
                  << "\n";

    } catch (boost::bad_any_cast w) {
      // std::cout << "Error casting. a_s (any) is really: " <<
      // boost::typeindex::type_index(a_s.type()).pretty_name() << "\n";
      std::cout << "casting error: " << w.what() << "\n";
    }
  } else {
    return 3;
  }

  outObjStrBin.empty();
  std::cout << "Reading and decoding real xpra data..."
            << "\n";
  strbindata = readBinaryData("xpra-example-hello.bin", readSize);
  if (strbindata != nullptr) {
    try {
      string strBin(strbindata, readSize);
      decode(strBin, outObjStrBin);
      std::cout << "Decoding done, printing...\n";
      std::list<boost::any> b =
          boost::any_cast<std::list<boost::any>>(outObjStrBin);
      std::cout << "Decoded: " << b.size() << " items in list.\n";
      outputDataStructure(outObjStrBin, 0);
      std::cout << "Check OK!"
                << "\n";

    } catch (boost::bad_any_cast w) {
      // std::cout << "Error casting. a_s (any) is really: " <<
      // boost::typeindex::type_index(a_s.type()).pretty_name() << "\n";
      std::cout << "casting error: " << w.what() << "\n";
    }
  } else {
    return 3;
  }

  return 0;
}