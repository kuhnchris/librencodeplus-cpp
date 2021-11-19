#include "./librencodeplus.hpp"
#include <boost/any.hpp>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <string>
#include <typeinfo>

using namespace rencodeplus::consts;

namespace rencodeplus {
    namespace encode {
        void encode_specific(string data, string &encodedStr) {

#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << ">>Encoding string...\n";
#endif
            if (data.length() < STR_FIXED_COUNT) {
                unsigned char c = STR_FIXED_START + data.length();
                encodedStr = encodedStr + c + data;
            } else {
                unsigned char c = data.length();
                encodedStr = encodedStr + c + (unsigned char)':' + data;
            }
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << "<<Encoded string...\n";
#endif
        }

        void encode_specific(int data, string &encodedStr) {

#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << ">>Encoding int...\n";
#endif
            if (data < INT_POS_FIXED_COUNT) {
                encodedStr = encodedStr + static_cast<unsigned char>(
                                              INT_POS_FIXED_START + data);
            } else {
                // unsigned char c = data.length();
                encodedStr = encodedStr + static_cast<unsigned char>(CHR_INT1) +
                             static_cast<unsigned char>(data);
            }
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << "<<Encoded int...\n";
#endif
        }

        void encode_specific(bool data, string &encodedStr) {
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << ">>Encoding bool...\n";
#endif
            if (data) {
                encodedStr = encodedStr + static_cast<unsigned char>(CHR_TRUE);
            } else {
                encodedStr = encodedStr + static_cast<unsigned char>(CHR_FALSE);
            }
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << "<<Encoded bool...\n";
#endif
        }

        void encode_specific(std::string data, string &encodedStr) {

#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << ">>Encoding string (std)...\n";
#endif
            string d;
            for (char c : data) {
                d = d + static_cast<unsigned char>(c);
            }
            if (data.length() < STR_FIXED_COUNT) {
                unsigned char c = STR_FIXED_START + data.length();
                encodedStr = encodedStr + c + d;
            } else {
                unsigned char c = data.length();
                encodedStr = encodedStr + c + (unsigned char)':' + d;
            }
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << "<<Encoded string (std)... " << data << "\n";
#endif
        }

        void encode_specific(std::map<std::string, boost::any> data,
                             string &encodedStr) {
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << ">>encoding map...\n";
#endif
            string strAdd;
            for (auto &p : data) {
#ifdef DEBUG
                std::cout << strAdd.size() << " - "
                          << "attempting to convert key..."
                          << type_name<typeof(p.first)>() << "\n";
#endif
                encode_try_all(p.first, strAdd);
#ifdef DEBUG
                std::cout << strAdd.size() << " - "
                          << "attempting to convert value..."
                          << type_name<typeof(p.second)>() << "\n";
#endif
                encode_try_all(p.second, strAdd);
            }

            encodedStr = encodedStr + CHR_DICT;
            encodedStr = encodedStr + (strAdd);
            encodedStr = encodedStr + CHR_TERM;

#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << "<<encoded map...\n";
#endif
        }

        void encode_specific(std::list<boost::any> data, string &encodedStr) {
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << ">>encoding list...\n";
#endif
            string strAdd;
            for (auto a = data.begin(); a != data.end(); ++a) {
#ifdef DEBUG
                std::cout << encodedStr.size() << " - "
                          << "List has entry of type: " << a->type().name()
                          << '\n';
#endif
                encode_try_all(*a, strAdd);
            }
            if (data.size() < LIST_FIXED_COUNT) {
                unsigned char c = LIST_FIXED_START + data.size();
                encodedStr = encodedStr + c;
                encodedStr = encodedStr + strAdd;
            } else {
                encodedStr = encodedStr + CHR_LIST;
                encodedStr = encodedStr + strAdd;
                encodedStr = encodedStr + CHR_TERM;
            }
#ifdef DEBUG
            std::cout << encodedStr.size() << " - "
                      << "<<encoded list...\n";
#endif
        }

        template <typename T>
        bool encode_any(boost::any data, string &encodedStr) {
            if (is_any_a<T>(data)) {
                encode_specific(boost::any_cast<T>(data), encodedStr);
                return true;
            }
            return false;
        }

        bool encode_try_all(boost::any data, string &encodedStr) {
#ifdef DEBUG
            std::cerr << "(rencode) DEBUG: parsing: '"
                      << boost::typeindex::type_index(data.type()).pretty_name()
                      << "'\n";
#endif

            if (encode_any<const char *>(data, encodedStr))
                return true;
            else if (encode_any<char *>(data, encodedStr))
                return true;
            else if (encode_any<const unsigned char *>(data, encodedStr))
                return true;
            else if (encode_any<const std::string>(data, encodedStr))
                return true;
            else if (encode_any<std::map<std::string, boost::any>>(data,
                                                                   encodedStr))
                return true;
            else if (encode_any<std::list<boost::any>>(data, encodedStr))
                return true;
            else if (encode_any<int>(data, encodedStr))
                return true;
            else if (encode_any<bool>(data, encodedStr))
                return true;
            std::cerr << "(rencode) could not convert data of type '"
                      << data.type().name() << "' to anything useful.\n";
            return false;
        }

        void encode(boost::any data, string &encodedStr) {
            encode_try_all(data, encodedStr);
        }
    } // namespace encode
} // namespace rencodeplus
