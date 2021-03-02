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
Helper class to avoid dynamic memory allocation of split functions
maxSplits indicates how many splits to perform at most resulting in at most maxSplits + 1 elements
*/

template<class StringOrStringView, size_t maxSplits>
struct SplitResult;


/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* The string will be splitted at most maxSplits times
* Empty Substrings are kept.
*
*   split("1|23|456|7",   "|")  => ["1", "23", "456", "7"]
*   split("1|23|456|7",   '|')  => ["1", "23", "456", "7"]
*   split("1|23||456|7", "||")  => ["1|23", "456|7"]
*   split("12||34",       "|")  => ["12", "", "34"]
*   split("12|34|",       "|")  => ["12", "34", ""]
*   split("|12|34",       "|")  => ["", "12", "34"]
*   split("1234",         "|")  => ["1234"]
*   split("",              ? )  => [""]
*   split("1234",          "")  => ["","1", "2", "3", "4"]
*   split("1|2|3|4|5", "|", 2)  => ["1", "2", "3|4|5"]
*   split("1|2|3|4|5", "|", 0)  => ["1|2|3|4|5"]
*   split("1||3|4|5",  "|", 2)  => ["1", "", "3|4|5"]
*   split("1||3|4|5",  "|", 20) => ["1", "", "3", "4", "5"]
*/

[[nodiscard]] inline std::vector<std::string_view> split(std::string_view source, std::string_view separator);

[[nodiscard]] inline std::vector<std::string_view> split(std::string_view source, std::string_view separator, size_t maxSplits);

[[nodiscard]] inline std::vector<std::string_view> split(std::string_view source, char separator);

[[nodiscard]] inline std::vector<std::string_view> split(std::string_view source, char separator, size_t maxSplits);


template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> split(std::string_view source, std::string_view separator);

template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> split(std::string_view source, char separator);


/*template<size_t maxSplits>
[[nodiscard]] constexpr inline SplitResult<maxSplits> split(std::string_view source, char separator);*/


/**
* Splits source into substrings wherever any of the separators occurs, and returns a list containing the substrings.
* If none of the separators is found in source a single-element list containing source is returned.
* The string will be splitted at most maxSplits times
* Empty Substrings are kept.
*
*   splitAnyOf("1|23#45",     "|#") => ["1", "23", "45"]
*   splitAnyOf("1|#23|45",    "|#") => ["1", "", "23", "45"]
*   splitAnyOf("1|23#45",       "") => ["1|23#45"]
*   splitAnyOf("1|23#45",      "#") => ["1|23", "45]
*   splitAnyOf("12345",       "|#") => ["12345"]
*   splitAnyOf("",              ? ) => [""]
*   splitAnyOf("1|23#4|5", "|#", 2) => ["1", "23", "4|5"]
*/
[[nodiscard]] inline std::vector<std::string_view> splitAnyOf(std::string_view source, std::string_view separators);

[[nodiscard]] inline std::vector<std::string_view> splitAnyOf(std::string_view source, std::string_view separators, size_t maxSplits);

template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> splitAnyOf(std::string_view source, std::string_view separators);


/**
* Splits source into substrings wherever any not of the separators occurs, and returns a list containing the substrings.
* The string will be splitted at most maxSplits times
* Empty Substrings are kept.
*
*   splitAnyBut("1|2#45",     "|#") => ["", "|", "#", "", ""]
*   splitAnyBut("1|#",        "|#") => ["", "|#"]
*   splitAnyBut("123",          "") => ["", "", "", ""]
*   splitAnyBut("12345", "1234567") => ["12345"]
*   splitAnyBut("",             ? ) => [""]
*   splitAnyBut("|1#2|#", "123", 2) => ["", "1", "2|#"]
*/

[[nodiscard]] inline std::vector<std::string_view> splitAnyBut(std::string_view source, std::string_view separators);

[[nodiscard]] inline std::vector<std::string_view> splitAnyBut(std::string_view source, std::string_view separators, size_t maxSplits);

template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> splitAnyBut(std::string_view source, std::string_view separators);

/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* The string will be splitted at most maxSplits times
* Empty Substrings are discarded.
*
*   splitSkipEmpty("1|23|456|7",   "|")     => ["1", "23", "456", "7"]
*   splitSkipEmpty("1|23|456|7",   '|')     => ["1", "23", "456", "7"]
*   splitSkipEmpty("1|23||456|7", "||")     => ["1|23", "456|7"]
*   splitSkipEmpty("12||34",       "|")     => ["12", "34"]
*   splitSkipEmpty("12|34|",       "|")     => ["12", "34"]
*   splitSkipEmpty("|12|34",       "|")     => ["12", "34"]
*   splitSkipEmpty("1234",         "|")     => ["1234"]
*   splitSkipEmpty("",              ? )     => []
*   splitSkipEmpty("1234",         "" )     => ["1", "2", "3", "4"]
*   splitSkipEmpty("1|2|3|4|5",  "|", 2)    => ["1", "2", "3|4|5"]
*   splitSkipEmpty("1|2|3|4|5",  "|", 0)    => ["1|2|3|4|5"]
*   splitSkipEmpty("1||3|4|5",   "|", 2)    => ["1", "3|4|5"]
*
*/

[[nodiscard]] inline std::vector<std::string_view> splitSkipEmpty(std::string_view source, std::string_view separator);

[[nodiscard]] inline std::vector<std::string_view> splitSkipEmpty(std::string_view source, std::string_view separator, size_t maxSplits);

[[nodiscard]] inline std::vector<std::string_view> splitSkipEmpty(std::string_view source, char separator);

[[nodiscard]] inline std::vector<std::string_view> splitSkipEmpty(std::string_view source, char separator, size_t maxSplits);

template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> splitSkipEmpty(std::string_view source, std::string_view separator);

template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> splitSkipEmpty(std::string_view source, char separator);


/**
* Splits source into substrings wherever any of the separators occurs, and returns a list containing the substrings.
* If none of the separators is found in source a single-element list containing source is returned.
* The string will be splitted at most maxSplits times
* Empty Substrings are discarded.
*
*   splitAnyOfSkipEmpty("1|23#45",     "|#") => ["1", "23", "45"]
*   splitAnyOfSkipEmpty("1|#23|45",    "|#") => ["1", "23", "45"]
*   splitAnyOfSkipEmpty("1|23#45",       "") => ["1|23#45"]
*   splitAnyOfSkipEmpty("1|23#45",      "#") => ["1|23", "45]
*   splitAnyOfSkipEmpty("12345",       "|#") => ["12345"]
*   splitAnyOfSkipEmpty("",              ? ) => []
*   splitAnyOfSkipEmpty("1|23#4|5", "|#", 2) => ["1", "23", "4|5"]
*/
[[nodiscard]] inline std::vector<std::string_view> splitAnyOfSkipEmpty(std::string_view source, std::string_view separators);

[[nodiscard]] inline std::vector<std::string_view> splitAnyOfSkipEmpty(std::string_view source, std::string_view separators, size_t maxSplits);

template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> splitAnyOfSkipEmpty(std::string_view source, std::string_view separators);
/**
* Splits source into substrings wherever any not of the separators occurs, and returns a list containing the substrings.
* The string will be splitted at most maxSplits times
* Empty Substrings are discarded.
*
*   splitAnyButSkipEmpty("1|2#45",     "|#") => ["|", "#"]
*   splitAnyButSkipEmpty("1|#",        "|#") => ["|#"]
*   splitAnyButSkipEmpty("123",          "") => []
*   splitAnyButSkipEmpty("12345", "1234567") => ["12345"]
*   splitAnyButSkipEmpty("",             ? ) => []
*   splitAnyButSkipEmpty("|1#2|#", "123", 2) => ["1", "2|#"]
*/

[[nodiscard]] inline std::vector<std::string_view> splitAnyButSkipEmpty(std::string_view source, std::string_view separators);

[[nodiscard]] inline std::vector<std::string_view> splitAnyButSkipEmpty(std::string_view source, std::string_view separators, size_t maxSplits);

template<size_t maxSplits>
[[nodiscard]] inline SplitResult<std::string_view, maxSplits> splitAnyButSkipEmpty(std::string_view source, std::string_view separators);
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

[[nodiscard]] constexpr inline std::string_view stripEnd(std::string_view str, std::string_view stripChars) noexcept;


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
[[nodiscard]] constexpr inline std::string_view stripStart(std::string_view str, std::string_view stripChars) noexcept;


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
[[nodiscard]] constexpr inline std::string_view strip(std::string_view str, std::string_view stripChars) noexcept;


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

[[nodiscard]] constexpr inline std::string_view iStripEnd(std::string_view str, std::string_view stripChars) noexcept;


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
[[nodiscard]] constexpr inline std::string_view iStripStart(std::string_view str, std::string_view stripChars) noexcept;


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
[[nodiscard]] constexpr inline std::string_view iStrip(std::string_view str, std::string_view stripChars) noexcept;


} // namespace STRINGUTILS_NAMESPACE

#include "StringUtils.ipp"
#endif // !STRINGUTILS_HPP
