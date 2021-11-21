#include "./librencodeplus.hpp"
#include <boost/any.hpp>
#include <boost/type_index.hpp>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <string>

using namespace rencodeplus::consts;

#ifdef DEBUG
int currLevel = 0;
#endif

namespace rencodeplus {
    namespace decode {
        using namespace rencodeplus::decode;
        bool decode_fixed_int(string &inputStr, boost::any &outObject) {
            try {
                int len = inputStr[0] - INT_POS_FIXED_START;
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracting fixed int..."
                          << "\n";
#endif
                unsigned int f = len;
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
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
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracting fixed neg int..."
                          << "\n";
#endif
                int f = len;
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
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
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracting int (1 byte)..."
                          << "\n";
#endif
                unsigned int f = inputStr[1];
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracted: " << f << "\n";
#endif
                outObject = f;
                inputStr = string(&inputStr[2], inputStr.size() - 2);
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
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracting int (2 byte)..."
                          << "\n";
#endif
                long long f = (inputStr[2] << 8) + (inputStr[3]);
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracted: " << f << "\n";
#endif
                outObject = f;
                inputStr = string(&inputStr[3], inputStr.size() - 3);
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
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracting int (4 byte)..."
                          << "\n";
#endif
                long long f = (inputStr[1] << 24) + (inputStr[2] << 16) +
                              (inputStr[3] << 8) + (inputStr[4]);
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracted: " << f << "\n";
#endif
                outObject = f;
                inputStr = string(&inputStr[5], inputStr.size() - 5);
            } catch (boost::bad_any_cast c) {
                std::cout << "Extracting int4 from rencode failed."
                          << "\n";
                return false;
            }
            return true;
        }
        bool decode_big_number(string &inputStr, boost::any &outObject) {
            const int bigNumberSize = 18;
            inputStr = string(&inputStr[1], inputStr.size() - 1);
            // HOW TO HANDLE THAT.
            // TODO: Impelment 18-byte number in C++...
            inputStr = string(&inputStr[bigNumberSize],
                              inputStr.size() - bigNumberSize);
            while (inputStr[0] != CHR_TERM)
                inputStr = string(&inputStr[1], inputStr.size() - 1);

            inputStr = string(&inputStr[1], inputStr.size() - 1);
            return true;
        }

        bool decode_fixed_string(string &inputStr, boost::any &outObject) {
            try {
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracting fixed string..."
                          << "\n";
#endif
                int len = inputStr[0] - STR_FIXED_START;
                if (len > 0) {
                    std::string f = reinterpret_cast<const char *>(
                        inputStr.substr(1, len).c_str());
                    outObject = f;
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "Extracted: " << f << "\n";
#endif
                } else {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "String length 0."
                              << "\n";
#endif
                }
                inputStr =
                    string(&inputStr[len + 1], inputStr.size() - len - 1);
            } catch (boost::bad_any_cast c) {
                std::cout << "Extracting fixed string from rencode failed."
                          << "\n";
                outObject = "";
                return false;
            }
            return true;
        }

        bool decode_string(string &inputStr, boost::any &outObject) {
            return false;
        }

        bool decode_fixed_list(string &inputStr, boost::any &outObject) {
            int listLen = inputStr[0] - LIST_FIXED_START;
            inputStr = string(&inputStr[1], inputStr.size() - 1);
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
            inputStr = string(&inputStr[1], inputStr.size() - 1);
            boost::any innerObjK;
            boost::any innerObjV;
            std::map<std::string, boost::any> l =
                std::map<std::string, boost::any>();
            while (inputStr[0] != CHR_TERM) {
                try {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
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
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "Converting for dictionary..."
                              << "\n";
#endif

                    std::string kStr;
                    try {
                        kStr = treatNumbersAsString(innerObjK);
                    } catch (boost::bad_any_cast e) {

                        try {
                            kStr = boost::any_cast<std::string>(innerObjK);
                        } catch (boost::bad_any_cast e) {

                            std::cout << "Could not convert key. Expected: "
                                         "std::string/char*, got: "
                                      << boost::typeindex::type_index(
                                             innerObjK.type())
                                             .pretty_name()
                                      << "\n"
                                      << "Error: " << e.what() << "\n";
                            outObject = l;
                            return false;
                        }
                    }
                    try {
#ifdef DEBUG
                        for (int zz = 0; zz < currLevel; zz++) {
                            std::cout << "\t";
                        }
                        std::cout << "adding to dictionary..."
                                  << "\n";
#endif
                        l.insert(std::pair<std::string, boost::any>(kStr,
                                                                    innerObjV));
                    } catch (boost::bad_any_cast e) {

                        std::cout
                            << "Could not convert value. Expected: boost::any, "
                               "got: "
                            << boost::typeindex::type_index(innerObjK.type())
                                   .pretty_name()
                            << "\n"
                            << "Error: " << e.what() << "\n";
                        outObject = l;
                        return false;
                    }
                    innerObjK = nullptr;
                    innerObjV = nullptr;
                } catch (boost::bad_any_cast e) {

                    std::cout << "Additional Dictionary error. "
                              << "\n"
                              << "Error: " << e.what() << "\n";
                    outObject = l;
                    return false;
                }
            }
            if (inputStr.size() > 0)
                inputStr = string(&inputStr[1], inputStr.size() - 1);
            outObject = l;
            return true;
        }

        bool decode_fixed_dict(string &inputStr, boost::any &outObject) {
            int cnt = inputStr[0] - DICT_FIXED_START;
            inputStr = string(&inputStr[1], inputStr.size() - 1);

            boost::any innerObjK;
            boost::any innerObjV;
            std::map<std::string, boost::any> l =
                std::map<std::string, boost::any>();
            for (int i = 0; i < cnt; i++) {
                try {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
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
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "Adding to fixed dictionary..."
                              << "\n";
#endif
                    std::string kStr = boost::any_cast<std::string>(innerObjK);
                    try {
                        kStr = treatNumbersAsString(innerObjK);
                    } catch (boost::bad_any_cast e) {

                        try {
                            kStr = boost::any_cast<std::string>(innerObjK);
                        } catch (boost::bad_any_cast e) {

                            std::cout << "Could not convert key. Expected: "
                                         "std::string/char*, got: "
                                      << boost::typeindex::type_index(
                                             innerObjK.type())
                                             .pretty_name()
                                      << "\n"
                                      << "Error: " << e.what() << "\n";

                            outputDataStructure(innerObjK, 0);
                            std::cout << "\n";
                            outObject = l;
                            return false;
                        }
                    }

                    l.insert(
                        std::pair<std::string, boost::any>(kStr, innerObjV));
                    innerObjK = nullptr;
                    innerObjV = nullptr;
                } catch (boost::bad_any_cast e) {

                    std::cout << "Could not convert key. Expected: "
                                 "std::string/char*, got: "
                              << boost::typeindex::type_index(innerObjK.type())
                                     .pretty_name()
                              << "\n"
                              << "Error: " << e.what() << "\n";

                    outputDataStructure(innerObjK, 0);
                    std::cout << "\n";
                    outObject = l;
                    return false;
                }
            }
            outObject = l;
            return true;
        }

        bool decode_list(string &inputStr, boost::any &outObject) {
            inputStr = string(&inputStr[1], inputStr.size() - 1);
            boost::any innerObj;
            std::list<boost::any> l = std::list<boost::any>();
            while (inputStr[0] != CHR_TERM) {
                if (!decode(inputStr, innerObj)) {
                    outObject = l;
                    // return false;
                } else {
                    l.push_back(innerObj);
                }
                innerObj = nullptr;
            }
            inputStr = string(&inputStr[1], inputStr.size() - 1);
            outObject = l;
            return true;
        }

        bool decode_weird_number_string(string &inputStr,
                                        boost::any &outObject) {
            try {
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracting fixed string..."
                          << "\n";
#endif
                std::string newOut = "";

                while (inputStr[0] != ':') {
                    std::string f = reinterpret_cast<const char *>(
                        inputStr.substr(0, 1).c_str());
                    newOut = newOut + f;
                    inputStr = string(&inputStr[1], inputStr.size() - 1);
                }
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "Extracted: " << newOut << "\n";
#endif
                inputStr = string(&inputStr[1], inputStr.size() - 1);

                int nLen = stoi(newOut);
                outObject = string(inputStr, nLen);
                inputStr = string(&inputStr[nLen], inputStr.size() - nLen);
            } catch (boost::bad_any_cast c) {
                std::cout << "Extracting fixed string from rencode failed."
                          << "\n";
                outObject = "";
                return false;
            }
            return true;
        }

        bool decode(string &inputStr, boost::any &outObject) {
#ifdef DEBUG
            currLevel++;
            for (int zz = 0; zz < currLevel; zz++) {
                std::cout << "\t";
            }
            std::cout << "inputStr still has a size() of: " << inputStr.size()
                      << "\n";
#endif
            switch (inputStr[0]) {
            case CHR_TRUE:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found const 'true'"
                          << "\n";
#endif
                outObject = true;
                inputStr = string(&inputStr[1], inputStr.size() - 1);
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            case CHR_FALSE:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found const 'false'"
                          << "\n";
#endif
                outObject = false;
                inputStr = string(&inputStr[1], inputStr.size() - 1);
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            case CHR_INT1:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found a int1 header char"
                          << "\n";
#endif
                if (!decode_int_1(inputStr, outObject)) {
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return false;
                }
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            case CHR_INT2:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found a int2 header char"
                          << "\n";
#endif
                if (!decode_int_2(inputStr, outObject)) {
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return false;
                }
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            case CHR_INT4:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found a int4 header char"
                          << "\n";
#endif
                if (!decode_int_4(inputStr, outObject)) {
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return false;
                }
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            case CHR_LIST:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found a list header char"
                          << "\n";
#endif
                if (!decode_list(inputStr, outObject)) {
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return false;
                }
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            case CHR_DICT:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found a dictionary header char"
                          << "\n";
#endif
                if (!decode_dict(inputStr, outObject)) {
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return false;
                }
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            case CHR_INT:
#ifdef DEBUG
                for (int zz = 0; zz < currLevel; zz++) {
                    std::cout << "\t";
                }
                std::cout << "found a int (big one)"
                          << "\n";
#endif
                if (!decode_big_number(inputStr, outObject)) {
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return false;
                }
#ifdef DEBUG
                currLevel--;
                std::cout << "<< return"
                          << "Here: " << __LINE__ << "\n";
#endif
                return true;
            default:

                if (DICT_FIXED_START <= inputStr[0] &&
                    inputStr[0] <= (DICT_FIXED_START + DICT_FIXED_COUNT)) {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "found a fixed +int"
                              << "\n";
#endif
                    if (!decode_fixed_dict(inputStr, outObject)) {
#ifdef DEBUG
                        currLevel--;
                        std::cout << "<< return"
                                  << "Here: " << __LINE__ << "\n";
#endif
                        return false;
                    }
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return true;
                }

                if (INT_POS_FIXED_START <= inputStr[0] &&
                    inputStr[0] <=
                        (INT_POS_FIXED_START + INT_POS_FIXED_COUNT)) {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "found a fixed +int"
                              << "\n";
#endif
                    if (!decode_fixed_int(inputStr, outObject)) {
#ifdef DEBUG
                        currLevel--;
                        std::cout << "<< return"
                                  << "Here: " << __LINE__ << "\n";
#endif
                        return false;
                    }
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return true;
                }
                if (INT_NEG_FIXED_START <= inputStr[0] &&
                    inputStr[0] <=
                        (INT_NEG_FIXED_START + INT_NEG_FIXED_COUNT)) {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "found a fixed -int"
                              << "\n";
#endif
                    if (!decode_fixed_neg_int(inputStr, outObject)) {
#ifdef DEBUG
                        currLevel--;
                        std::cout << "<< return"
                                  << "Here: " << __LINE__ << "\n";
#endif
                        return false;
                    }
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return true;
                }

                if (STR_FIXED_START <= inputStr[0] &&
                    inputStr[0] < (STR_FIXED_START + STR_FIXED_COUNT)) {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "found a fixed string"
                              << "\n";
#endif
                    if (!decode_fixed_string(inputStr, outObject)) {
#ifdef DEBUG
                        currLevel--;
                        std::cout << "<< return"
                                  << "Here: " << __LINE__ << "\n";
#endif
                        return false;
                    }
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return true;
                } else if (LIST_FIXED_START <= inputStr[0] &&
                           inputStr[0] <=
                               (LIST_FIXED_START + LIST_FIXED_COUNT - 1)) {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "found a fixed list"
                              << "\n";
#endif
                    if (!decode_fixed_list(inputStr, outObject)) {
#ifdef DEBUG
                        currLevel--;
                        std::cout << "<< return"
                                  << "Here: " << __LINE__ << "\n";
#endif
                        return false;
                    }
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return true;
                } else if (48 <= inputStr[0] && inputStr[0] <= 57) {
#ifdef DEBUG
                    for (int zz = 0; zz < currLevel; zz++) {
                        std::cout << "\t";
                    }
                    std::cout << "found this weird string thing..."
                              << "\n";
#endif
                    if (!decode_weird_number_string(inputStr, outObject)) {
#ifdef DEBUG
                        currLevel--;
                        std::cout << "<< return"
                                  << "Here: " << __LINE__ << "\n";
#endif
                        return false;
                    }
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return true;
                } else {
                    std::cout << "Could not interpret first character '"
                              << inputStr[0] << "(0x" << std::hex
                              << static_cast<unsigned int>(inputStr[0]) << ","
                              << std::dec
                              << static_cast<unsigned int>(inputStr[0]) << ")"
                              << "' as anything useful. :-(\n";

                    inputStr = string(&inputStr[1], inputStr.size() - 1);
#ifdef DEBUG
                    currLevel--;
                    std::cout << "<< return"
                              << "Here: " << __LINE__ << "\n";
#endif
                    return false;
                }
            }
#ifdef DEBUG
            currLevel--;
            std::cout << "<< return"
                      << "Here: " << __LINE__ << "\n";
#endif
            return false;
        }
    } // namespace decode
} // namespace rencodeplus
