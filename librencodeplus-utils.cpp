/* Some code taken from:
 * https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/81886
 */
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include "librencodeplus.hpp"
#include <boost/any.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>

using namespace rencodeplus::utils;

namespace rencodeplus {
    namespace utils {

        void outputStrInHex(string str) {
            for (unsigned char c : str) {
                printf("%X|%3d (%c)\n", c, c, c);
            }
            printf("\n");
        }

        unsigned char *readBinaryData(std::string filename, int &size) {
            char *bvar;
            std::basic_ifstream<char> inF(
                filename, std::ios::in | std::ios::binary | std::ios::ate);
            if (inF.is_open()) {
                size = inF.tellg();
                bvar = new char[size];
                inF.seekg(0, std::ios::beg);
                inF.read(bvar, size);
                inF.close();
                return reinterpret_cast<unsigned char *>(bvar);
            } else {
                std::cerr << "Could not open binary file '" << filename
                          << "'\n";
                return nullptr;
            }
        }

        int writeToFile(std::string fileName, string content) {
            std::ofstream fout(fileName, std::ios::out | std::ios::trunc |
                                             std::ios::binary);
            if (fout.is_open()) {
                std::cout << "exporting " << content.size() << " bytes...\n";
                fout.write(reinterpret_cast<const char *>(content.c_str()),
                           content.size());
                fout.flush();
                fout.close();
                return 0;
            } else {
                std::cerr << "Could not open file '" << fileName
                          << "' for writing.";
                return 1;
            }
        }

        void outputDataStructure(boost::any any, int level, bool prefix) {
            if (prefix) {
                for (int i = 0; i < level; i++)
                    std::cout << " ";
                std::cout << "* ";
            }
            if (is_any_a<std::list<boost::any>>(any)) {
                std::cout << "(list)\n";
                std::list<boost::any> l =
                    boost::any_cast<std::list<boost::any>>(any);
                for (auto ll : l) {
                    rencodeplus::utils::outputDataStructure(ll, level + 2,false);
                    std::cout << "\n";
                }
            } else if (is_any_a<std::string>(any)) {
                if (prefix)
                    std::cout << "(string) ";
                std::cout << boost::any_cast<std::string>(any);
            } else if (is_any_a<std::map<std::string, boost::any>>(any)) {
                std::cout << "(map: string, any)"
                          << "\n";
                std::map<std::string, boost::any> l =
                    boost::any_cast<std::map<std::string, boost::any>>(any);
                for (auto ll : l) {
                    for (int i = 0; i < level + 2; i++)
                        std::cout << " ";
                    std::cout << "- [";
                    outputDataStructure(ll.first, 0, false);
                    std::cout << "]: ";
                    outputDataStructure(ll.second, level + 2, false);
                    std::cout << "\n";
                }
            } else if (is_any_a<unsigned int>(any)) {
                std::cout << "(unsigned int) ";
                std::cout << boost::any_cast<unsigned int>(any);
            } else if (is_any_a<long long>(any)) {
                std::cout << "(long long) ";
                std::cout << boost::any_cast<long long>(any);
            } else if (is_any_a<bool>(any)) {
                std::cout << "(bool) ";
                if (boost::any_cast<bool>(any))
                    std::cout << "TRUE";
                else
                    std::cout << "FALSE";

            } else {

                std::cout
                    << "??? ("
                    << boost::typeindex::type_index(any.type()).pretty_name()
                    << ")";
            }
        }
    } // namespace utils
} // namespace rencodeplus
