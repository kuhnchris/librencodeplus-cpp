#include "../librencodeplus.hpp"
#include <boost/any.hpp>
#include <list>
#include <map>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
    bool outAsFiles = false;

    if (argc > 1) {
        if (strcmp(argv[1], "--write") == 0) {
            outAsFiles = true;
        }
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            std::cout << "Test app for librencodeplus."
                      << "\n";
            std::cout << "Options:"
                      << "\n";
            std::cout << "--write     - outputs the encoded strings into files"
                      << "\n";
            std::cout << "--help | -h - help (this)"
                      << "\n";
            return 0;
        }
    }
    string ostr;
    std::string str = "hello";
    std::cout << "Outputting str.. " << '\n';
    encode(str, ostr);
    if (outAsFiles)
        writeToFile("encode-str.bin", ostr);

    ostr.clear();
    std::map<std::string, boost::any> s = {{"hello", "hi"}};
    std::cout << "Outputting map.. " << '\n';
    // encode(ostr, 0, sp);
    encode(s, ostr);
    if (outAsFiles)
        writeToFile("encode-map.bin", ostr);

    ostr.clear();
    std::cout << "Outputting list.. " << '\n';
    std::list<boost::any> l = {};
    l.push_back("hello");
    l.push_back(s);
    encode(l, ostr);
    if (outAsFiles)
        writeToFile("encode-list.bin", ostr);

    outputStrInHex(ostr);
    return 0;
}