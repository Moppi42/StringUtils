#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP


#include "StringUtilsPrivate.hpp"
namespace STRINGUTILS_NAMESPACE {
enum SplitBehavior
{
    SKIP_EMPTY_PARTS,
    KEEP_EMPTY_PARTS
};

enum CaseSensitivity
{
    CASE_INSENSITIVE,
    CASE_SENSITIVE
};


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
[[nodiscard]] static inline std::string join(const Delimiter& delimiter, Args&&... args)
{
    return Detail::join(delimiter, std::forward<Args>(args)...);
}


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
[[nodiscard]] static inline std::string joinOptional(const Delimiter& delimiter, Args&&... args)
{
    if constexpr (std::disjunction_v<Detail::is_optional<Args>...>)
    { // contains any optionals
        return Detail::joinOptional(delimiter, std::forward<Args>(args)...);
    }
    else
    {
        return Detail::join(delimiter, std::forward<Args>(args)...);
    }
}


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
[[nodiscard]] static inline std::string concat(Args&&... args)
{
    return Detail::concat(std::forward<Args>(args)...);
}


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
[[nodiscard]] constexpr bool equals(const std::string_view str1, const std::string_view str2) noexcept
{
    return str1 == str2;
}

[[nodiscard]] constexpr bool equals(const char c1, const std::string_view str2) noexcept
{
    return (str2.size() == 1) && c1 == str2[0];
}

[[nodiscard]] constexpr bool equals(const std::string_view str1, const char c2) noexcept
{
    return (str1.size() == 1) && str1[0] == c2;
}

[[nodiscard]] constexpr bool equals(const char c1, const char c2) noexcept
{
    return c1 == c2;
}


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
[[nodiscard]] constexpr bool iEquals(const std::string_view str1, const std::string_view str2) noexcept
{
    const size_t size = str1.size();
    return (size == str2.size() && Detail::iEquals(str1.data(), str2.data(), size));
}

[[nodiscard]] constexpr bool iEquals(const char c1, const std::string_view str2) noexcept
{
    return (str2.size() == 1) && Detail::charEqualsIgnoreCase(c1, str2[0]);
}

[[nodiscard]] constexpr bool iEquals(const std::string_view str1, const char c2) noexcept
{
    return (str1.size() == 1) && Detail::charEqualsIgnoreCase(str1[0], c2);
}

[[nodiscard]] constexpr bool iEquals(const char c1, const char c2) noexcept
{
    return Detail::charEqualsIgnoreCase(c1, c2);
}


/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* If splitBehavior is SplitBehavior::SKIP_EMPTY_PARTS empty substrings are discarded. By default splitBehavior is set to SplitBehavior::KEEP_EMPTY_PARTS
*
*   split("1|23|456|7", "|", SplitBehavior::KEEP_EMPTY_PARTS)   => ["1", "23", "456", "7"]
*   split("12||34",     "|", SplitBehavior::KEEP_EMPTY_PARTS)   => ["12", "", "34"]
*   split("12||34",     "|", SplitBehavior::SKIP_EMPTY_PARTS)   => ["12", "34"]
*   split("12|34|",     "|", SplitBehavior::KEEP_EMPTY_PARTS)   => ["12", "34", ""]
*   split("12|34|",     "|", SplitBehavior::SKIP_EMPTY_PARTS)   => ["12", "34"]
*   split("|12|34",     "|", SplitBehavior::KEEP_EMPTY_PARTS)   => ["", "12", "34"]
*   split("|12|34",     "|", SplitBehavior::SKIP_EMPTY_PARTS)   => ["12", "34"]
*   split("1234",       "|", ?                              )   => ["1234"]
*   split("",            ? , SplitBehavior::KEEP_EMPTY_PARTS)   => [""]
*   split("",            ? , SplitBehavior::SKIP_EMPTY_PARTS)   => []
*   split("1234",       "" , SplitBehavior::KEEP_EMPTY_PARTS)   => ["","1", "2", "3", "4"]
*   split("1234",       "" , SplitBehavior::SKIP_EMPTY_PARTS)   => ["1", "2", "3", "4"]
*
*/
[[nodiscard]] inline std::vector<std::string> split(std::string_view source, std::string_view separator, const StringUtils::SplitBehavior splitBehavior = SplitBehavior::KEEP_EMPTY_PARTS)
{
    if (splitBehavior == SplitBehavior::KEEP_EMPTY_PARTS)
    {
        return Detail::splitKeepEmptyParts(source, separator);
    }
    else
    {
        return Detail::splitSkipEmptyParts(source, separator);
    }
}

/**
* Splits source into substrings wherever the separator occurs, and returns a list containing the substrings.
* If separator is not found in source a single-element list containing source is returned.
* If splitBehavior is SplitBehavior::SKIP_EMPTY_PARTS empty substrings are discarded. By default splitBehavior is set to SplitBehavior::KEEP_EMPTY_PARTS
*
*   split("1|23|456|7", '|', SplitBehavior::KEEP_EMPTY_PARTS)   => ["1", "23", "456", "7"]
*   split("12||34",     '|', SplitBehavior::KEEP_EMPTY_PARTS)   => ["12", "", "34"]
*   split("12||34",     '|', SplitBehavior::SKIP_EMPTY_PARTS)   => ["12", "34"]
*   split("12|34|",     '|', SplitBehavior::KEEP_EMPTY_PARTS)   => ["12", "34", ""]
*   split("12|34|",     '|', SplitBehavior::SKIP_EMPTY_PARTS)   => ["12", "34"]
*   split("|12|34",     '|', SplitBehavior::KEEP_EMPTY_PARTS)   => ["", "12", "34"]
*   split("|12|34",     '|', SplitBehavior::SKIP_EMPTY_PARTS)   => ["12", "34"]
*   split("1234",       '|', ?                              )   => ["1234"]
*   split("",            ? , SplitBehavior::KEEP_EMPTY_PARTS)   => [""]
*   split("",            ? , SplitBehavior::SKIP_EMPTY_PARTS)   => []
*
*/

[[nodiscard]] inline std::vector<std::string> split(std::string_view source, char separator, const StringUtils::SplitBehavior splitBehavior)
{
    if (splitBehavior == SplitBehavior::KEEP_EMPTY_PARTS)
    {
        return Detail::splitKeepEmptyParts(source, separator);
    }
    else
    {
        return Detail::splitSkipEmptyParts(source, separator);
    }
}


//#######################################################################################
//
//                                  Find
//
//#######################################################################################

[[nodiscard]] constexpr size_t find(const std::string_view hayStack, const std::string_view needle, const size_t startIndex = 0) noexcept
{
    return hayStack.find(needle, startIndex);
}

[[nodiscard]] constexpr size_t find(const std::string_view hayStack, const char needle, const size_t startIndex = 0) noexcept
{
    return hayStack.find(needle, startIndex);
}

[[nodiscard]] constexpr size_t findAnyOf(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept
{
    return hayStack.find_first_of(needles, startIndex);
}

[[nodiscard]] constexpr size_t findAnyBut(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept
{
    return hayStack.find_first_not_of(needles, startIndex);
}

//#######################################################################################
//
//                                  Find case insensitive
//
//#######################################################################################

[[nodiscard]] constexpr size_t iFind(const std::string_view hayStack, const std::string_view needle, const size_t startIndex = 0) noexcept
{
    return Detail::iFind(hayStack.data(), hayStack.size(), startIndex, needle.data(), needle.size());
}

[[nodiscard]] constexpr size_t iFind(const std::string_view hayStack, const char needle, size_t startIndex = 0) noexcept
{
    return Detail::iFind(hayStack.data(), hayStack.size(), startIndex, needle);
}

[[nodiscard]] constexpr size_t iFindAnyOf(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept
{
    return Detail::iFindAnyOf(hayStack.data(), hayStack.size(), startIndex, needles.data(), needles.size());
}

[[nodiscard]] constexpr size_t iFindAnyBut(const std::string_view hayStack, const std::string_view needles, const size_t startIndex = 0) noexcept
{
    return Detail::iFindAnyBut(hayStack.data(), hayStack.size(), startIndex, needles.data(), needles.size());
}


//#######################################################################################
//
//                                  Contains
//
//#######################################################################################


[[nodiscard]] constexpr bool contains(const std::string_view hayStack, const std::string_view needle) noexcept
{
    return STRINGUTILS_NAMESPACE::find(hayStack, needle, 0) != INDEX_NOT_FOUND;
}

[[nodiscard]] constexpr bool containsAnyOf(const std::string_view hayStack, const std::string_view needles) noexcept
{
    return findAnyOf(hayStack, needles, 0) != INDEX_NOT_FOUND;
}

[[nodiscard]] constexpr bool containsNoneOf(const std::string_view hayStack, const std::string_view needles) noexcept
{
    return findAnyOf(hayStack, needles, 0) == INDEX_NOT_FOUND;
}

[[nodiscard]] constexpr bool containsOnly(const std::string_view hayStack, const std::string_view needles) noexcept
{
    return findAnyBut(hayStack, needles, 0) == INDEX_NOT_FOUND;
}

//#######################################################################################
//
//                               Contains case insensitive
//
//#######################################################################################


[[nodiscard]] constexpr bool iContains(const std::string_view hayStack, const std::string_view needle) noexcept
{
    return iFind(hayStack, needle, 0) != INDEX_NOT_FOUND;
}

[[nodiscard]] constexpr bool iContainsAnyOf(const std::string_view hayStack, const std::string_view needles) noexcept
{
    return iFindAnyOf(hayStack, needles, 0) != INDEX_NOT_FOUND;
}

[[nodiscard]] constexpr bool iContainsNoneOf(const std::string_view hayStack, const std::string_view needles) noexcept
{
    return iFindAnyOf(hayStack, needles, 0) == INDEX_NOT_FOUND;
}

[[nodiscard]] constexpr bool iContainsOnly(const std::string_view hayStack, const std::string_view needles) noexcept
{
    return iFindAnyBut(hayStack, needles, 0) == INDEX_NOT_FOUND;
}

//#######################################################################################
//
//                                      EndsWith
//
//#######################################################################################


[[nodiscard]] constexpr bool endsWith(const std::string_view source, const std::string_view suffix) noexcept
{
    const size_t sourceLength = source.length();
    const size_t suffixLength = suffix.length();
    return sourceLength >= suffixLength && Detail::equals(suffix.data(), source.data() + (sourceLength - suffixLength), suffixLength);
}

[[nodiscard]] constexpr bool endsWithAnyOf(const std::string_view source, const std::string_view suffixes) noexcept
{
    const size_t sourceLength = source.length();
    return sourceLength >= 1 && (Detail::findChar(suffixes.data(), suffixes.size(), source[sourceLength - 1]) != nullptr);
}

[[nodiscard]] constexpr bool endsWithNoneOf(std::string_view source, std::string_view suffixes) noexcept
{
    return !endsWithAnyOf(source, suffixes);
}


//#######################################################################################
//
//                                      EndsWith case insensitive
//
//#######################################################################################


[[nodiscard]] constexpr bool iEndsWith(std::string_view source, std::string_view suffix) noexcept
{
    const size_t sourceLength = source.length();
    const size_t suffixLength = suffix.length();
    return sourceLength >= suffixLength && Detail::iEquals(suffix.data(), source.data() + (sourceLength - suffixLength), suffixLength);
}

[[nodiscard]] constexpr bool iEndsWithAnyOf(std::string_view source, std::string_view suffixes) noexcept
{
    const size_t sourceLength = source.length();
    return sourceLength >= 1 && (Detail::iFindChar(suffixes.data(), suffixes.size(), source[sourceLength - 1]) != nullptr);
}

[[nodiscard]] constexpr bool iEndsWithNoneOf(std::string_view source, std::string_view suffixes) noexcept
{
    return !iEndsWithAnyOf(source, suffixes);
}


//#######################################################################################
//
//                                      StartsWith
//
//#######################################################################################


[[nodiscard]] constexpr bool startsWith(std::string_view source, std::string_view prefix) noexcept
{
    const size_t sourceLength = source.length();
    const size_t prefixLength = prefix.length();
    return sourceLength >= prefixLength && Detail::equals(prefix.data(), source.data(), prefixLength);
}

[[nodiscard]] constexpr bool startsWithAnyOf(std::string_view source, std::string_view prefixes) noexcept
{
    const size_t sourceLength = source.length();
    return sourceLength >= 1 && (Detail::findChar(prefixes.data(), prefixes.size(), source[0]) != nullptr);
}

[[nodiscard]] constexpr bool startsWithNoneOf(std::string_view source, std::string_view prefixes) noexcept
{
    return !startsWithAnyOf(source, prefixes);
}


//#######################################################################################
//
//                                      StartsWith case insensitive
//
//#######################################################################################

[[nodiscard]] constexpr bool iStartsWith(std::string_view source, std::string_view prefix) noexcept
{
    const size_t sourceLength = source.length();
    const size_t prefixLength = prefix.length();
    return sourceLength >= prefixLength && Detail::iEquals(prefix.data(), source.data(), prefixLength);
}

[[nodiscard]] constexpr bool iStartsWithAnyOf(std::string_view source, std::string_view prefixes) noexcept
{
    const size_t sourceLength = source.length();
    return sourceLength >= 1 && (Detail::iFindChar(prefixes.data(), prefixes.size(), source[0]) != nullptr);
}

[[nodiscard]] constexpr bool iStartsWithNoneOf(std::string_view source, std::string_view prefixes) noexcept
{
    return !iStartsWithNoneOf(source, prefixes);
}


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
[[nodiscard]] constexpr std::string_view leftView(const std::string_view str, const size_t n) noexcept
{
    return str.substr(0, n);
}


/**
* Returns the rightmost n characters of str
*   rightView("123456789", 2)        => "89"
*   rightView("1", 2)                => "1"
*   rightView("", ?)                 => ""
*   rightView("123456789", 0)        => ""
*   rightView("1234567890", 11)      => "1234567890"
*/
[[nodiscard]] constexpr std::string_view rightView(std::string_view str, const size_t n) noexcept
{
    const size_t strLength = str.length();
    const size_t length = std::min(n, strLength);
    str.remove_prefix(strLength - length);
    return str;
}


//#######################################################################################
//
//                                      toString
//
//#######################################################################################

template<typename T>
[[nodiscard]] inline std::string toString(const T& value)
{
    return Detail::toString(value);
}


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

[[nodiscard]] constexpr inline std::string_view stripEnd(std::string_view str, std::string_view stripChars)
{
    const size_t end = str.find_last_not_of(stripChars) + 1;
    str.remove_suffix(str.size() - end);
    return str;
}


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
[[nodiscard]] constexpr inline std::string_view stripStart(std::string_view str, std::string_view stripChars)
{
    const size_t start = str.find_first_not_of(stripChars);
    str.remove_prefix(std::min(str.size(), start));
    return str;
}


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
[[nodiscard]] constexpr inline std::string_view strip(std::string_view str, std::string_view stripChars)
{
    const size_t start = str.find_first_not_of(stripChars);
    str.remove_prefix(std::min(str.size(), start));
    const size_t end = str.find_last_not_of(stripChars) + 1;
    str.remove_suffix(str.size() - end);
    return str;
}


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

[[nodiscard]] constexpr inline std::string_view iStripEnd(std::string_view str, std::string_view stripChars)
{
    const size_t end = Detail::irFindAnyBut(str.data(), str.size(), 0, stripChars.data(), stripChars.size()) + 1;
    return str.substr(0, end);
}


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
[[nodiscard]] constexpr inline std::string_view iStripStart(std::string_view str, std::string_view stripChars)
{
    const size_t start = Detail::iFindAnyBut(str.data(), str.size(), 0, stripChars.data(), stripChars.size());
    str.remove_prefix(std::min(str.size(), start));
    return str;
}


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
[[nodiscard]] constexpr inline std::string_view iStrip(std::string_view str, std::string_view stripChars)
{
    const size_t start = Detail::iFindAnyBut(str.data(), str.size(), 0, stripChars.data(), stripChars.size());
    str.remove_prefix(std::min(str.size(), start));
    
    const size_t end = Detail::irFindAnyBut(str.data(), str.size(), 0, stripChars.data(), stripChars.size()) + 1;
    str.remove_suffix(str.size() - end);
    
    return str;
}


} // namespace STRINGUTILS_NAMESPACE


#endif // !STRINGUTILS_HPP
