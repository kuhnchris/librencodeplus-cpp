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

int decode_file(std::string filename) {
  string ostr;
  boost::any outObjStrBin;
  int readSize;
  unsigned char *strbindata = readBinaryData(filename, readSize);
  std::cout << "Reading and decoding file... " << filename << " (size: " << readSize << ")"
            << "\n";
  if (strbindata != nullptr) {
    string strBin(strbindata, readSize);
    decode(strBin, outObjStrBin);
    std::cout << "Decoding done, printing...\n";
    outputDataStructure(outObjStrBin, 0);
    std::cout << "\n";
  } else {
    return 1;
  }
  return 0;
}

int main(int argc, char** argv) {
     if (argc > 1){
        if (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0){
            std::cout << "Decode tool for librencodeplus." << "\n";
            std::cout << "usage: " << argv[0] << " [filename]" << "\n\n";
            std::cout << "Options:" << "\n";
            std::cout << "--help | -h - help (this)" << "\n";
            return 0;
        } else {
          decode_file(argv[1]);
        }
    }
}