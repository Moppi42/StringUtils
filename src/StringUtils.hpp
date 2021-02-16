#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP
#include "StringUtilsPrivate.hpp"
namespace STRINGUTILS_NAMESPACE {


constexpr auto asciiLowerCase = "abcdefghijklmnopqrstuvwxyz";
constexpr auto asciiUpperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr auto asciiLetters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr auto whitespace = " \t\n\r\v\f";
constexpr auto digits = "0123456789";
constexpr auto hexDigits = "0123456789abcdefABCDEF";
constexpr auto octDigits = "01234567";
constexpr auto punctuation = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
constexpr auto printable = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ \t\n\r\v\f!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";


/**
* Joins the strings passed in as parameters into a single string separated by the delimiter.
* If you don't want a delimiter prefer StringUtils::concat over join with an empty separator.
*   join('|', "abc")                            => "abc"
*   join('|')                                   => ""
*   join('|', "", "")                           => "|"
*   join('|', "abc", "def", 'x')                => "abc|def|x"
*   join("| ", 'x', 'y', 'z')                   => "x| y| z"
*   join('|', "abc", "def", "ghij")             => "abc|def|ghij"
*   join("abc", "def", "ghij")                  => "defabcghij"
*   join("||\000||", "abc", "def")              => "abc||def"
*   join('|', "abc", "def\000ghi")              => "abc|def"
*   join('\0', "abc", "def")                    => "abc\0def"
*   join('|', "abc", '\0', "def")               => "abc|\0|def"
*   join('|', std::make_optional("abc"), "def") => "abc|def"
*   join('|', "abc", std::nullopt, "def")       => "abc||def"
*   join('|', std::nullopt, std::nullopt)       => "||"
*/
template<typename Delimiter, typename... Args>
[[nodiscard]] static inline std::string join(const Delimiter& delimiter, Args&&... args);


/**
* Joins the strings passed in as parameters into a single string separated by the delimiter.
* Discards any optionals that have no value, resulting in no separator
* If no optionals are present the behaviour is the same as StringUtils::join
+   joinOptional('|', "abc")                            => "abc"
*   joinOptional('|')                                   => ""
*   joinOptional('|', "", "")                           => "|"
*   joinOptional('|', "abc", "def", 'x')                => "abc|def|x"
*   joinOptional("| ", 'x', 'y', 'z')                   => "x| y| z"
*   joinOptional('|', "abc", "def", "ghij")             => "abc|def|ghij"
*   joinOptional("abc", "def", "ghij")                  => "defabcghij"
*   joinOptional("||\000||", "abc", "def")              => "abc||def"
*   joinOptional('|', "abc", "def\000ghi")              => "abc|def"
*   joinOptional('\0', "abc", "def")                    => "abc\0def"
*   joinOptional('|', "abc", '\0', "def")               => "abc|\0|def"
*   joinOptional('|', std::make_optional("abc"), "def") => "abc|def"
*   joinOptional('|', "abc", std::nullopt, "def")       => "abc|def"
*   joinOptional('|', std::nullopt, std::nullopt)       => ""
*/
template<typename Delimiter, typename... Args>
[[nodiscard]] static inline std::string joinOptional(const Delimiter& delimiter, Args&&... args);

/**
* Concatenates the strings passed in as parameters into a single string. Accepts std::string, std::string_view, const char* and char as parameters.
*   concat("abc", "def")                        => "abcdef"
*   concat()                                    => ""
*   concat("abc")                               => "abc"
*   concat('x')                                 => "x"
*   concat("abc", 'X', "deF")                   => "abcXdeF"
*   concat("", "")                              => ""
*   concat("123", "")                           => "123"
*   concat('x', 'y', 'z')                       => "xyz"
*   concat(std::make_optional("abc"), "def")    => "abcdef"
*   concat("abc", std::nullopt, "def")          => "abcdef"
*   concat(std::nullopt, std::nullopt)          => ""
*/
template<typename... Args>
[[nodiscard]] static inline std::string concat(Args&&... args);


/**
* Checks, whether two strings are equal
*    equals("abc", "abc")   => true
*    equals("Abc", "abc")   => false
*    equals("abc", "ab")    => false
*    equals("", "")         => true
*    equals("abc", "abd")   => false
*    equals("Abc", "")      => false
*    equals("A", 'A')       => true
*    equals('A', "A")       => true
*    equals("a", 'A')       => false
*    equals('a', "A")       => false
*    equals('x', "")        => false
*    equals("", 'x')        => false
*    equals('X', 'X')       => true
*    equals('A', 'a')       => false
*/
[[nodiscard]] constexpr bool equals(const std::string_view str1, const std::string_view str2) noexcept;

[[nodiscard]] constexpr bool equals(const char c1, const std::string_view str2) noexcept;

[[nodiscard]] constexpr bool equals(const std::string_view str1, const char c2) noexcept;

[[nodiscard]] constexpr bool equals(const char c1, const char c2) noexcept;


/**
* Checks, whether two strings are equal ignoring case
*    iEquals("abc", "abc")  => true
*    iEquals("Abc", "abc")  => true
*    iEquals("ABC", "aBc")  => true
*    iEquals("Abc", "ab")   => false
*    iEquals("", "")        => true
*    iEquals("Abc", "Ab")   => false
*    iEquals("Abc", "ABd")  => false
*    iEquals("Abc", "")     => false
*    iEquals("A", 'A')      => true
*    iEquals('A', "A")      => true
*    iEquals('A', 'A')      => true
*    iEquals("a", 'A')      => true
*    iEquals('a', "A")      => true
*    iEquals('A', 'a')      => true
*    iEquals('x', "")       => false
*    iEquals("", 'x')       => false

*/
[[nodiscard]] constexpr bool iEquals(const std::string_view str1, const std::string_view str2) noexcept;


[[nodiscard]] constexpr bool iEquals(const char c1, const std::string_view str2) noexcept;


[[nodiscard]] constexpr bool iEquals(const std::string_view str1, const char c2) noexcept;


[[nodiscard]] constexpr bool iEquals(const char c1, const char c2) noexcept;


/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* Empty Substrings are kept.
*
*   splitKeepEmptyParts("1|23|456|7", "|")      => ["1", "23", "456", "7"]
*   splitKeepEmptyParts("1|23||456|7", "||")    => ["1|23", "456|7"]
*   splitKeepEmptyParts("12||34",     "|")      => ["12", "", "34"]
*   splitKeepEmptyParts("12|34|",     "|")      => ["12", "34", ""]
*   splitKeepEmptyParts("|12|34",     "|")      => ["", "12", "34"]
*   splitKeepEmptyParts("1234",       "|")      => ["1234"]
*   splitKeepEmptyParts("",            ? )      => [""]
*   splitKeepEmptyParts("1234",       "")       => ["","1", "2", "3", "4"]
*/

[[nodiscard]] inline std::vector<std::string> splitKeepEmptyParts(std::string_view source, std::string_view separator);

/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* Empty Substrings are kept.
*
*   splitKeepEmptyParts("1|23|456|7", '|')  => ["1", "23", "456", "7"]
*   splitKeepEmptyParts("12||34",     '|')  => ["12", "", "34"]
*   splitKeepEmptyParts("12|34|",     '|')  => ["12", "34", ""]
*   splitKeepEmptyParts("|12|34",     '|')  => ["", "12", "34"]
*   splitKeepEmptyParts("1234",       '|')  => ["1234"]
*   splitKeepEmptyParts("",            ? )  => [""]
*/

[[nodiscard]] inline std::vector<std::string> splitKeepEmptyParts(std::string_view source, char separator);

/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* Empty Substrings are discarded.
*
*   splitSkipEmptyParts("1|23|456|7",   "|")    => ["1", "23", "456", "7"]
*   splitSkipEmptyParts("1|23||456|7", "||")    => ["1|23", "456|7"]
*   splitSkipEmptyParts("12||34",       "|")    => ["12", "34"]
*   splitSkipEmptyParts("12|34|",       "|")    => ["12", "34"]
*   splitSkipEmptyParts("|12|34",       "|")    => ["12", "34"]
*   splitSkipEmptyParts("1234",         "|")    => ["1234"]
*   splitSkipEmptyParts("",              ? )    => []
*   splitSkipEmptyParts("1234",         "" )    => ["1", "2", "3", "4"]
*
*/

[[nodiscard]] inline std::vector<std::string> splitSkipEmptyParts(std::string_view source, std::string_view separator);

/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* Empty Substrings are discarded.
* 
*   splitSkipEmptyParts("1|23|456|7", '|')  => ["1", "23", "456", "7"]
*   splitSkipEmptyParts("12||34",     '|')  => ["12", "34"]
*   splitSkipEmptyParts("12|34|",     '|')  => ["12", "34"]
*   splitSkipEmptyParts("|12|34",     '|')  => ["12", "34"]
*   splitSkipEmptyParts("1234",       '|')  => ["1234"]
*   splitSkipEmptyParts("",            ? )  => []
*
*/

[[nodiscard]] inline std::vector<std::string> splitSkipEmptyParts(std::string_view source, char separator);


//#######################################################################################
//
//                                  Find
//
//#######################################################################################

[[nodiscard]] constexpr size_t find(const std::string_view hayStack, const std::string_view needle, const size_t startIndex = 0) noexcept;

[[nodiscard]] constexpr size_t find(const std::string_view hayStack, const char needle, const size_t startIndex = 0) noexcept;

[[nodiscard]] constexpr size_t findAnyOf(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept;

[[nodiscard]] constexpr size_t findAnyBut(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept;

//#######################################################################################
//
//                                  Find case insensitive
//
//#######################################################################################

[[nodiscard]] constexpr size_t iFind(const std::string_view hayStack, const std::string_view needle, const size_t startIndex = 0) noexcept;

[[nodiscard]] constexpr size_t iFind(const std::string_view hayStack, const char needle, size_t startIndex = 0) noexcept;

[[nodiscard]] constexpr size_t iFindAnyOf(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept;

[[nodiscard]] constexpr size_t iFindAnyBut(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept;


//#######################################################################################
//
//                                  Contains
//
//#######################################################################################


[[nodiscard]] constexpr bool contains(const std::string_view hayStack, const std::string_view needle) noexcept;

[[nodiscard]] constexpr bool containsAnyOf(const std::string_view hayStack, const std::string_view needles) noexcept;

[[nodiscard]] constexpr bool containsNoneOf(const std::string_view hayStack, const std::string_view needles) noexcept;

[[nodiscard]] constexpr bool containsOnly(const std::string_view hayStack, const std::string_view needles) noexcept;

//#######################################################################################
//
//                               Contains case insensitive
//
//#######################################################################################


[[nodiscard]] constexpr bool iContains(const std::string_view hayStack, const std::string_view needle) noexcept;

[[nodiscard]] constexpr bool iContainsAnyOf(const std::string_view hayStack, const std::string_view needles) noexcept;

[[nodiscard]] constexpr bool iContainsNoneOf(const std::string_view hayStack, const std::string_view needles) noexcept;

[[nodiscard]] constexpr bool iContainsOnly(const std::string_view hayStack, const std::string_view needles) noexcept;

//#######################################################################################
//
//                                      EndsWith
//
//#######################################################################################


[[nodiscard]] constexpr bool endsWith(const std::string_view source, const std::string_view suffix) noexcept;

[[nodiscard]] constexpr bool endsWithAnyOf(const std::string_view source, const std::string_view suffixes) noexcept;

[[nodiscard]] constexpr bool endsWithNoneOf(std::string_view source, std::string_view suffixes) noexcept;


//#######################################################################################
//
//                                      EndsWith case insensitive
//
//#######################################################################################


[[nodiscard]] constexpr bool iEndsWith(std::string_view source, std::string_view suffix) noexcept;

[[nodiscard]] constexpr bool iEndsWithAnyOf(std::string_view source, std::string_view suffixes) noexcept;

[[nodiscard]] constexpr bool iEndsWithNoneOf(std::string_view source, std::string_view suffixes) noexcept;


//#######################################################################################
//
//                                      StartsWith
//
//#######################################################################################


[[nodiscard]] constexpr bool startsWith(std::string_view source, std::string_view prefix) noexcept;

[[nodiscard]] constexpr bool startsWithAnyOf(std::string_view source, std::string_view prefixes) noexcept;

[[nodiscard]] constexpr bool startsWithNoneOf(std::string_view source, std::string_view prefixes) noexcept;


//#######################################################################################
//
//                                      StartsWith case insensitive
//
//#######################################################################################

[[nodiscard]] constexpr bool iStartsWith(std::string_view source, std::string_view prefix) noexcept;

[[nodiscard]] constexpr bool iStartsWithAnyOf(std::string_view source, std::string_view prefixes) noexcept;

[[nodiscard]] constexpr bool iStartsWithNoneOf(std::string_view source, std::string_view prefixes) noexcept;


//#######################################################################################
//
//                                      substrings
//
//#######################################################################################


/**
* Returns the leftmost n characters of str
*   leftView("123456789", 2)        => "12"
*   leftView("1", 2)                => "1"
*   leftView("", ?)                 => ""
*   leftView("123456789", 0)        => ""
*   leftView("1234567890", 11)      => "1234567890"
*/
[[nodiscard]] constexpr std::string_view leftView(const std::string_view str, const size_t n) noexcept;


/**
* Returns the rightmost n characters of str
*   rightView("123456789", 2)        => "89"
*   rightView("1", 2)                => "1"
*   rightView("", ?)                 => ""
*   rightView("123456789", 0)        => ""
*   rightView("1234567890", 11)      => "1234567890"
*/
[[nodiscard]] constexpr std::string_view rightView(std::string_view str, const size_t n) noexcept;


//#######################################################################################
//
//                                      toString
//
//#######################################################################################

template<typename T>
[[nodiscard]] inline std::string toString(const T& value);


//#######################################################################################
//
//                                      strip
//
//#######################################################################################


/**
* Strips any of a set of characters from the end of a string
*   stripEnd("abc",     "")           => "abc"
*   stripEnd("abc",     "x")          => "abc"
*   stripEnd("abc",     "bc")         => "a"
*   stripEnd("",         ?   )        => ""
*   stripEnd("abc",     "cba")        => ""
*   stripEnd("Abc",     "cba")        => "A"
*   stripEnd("abcxabc", "bac")        => "abcx"
*   stripEnd("abcxabc", "bacdefghi")  => "abcx"
*/

[[nodiscard]] constexpr inline std::string_view stripEnd(std::string_view str, std::string_view stripChars);


/**
* Strips any of a set of characters from the start of a string
*   stripStart("abc",     "")           => "abc"
*   stripStart("abc",     "x")          => "abc"
*   stripStart("abc",     "ab")         => "c"
*   stripStart("",         ?   )        => ""
*   stripStart("abc",     "cba")        => ""
*   stripStart("abC",     "cba")        => "C"
*   stripStart("abcxabc", "bac")        => "xabc"
*   stripStart("abcxabc", "bacdefghi")  => "xabc"
*/
[[nodiscard]] constexpr inline std::string_view stripStart(std::string_view str, std::string_view stripChars);


/**
* Strips any of a set of characters from the start and end of a string
*   strip("abc",     "")           => "abc"
*   strip("abc",     "x")          => "abc"
*   strip("abc",     "ab")         => "c"
*   strip("",         ?   )        => ""
*   strip("abc",     "cba")        => ""
*   strip("aBc",     "cba")        => "B"
*   strip("abcxabc", "bac")        => "x"
*   strip("abcxabc", "bacdefghi")  => "x"
*   strip("abcxax",  "bacdefghi")  => "xax"
*/
[[nodiscard]] constexpr inline std::string_view strip(std::string_view str, std::string_view stripChars);


//#######################################################################################
//
//                                strip case insensitive
//
//#######################################################################################


/**
* Strips any of a set of characters from the end of a string
*   iStripEnd("abc",     "")           => "abc"
*   iStripEnd("abc",     "x")          => "abc"
*   iStripEnd("abc",     "bc")         => "a"
*   iStripEnd("",         ?   )        => ""
*   iStripEnd("abc",     "cba")        => ""
*   iStripEnd("Abc",     "cba")        => ""
*   iStripEnd("abcxaBc", "bac")        => "abcx"
*   iStripEnd("abcxaBc", "bacdefghi")  => "abcx"
*/

[[nodiscard]] constexpr inline std::string_view iStripEnd(std::string_view str, std::string_view stripChars);


/**
* Strips any of a set of characters from the start of a string
*   stripStart("abc",     "")           => "abc"
*   stripStart("abc",     "x")          => "abc"
*   stripStart("abc",     "ab")         => "c"
*   stripStart("",         ?   )        => ""
*   stripStart("abc",     "cba")        => ""
*   stripStart("abC",     "cba")        => ""
*   stripStart("aBcxabc", "bac")        => "xabc"
*   stripStart("aBcxabc", "bacdefghi")  => "xabc"
*/
[[nodiscard]] constexpr inline std::string_view iStripStart(std::string_view str, std::string_view stripChars);


/**
* Strips any of a set of characters from the start and end of a string
*   strip("abc",     "")           => "abc"
*   strip("abc",     "x")          => "abc"
*   strip("abc",     "ab")         => "c"
*   strip("",         ?   )        => ""
*   strip("abc",     "cba")        => ""
*   strip("aBc",     "cba")        => ""
*   strip("aBcxabc", "bac")        => "x"
*   strip("aBcxabC", "bacdefghi")  => "x"
*   strip("aBcxax",  "bacdefghi")  => "xax"
*/
[[nodiscard]] constexpr inline std::string_view iStrip(std::string_view str, std::string_view stripChars);


} // namespace STRINGUTILS_NAMESPACE

#include "StringUtils.ipp"
#endif // !STRINGUTILS_HPP
