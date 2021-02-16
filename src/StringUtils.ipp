#ifndef STRINGUTILS_IPP
#define STRINGUTILS_IPP


#include "StringUtilsPrivate.hpp"
namespace STRINGUTILS_NAMESPACE {



template<typename Delimiter, typename... Args>
[[nodiscard]] static inline std::string join(const Delimiter& delimiter, Args&&... args)
{
    return Detail::join(delimiter, std::forward<Args>(args)...);
}


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


template<typename... Args>
[[nodiscard]] static inline std::string concat(Args&&... args)
{
    return Detail::concat(std::forward<Args>(args)...);
}


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


[[nodiscard]] inline std::vector<std::string> splitKeepEmptyParts(std::string_view source, std::string_view separator)
{
    return Detail::splitKeepEmptyParts(source, separator);
}


[[nodiscard]] inline std::vector<std::string> splitKeepEmptyParts(std::string_view source, char separator)
{
    return Detail::splitKeepEmptyParts(source, separator);
}


[[nodiscard]] inline std::vector<std::string> splitSkipEmptyParts(std::string_view source, std::string_view separator)
{
    return Detail::splitSkipEmptyParts(source, separator);
}


[[nodiscard]] inline std::vector<std::string> splitSkipEmptyParts(std::string_view source, char separator)
{
    return Detail::splitSkipEmptyParts(source, separator);
}


//#######################################################################################
//
//                                  Find
//
//#######################################################################################

[[nodiscard]] constexpr size_t find(const std::string_view hayStack, const std::string_view needle, const size_t startIndex) noexcept
{
    return hayStack.find(needle, startIndex);
}

[[nodiscard]] constexpr size_t find(const std::string_view hayStack, const char needle, const size_t startIndex) noexcept
{
    return hayStack.find(needle, startIndex);
}

[[nodiscard]] constexpr size_t findAnyOf(const std::string_view hayStack, const std::string_view needles, const size_t startIndex) noexcept
{
    return hayStack.find_first_of(needles, startIndex);
}

[[nodiscard]] constexpr size_t findAnyBut(const std::string_view hayStack, const std::string_view needles, const size_t startIndex) noexcept
{
    return hayStack.find_first_not_of(needles, startIndex);
}

//#######################################################################################
//
//                                  Find case insensitive
//
//#######################################################################################

[[nodiscard]] constexpr size_t iFind(const std::string_view hayStack, const std::string_view needle, const size_t startIndex) noexcept
{
    return Detail::iFind(hayStack.data(), hayStack.size(), startIndex, needle.data(), needle.size());
}

[[nodiscard]] constexpr size_t iFind(const std::string_view hayStack, const char needle, size_t startIndex) noexcept
{
    return Detail::iFind(hayStack.data(), hayStack.size(), startIndex, needle);
}

[[nodiscard]] constexpr size_t iFindAnyOf(const std::string_view hayStack, const std::string_view needles, const size_t startIndex) noexcept
{
    return Detail::iFindAnyOf(hayStack.data(), hayStack.size(), startIndex, needles.data(), needles.size());
}

[[nodiscard]] constexpr size_t iFindAnyBut(const std::string_view hayStack, const std::string_view needles, const size_t startIndex) noexcept
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
    return !iStartsWithAnyOf(source, prefixes);
}


//#######################################################################################
//
//                                      substrings
//
//#######################################################################################


[[nodiscard]] constexpr std::string_view leftView(const std::string_view str, const size_t n) noexcept
{
    return str.substr(0, n);
}


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


[[nodiscard]] constexpr inline std::string_view stripEnd(std::string_view str, std::string_view stripChars)
{
    const size_t end = str.find_last_not_of(stripChars) + 1;
    str.remove_suffix(str.size() - end);
    return str;
}


[[nodiscard]] constexpr inline std::string_view stripStart(std::string_view str, std::string_view stripChars)
{
    const size_t start = str.find_first_not_of(stripChars);
    str.remove_prefix(std::min(str.size(), start));
    return str;
}


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


[[nodiscard]] constexpr inline std::string_view iStripEnd(std::string_view str, std::string_view stripChars)
{
    const size_t end = Detail::irFindAnyBut(str.data(), str.size(), 0, stripChars.data(), stripChars.size()) + 1;
    return str.substr(0, end);
}


[[nodiscard]] constexpr inline std::string_view iStripStart(std::string_view str, std::string_view stripChars)
{
    const size_t start = Detail::iFindAnyBut(str.data(), str.size(), 0, stripChars.data(), stripChars.size());
    str.remove_prefix(std::min(str.size(), start));
    return str;
}


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
