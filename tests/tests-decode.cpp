#include "../librencodeplus.hpp"
#include <boost/any.hpp>
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <list>
#include <map>
#include <string>

using namespace rencodeplus::decode;

int test_str();
int test_map();
int test_list();
int test_xpra();

int test_str() {
    string ostr;
    boost::any outObjStrBin;
    int readSize;
    unsigned char *strbindata = readBinaryData("encode-str.bin", readSize);
    std::cout << "Reading and decoding string..."
              << "\n";
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
    return 0;
}

int test_map() {
    std::cout << "Reading and decoding map/dictionary..."
              << "\n";

    string ostr;
    boost::any outObjStrBin;
    int readSize;
    unsigned char *strbindata = strbindata =
        readBinaryData("encode-map.bin", readSize);
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
            std::cout << "Key: " << it2->first << "; Value: "
                      << boost::any_cast<std::string>(it2->second) << "\n";
        else
            std::cout << "Check OK!"
                      << "\n";
    } else {
        return 2;
    }
    return 0;
}

int test_list() {
    std::cout << "Reading and decoding list..."
              << "\n";
    string ostr;
    boost::any outObjStrBin;
    int readSize;
    unsigned char *strbindata = readBinaryData("encode-list.bin", readSize);
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
                std::cout << "Key: " << it->first << "; Value: "
                          << boost::any_cast<std::string>(it->second) << "\n";

            std::cout << "Checking for invalid key 'inv'...\n";
            auto it2 = sout.find("inv");
            if (it2 != sout.end())
                std::cout << "Key: " << it2->first << "; Value: "
                          << boost::any_cast<std::string>(it2->second) << "\n";
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
    return 0;
}

int test_xpra() {
    std::cout << "Reading and decoding real xpra data..."
              << "\n";
    string ostr;
    boost::any outObjStrBin;
    int readSize;
    unsigned char *strbindata =
        readBinaryData("xpra-example-hello.bin", readSize);
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

int main() {
    int res_1 = test_str();
    int res_2 = test_map();
    int res_3 = test_list();
    int res_4 = test_xpra();
}