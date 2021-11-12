#include "../librencodeplus.hpp"
#include <boost/any.hpp>
#include <fstream>
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

int writeToFile(std::string fileName, string content){
    std::ofstream fout (fileName, std::ios::out|std::ios::trunc|std::ios::binary);
    if (fout.is_open()){
        std::cout << "exporting " << content.size() << " bytes...\n";
        fout.write(reinterpret_cast<const char*>(content.c_str()),content.size());
        fout.flush();
        fout.close();
        return 0;
    }else{
        std::cerr << "Could not open file '" << fileName << "' for writing.";
        return 1;
    } 
}

int main(int argc, char** argv){
    bool outAsFiles = false;

    if (argc > 1){
        if (strcmp(argv[1],"--write") == 0){
            outAsFiles = true;
        }
        if (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0){
            std::cout << "Test app for librencodeplus." << "\n";
            std::cout << "Options:" << "\n";
            std::cout << "--write     - outputs the encoded strings into files" << "\n";
            std::cout << "--help | -h - help (this)" << "\n";
            return 0;
        }
    }
    string ostr;
    std::string str = "hello";
    std::cout << "Outputting str.. "  << '\n';
    encode(str,ostr);
    if (outAsFiles)
        writeToFile("encode-str.bin", ostr);

    ostr.clear(); 
    std::map<std::string, boost::any> s = {{"hello","hi"}};
    std::cout << "Outputting map.. "  << '\n';
    //encode(ostr, 0, sp);
    encode(s,ostr);
    if (outAsFiles)
        writeToFile("encode-map.bin", ostr);

    ostr.clear();
    std::cout << "Outputting list.. "  << '\n';
    std::list<boost::any> l = {};
    l.push_back("hello");
    l.push_back(s);
    encode(l,ostr);
    if (outAsFiles)
        writeToFile("encode-list.bin", ostr);

    outputStrInHex(ostr);
    return 0;
}        