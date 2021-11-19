#ifndef _RENCODEPLUSCPP
#define _RENCODEPLUSCPP
#include <boost/any.hpp>
#include <list>
#include <map>
#include <string>
#include <climits>
#include <memory>
namespace rencodeplus {
namespace utils {}
namespace encode {}
namespace decode {}
namespace consts {}
} // namespace rencodeplus

using namespace rencodeplus::encode;
typedef std::basic_string<unsigned char> string;

void encode_specific(string data, string &encodedStr);
void encode_specific(std::string data, string &encodedStr);
void encode_specific(std::map<std::string, boost::any> data,
                     string &encodedStr);
void encode_specific(std::list<boost::any> data, string &encodedStr);
void encode_specific(bool data, string &encodedStr);
void encode_specific(int data, string &encodedStr);

template <typename T> bool encode_any(boost::any data, string &encodedStr);
bool encode_try_all(boost::any data, string &encodedStr);
void encode(boost::any data, string &encodedStr);

using namespace rencodeplus::decode;
bool decode_fixed_int(string &inputStr, boost::any &outObject);
bool decode_fixed_neg_int(string &inputStr, boost::any &outObject);
bool decode_fixed_string(string &inputStr, boost::any &outObject);
bool decode_fixed_list(string &inputStr, boost::any &outObject);
bool decode_fixed_dict(string &inputStr, boost::any &outObject);
bool decode_list(string &inputStr, boost::any &outObject);
bool decode_string(string &inputStr, boost::any &outObject);
bool decode_int_1(string &inputStr, boost::any &outObject);
bool decode_int_2(string &inputStr, boost::any &outObject);
bool decode_int_4(string &inputStr, boost::any &outObject);
bool decode_dict(string &inputStr, boost::any &outObject);
bool decode(string &inputStr, boost::any &outObject);

using namespace rencodeplus::consts;
const unsigned char DEFAULT_FLOAT_BITS = 32;
// Maximum length of integer when written as base 10 string.
const unsigned char MAX_INT_LENGTH = 64;
// The bencode 'typecodes' such as i, d, etc have been extended and
// relocated on the base-256 character set.
const unsigned char CHR_LIST = 59;
const unsigned char CHR_DICT = 60;
const unsigned char CHR_INT = 61;
const unsigned char CHR_INT1 = 62;
const unsigned char CHR_INT2 = 63;
const unsigned char CHR_INT4 = 64;
const unsigned char CHR_INT8 = 65;
const unsigned char CHR_FLOAT32 = 66;
const unsigned char CHR_FLOAT64 = 44;
const unsigned char CHR_TRUE = 67;
const unsigned char CHR_FALSE = 68;
const unsigned char CHR_NONE = 69;
const unsigned char CHR_TERM = 127;
// Positive integers with value embedded in typecode.
const unsigned char INT_POS_FIXED_START = 0;
const unsigned char INT_POS_FIXED_COUNT = 44;
// Dictionaries with length embedded in typecode.
const unsigned char DICT_FIXED_START = 102;
const unsigned char DICT_FIXED_COUNT = 25;
// Negative integers with value embedded in typecode.
const unsigned char INT_NEG_FIXED_START = 70;
const unsigned char INT_NEG_FIXED_COUNT = 32;
// Strings with length embedded in typecode.
const unsigned char STR_FIXED_START = 128;
const unsigned char STR_FIXED_COUNT = 64;
// Lists with length embedded in typecode.
const unsigned char LIST_FIXED_START = STR_FIXED_START + STR_FIXED_COUNT;
const unsigned char LIST_FIXED_COUNT = 64;

const int MAX_SIGNED_INT = INT_MAX;
const int MIN_SIGNED_INT = -INT_MAX;
// note: negating the Python value avoids compiler problems
//(negating the "long long" constant can make it unsigned with some compilers!)
const long long MAX_SIGNED_LONGLONG = LONG_LONG_MAX;
const long long MIN_SIGNED_LONGLONG = -LONG_LONG_MAX;

using namespace rencodeplus::utils;

template <typename T> std::string type_name() {
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void (*)(void *)> own(
#ifndef _MSC_VER
        abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#else
        nullptr,
#endif
        std::free);
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

template <typename T> bool is_any_a(boost::any a) {
    try {
        T i = boost::any_cast<T>(a);
    } catch (boost::bad_any_cast e) {
#ifdef DEBUG_IS_ANY_A
        std::cout << "error, no " << type_name<T>() << "\n";
#endif
        return false;
    }
    return true;
}

void outputDataStructure(boost::any any, int level, bool prefix = true);
int writeToFile(std::string fileName, string content);
void outputStrInHex(string str);
unsigned char *readBinaryData(std::string filename, int &size);

#endif