#ifndef RENCODEPLUSCONSTS
#define RENCODEPLUSCONSTS
// Default number of bits for serialized floats, either 32 or 64 (also a
// parameter for dumps()).
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

const int MAX_SIGNED_INT = (1 << 31);
const int MIN_SIGNED_INT = -MAX_SIGNED_INT;
// note: negating the Python value avoids compiler problems
//(negating the "long long" constant can make it unsigned with some compilers!)
const long long MAX_SIGNED_LONGLONG = ((long long)1 << 63);
const long long MIN_SIGNED_LONGLONG = -MAX_SIGNED_LONGLONG;

#endif