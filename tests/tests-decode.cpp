#include "../librencodeplus.hpp"
#include <boost/any.hpp>
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <map>
#include <list>


void outputStrInHex(string str){
    
    for( unsigned char c : str){
        printf("%X|%3d (%c)\n",c,c,c);
    }
    printf("\n");
}

unsigned char* readBinaryData(std::string filename){
    unsigned char* bvar;
    std::basic_ifstream<unsigned char> inF (filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (inF.is_open()){
        int size = inF.tellg();
        bvar = new unsigned char[size];
        inF.seekg(0,std::ios::beg);
        inF.read(bvar, size);
        inF.close();
        return bvar;
    } else {
        std::cerr << "Could not open binary file '" << filename << "'\n";
        return nullptr;
    }
}

int main(){
    string ostr;
    boost::any outObjStrBin;
    unsigned char* strbindata = readBinaryData("encode-str.bin");
    if (strbindata != nullptr){
        string strBin (strbindata);
        decode(strBin, outObjStrBin);
        std::cout << "Decoded: " << boost::any_cast<std::string>(outObjStrBin) << "\n";
    } else {
        return 1;
    }

    /*
    std::string str = "hello";
    std::cout << "Outputting str.. "  << '\n';
    encode(str,ostr);

    std::map<std::string, boost::any> s = {{"hello","hi"}};
    std::cout << "Outputting map.. "  << '\n';
    //encode(ostr, 0, sp);
    encode(s,ostr);

    std::cout << "Outputting list.. "  << '\n';
    std::list<boost::any> l = {};
    
    l.push_back("hello");
    l.push_back(s);
    encode(l,ostr);
    outputStrInHex(ostr);
    return 0;*/
}        