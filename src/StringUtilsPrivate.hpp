#ifndef STRINGUTILS_PRIVATE_HPP
#define STRINGUTILS_PRIVATE_HPP


#if defined(__clang__)
#    define STRINGUTILS_CLANG_COMPILER
#elif defined(__GNUC__) || defined(__GNUG__)
#    define STRINGUTILS_GNU_COMPILER
#elif defined(_MSC_VER)
#    define STRINGUTILS_MSVC_COMPILER
#else
#    pragma message("Detected none of Gcc, Clang or Msvc: hopefully it works")
#endif

#if !defined(STRINGUTILS_HAS_CXX17) && !defined(STRINGUTILS_HAS_CXX14) && !defined(STRINGUTILS_HAS_CXX11)
#    if defined(STRINGUTILS_MSVC_COMPILER)
#        define STRINGUTILS_STL_LANG _MSVC_LANG
#    else
#        define STRINGUTILS_STL_LANG __cplusplus
#    endif

#    if STRINGUTILS_STL_LANG >= 202002L
#        define STRINGUTILS_HAS_CXX20 1
#        define STRINGUTILS_HAS_CXX17 1
#        define STRINGUTILS_HAS_CXX14 1
#        define STRINGUTILS_HAS_CXX11 1
#    elif STRINGUTILS_STL_LANG >= 201703L
#        define STRINGUTILS_HAS_CXX20 0
#        define STRINGUTILS_HAS_CXX17 1
#        define STRINGUTILS_HAS_CXX14 1
#        define STRINGUTILS_HAS_CXX11 1
#    elif STRINGUTILS_STL_LANG >= 201402L
#        define STRINGUTILS_HAS_CXX20 0
#        define STRINGUTILS_HAS_CXX17 0
#        define STRINGUTILS_HAS_CXX14 1
#        define STRINGUTILS_HAS_CXX11 1
#    elif STRINGUTILS_STL_LANG >= 201103L
#        define STRINGUTILS_HAS_CXX20 0
#        define STRINGUTILS_HAS_CXX17 0
#        define STRINGUTILS_HAS_CXX14 0
#        define STRINGUTILS_HAS_CXX11 1
#    else // _STL_LANG <= 201103L
#        error "StringUtils require at least C++11"
#    endif

#    undef STRINGUTILS_STL_LANG

#    define STRING2(x) #    x
#    define STRING(x) STRING2(x)


#    ifdef STRINGUTILS_CLANG_COMPILER
#        pragma message("Detected Clang " STRING(__clang_major__) "." STRING(__clang_minor__) "." STRING(__clang_patchlevel__))
static_assert(__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 9), "Clang below 3.9.0 not supported");
#    endif

#    ifdef STRINGUTILS_GNU_COMPILER
#        pragma message("Detected GCC " STRING(__GNUC__) "." STRING(__GNUC_MINOR__) "." STRING(__GNUC_PATCHLEVEL__))
static_assert(__GNUC__ >= 7 && __GNUC_MINOR__ >= 1, "GCC below 7.1 not supported");
#    endif

#    ifdef STRINGUTILS_MSVC_COMPILER
#        pragma message("Detected MSVC " STRING(_MSC_VER))
static_assert(_MSC_VER >= 1915, "MSVC is not supported for versions below 19.15");
#    endif


#    undef STRING
#    undef STRING2

#endif


#include <string>
#include <cstring>
#include <type_traits>
#include <array>
#include <vector>


#ifndef STRINGUTILS_NO_STRINGSTREAM
#    include <sstream>
#endif

#if STRINGUTILS_HAS_CXX17
#    include <string_view>
#    include <optional>
#endif


#ifdef CUSTOM_STRINGUTILS_NAMESPACE
#    define STRINGUTILS_NAMESPACE CUSTOM_STRINGUTILS_NAMESPACE
#else
#    define STRINGUTILS_NAMESPACE StringUtils

#endif

using uchar = unsigned char;

namespace STRINGUTILS_NAMESPACE {
static constexpr size_t INDEX_NOT_FOUND{ std::string::npos };
#if STRINGUTILS_HAS_CXX17
static_assert(std::string::npos == std::string_view::npos, "Inconsistent values for std::string::npos and std::string_view::npos");
#endif
} // namespace STRINGUTILS_NAMESPACE


namespace STRINGUTILS_NAMESPACE {
namespace Detail {
/**
* Helper for is_string_convertible
*/
template<typename T>
using is_string_convertible_helper = std::integral_constant<bool,
    std::is_same_v<T, char> || std::is_same_v<T, std::string_view> || std::is_same_v<T, std::string> || std::is_same_v<T, const char*> || std::is_same_v<T, char*> || (std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>)>;

/**
* Allows only std::string, std::string_view, char, const char*, char*, char[], const char[]
*/
template<typename T>
using is_string_convertible = is_string_convertible_helper<std::decay_t<T>>;

template<typename T>
inline constexpr bool is_string_convertible_v = Detail::is_string_convertible<T>::value;

/**
* Helper for is_optional
*/
template<typename T, typename Enable = void>
struct is_optional_helper: std::false_type
{
};

template<typename T>
struct is_optional_helper<std::optional<T>>: std::true_type
{
};

/**
* Determines whether a template parameter is std::optional
*/
template<typename T>
using is_optional = is_optional_helper<std::remove_cv_t<std::remove_reference_t<T>>>;

template<typename T>
inline constexpr bool is_optional_v = Detail::is_optional<T>::value;


/**
* overloaded helper for getting the size of the string given by a template parameter
*/
static inline size_t getStringSize(const std::string& str) { return str.size(); }

/**
* overloaded helper for getting the size of the string given by a template parameter
*/
static inline constexpr size_t getStringSize(const std::string_view& str) { return str.size(); }

/**
* overloaded helper for getting the size of the string given by a template parameter
*/
static inline constexpr size_t getStringSize(const char) { return 1; }

/**
* overloaded helper for getting the size of the string given by a template parameter, char array must be '\0' terminated
*/
static inline constexpr size_t getStringSize(const char* ptr) { return std::char_traits<char>::length(ptr); }

/**
* overloaded helper for getting the size of the string given by a template parameter
* if the optional has no value 0 is returned else the length of the value
*/
template<typename T>
static inline constexpr size_t getStringSize(const std::optional<T>& optionalString)
{
    return (optionalString.has_value()) ? getStringSize(optionalString.value()) : 0;
}


/**
* overloaded helper for creating a std::string from a template parameter (there is no std::string constructor that takes a single char)
*/
static inline std::string makeString(const std::string& str) { return str; }
static inline std::string makeString(std::string&& str) { return std::string(std::move(str)); }
/**
* overloaded helper for creating a std::string from a template parameter (there is no std::string constructor that takes a single char)
*/
static inline std::string makeString(const std::string_view& str) { return std::string(str); }
/**
* overloaded helper for creating a std::string from a template parameter (there is no std::string constructor that takes a single char)
*/
static inline std::string makeString(const char c) { return std::string(1, c); }
/**
* overloaded helper for creating a std::string from a template parameter (there is no std::string constructor that takes a single char), char array must be '\0' terminated
*/
static inline std::string makeString(const char* ptr) { return std::string(ptr); }

/**
* overloaded helper for creating a std::string from a template parameter (there is no std::string constructor that takes a single char)
* Returns an empty string if optional has no value, else it returns a string constructed from the value
*/
template<typename T>
static inline std::string makeString(const std::optional<T>& optional)
{
    return (optional.has_value()) ? makeString(optional.value()) : std::string();
}
template<typename T>
static inline std::string makeString(std::optional<T>&& optional)
{
    return (optional.has_value()) ? makeString(std::move(optional.value())) : std::string();
}


/**
* overloaded helper for copying a string given as a template parameter into an existing string
*/
static inline void copyToString(const std::string& source, const size_t charCount, char* const destination) { std::memcpy(destination, source.data(), charCount * sizeof(char)); }

/**
* overloaded helper for copying a string given as a template parameter into an existing string
*/
static inline void copyToString(const std::string_view& source, const size_t charCount, char* const destination) { std::memcpy(destination, source.data(), charCount * sizeof(char)); }

/**
* overloaded helper for copying a string given as a template parameter into an existing string
*/
static inline void copyToString(const char source, const size_t charCount, char* const destination) { destination[0] = source; }

/**
* overloaded helper for copying a string given as a template parameter into an existing string
*/
static inline void copyToString(const char* source, const size_t charCount, char* const destination) { std::memcpy(destination, source, charCount * sizeof(char)); }

/**
* overloaded helper for copying a string given as a template parameter into an existing string, does nothing if optional has no value
*/
template<typename T>
static inline void copyToString(const std::optional<T>& optionalSource, const size_t charCount, char* const destination)
{
    if (optionalSource.has_value())
    {
        copyToString(optionalSource.value(), charCount, destination);
    }
}

/**
* Helper for joining with optional parameters (when to set a separator and when not to)
*/
template<typename T, typename Delimiter>
static inline bool joinOptionalCopyHelper(const bool isFirst, const T& source, size_t sourceLength, const Delimiter& delimiter, size_t delimiterSize, char*& copyDestination)
{
    if (isFirst)
    {
        Detail::copyToString(source, sourceLength, copyDestination);
        copyDestination += sourceLength;
    }
    else
    {
        Detail::copyToString(delimiter, delimiterSize, copyDestination),
            Detail::copyToString(source, sourceLength, copyDestination + delimiterSize),
            copyDestination += delimiterSize + sourceLength;
    }
    return false;
}

/**
* Helper for joining with optional parameters (when to set a separator and when not to)
*/
template<typename T, typename Delimiter>
static inline bool joinOptionalCopyHelper(const bool isFirst, const std::optional<T>& source, size_t sourceLength, const Delimiter& delimiter, size_t delimiterSize, char*& copyDestination)
{
    if (source.has_value())
    {
        return Detail::joinOptionalCopyHelper(isFirst, source.value(), sourceLength, delimiter, delimiterSize, copyDestination);
    }
    else
    {
        return isFirst;
    }
}

/**
* Helper for separator counting
*/
template<typename T>
inline constexpr size_t joinOptionalArgumentCountHelper(const T&)
{
    return 1;
}
/**
* Helper for separator counting
*/
template<typename T>
inline constexpr size_t joinOptionalArgumentCountHelper(const std::optional<T>& optional)
{
    return optional.has_value() ? 1 : 0;
}
/**
* Helper for counting how many separators are needed by joinOptional
*/
template<typename Arg0, typename... Args>
inline constexpr size_t joinOptionalArgumentCount(const Arg0& arg0, const Args&... args)
{
    size_t argumentCount = joinOptionalArgumentCountHelper(arg0);
    ((
         argumentCount += Detail::joinOptionalArgumentCountHelper(args)),
        ...);
    return argumentCount;
}


/**
* helper that creates an array containing the string lengths of each parameter of the parameter pack
*/
template<typename... Args>
static constexpr inline auto makeSizesArray(const Args&... args)
{
    constexpr size_t argCount = sizeof...(args);
    const std::array<size_t, argCount> sizes = { { Detail::getStringSize(args)... } };
    return sizes;
}

/**
 * helper that accumulates all lengths in the given array (needed by join to determine the total size) 
 */
template<size_t N>
static constexpr inline size_t totalSizeHelper(const std::array<size_t, N>& array) noexcept
{
    size_t totalSize = 0;
    for (size_t j = 0; j < N; ++j)
    {
        totalSize += array[j];
    }
    return totalSize;
}


constexpr inline char charToUpperCase(char c) noexcept
{
    const uchar c1 = static_cast<uchar>(c) - 'a';
    return (c1 <= 'z' - 'a') ? static_cast<char>(c1 + 'A') : c;
}

constexpr inline char charToLowerCase(const char c) noexcept
{
    const uchar c1 = static_cast<uchar>(c) - 'A';
    return (c1 <= ('Z' - 'A')) ? static_cast<char>(c1 + 'a') : c;
}

constexpr inline bool charEquals(const char c1, const char c2) noexcept
{
    return c1 == c2;
}

constexpr inline bool charEqualsIgnoreCase(const char c1, const char c2) noexcept
{
    return charToLowerCase(c1) == charToLowerCase(c2);
}

constexpr inline int compareCharIgnoreCase(const char c1, const char c2) noexcept
{
    return charToLowerCase(c1) - charToLowerCase(c2);
}


/**
* Wrapper for std::char_traits::find
*/
constexpr inline const char* findChar(const char* const hayStack, const size_t haySize, const char needle) noexcept
{
    return std::char_traits<char>::find(hayStack, haySize, needle);
}

/**
* Searches for character needle within the first count characters of the sequence pointed to by p in reverse order.
* A pointer to the first character in the range specified by [p, p + count) that compares equal to ch,
* or a null pointer if not found.
*/
constexpr inline const char* rFindChar(const char* const hayStack, const size_t haySize, const char needle) noexcept
{
    if (haySize != 0)
    {
        for (const char* position = hayStack + haySize - 1;; --position)
        {
            if (charEquals(*position, needle))
            {
                return position;
            }

            if (position == hayStack)
            {
                break;
            }
        }
    }
    return nullptr;
}


/**
* Searches for character ch case insensitively within the first count characters of the sequence pointed to by p.
* A pointer to the first character in the range specified by [p, p + count) that compares case insensitively equal to ch,
* or a null pointer if not found.
* See also char_traits::find()
*/

constexpr inline const char* iFindChar(const char* hayStack, const size_t haySize, const char needle) noexcept
{
    const char* const end = hayStack + haySize;
    const char lower = charToLowerCase(needle);
    const char upper = charToUpperCase(needle);
    for (; hayStack < end; ++hayStack)
    {
        if (*hayStack == lower || *hayStack == upper)
        {
            return hayStack;
        }
    }
    return nullptr;
}

/**
* Searches for character ch case insensitively within the first count characters of the sequence pointed to by p in reverse order.
* A pointer to the first character in the range specified by [p, p + count) that compares case insensitively equal to ch,
* or a null pointer if not found.
*/

constexpr inline const char* irFindChar(const char* const hayStack, const size_t haySize, const char needle) noexcept
{
    if (haySize != 0)
    {
        const char lower = charToLowerCase(needle);
        const char upper = charToUpperCase(needle);
        for (const char* position = hayStack + haySize - 1;; --position)
        {
            if (*position == lower || *position == upper)
            {
                return position;
            }

            if (position == hayStack)
            {
                break;
            }
        }
    }
    return nullptr;
}


/**
* Wrapper for char_traits::compare == 0
*/
constexpr inline bool equals(const char* const ptr1, const char* const ptr2, size_t size) noexcept
{
    return std::char_traits<char>::compare(ptr1, ptr2, size) == 0;
}


constexpr inline bool iEquals(const char* ptr1, const char* ptr2, size_t size) noexcept
{
    const char* end = ptr1 + size;
    for (; ptr1 != end; ++ptr1, ++ptr2)
    {
        if (!charEqualsIgnoreCase(*ptr1, *ptr2))
        {
            return false;
        }
    }
    return true;
}







//TODO: benchmark 64 byte version (1 bit per char)

class StringMatchHelper
{ // helper for findAny so we do not have O(n) instead of O(n^2)
  private:
    bool m_Marks[256] = {};

  public:
    constexpr void mark(const char c) noexcept
    {
        m_Marks[static_cast<uchar>(c)] = true;
    }

    constexpr void iMark(const char c) noexcept
    {
        mark(charToLowerCase(c));
        mark(charToUpperCase(c));
    }

    constexpr void mark(const char* start, const char* const end) noexcept
    {
        for (; start != end; ++start)
        {
            mark(*start);
        }
    }

    constexpr void iMark(const char* start, const char* const end) noexcept
    {
        for (; start != end; ++start)
        {
            iMark(*start);
        }
    }

    constexpr bool hasMatch(const char c) const noexcept
    {
        return m_Marks[static_cast<uchar>(c)];
    }
    constexpr bool hasMatch(const uchar c) const noexcept
    {
        return m_Marks[c];
    }
};

constexpr inline StringMatchHelper iGenerateStringMatchHelper(const char* const source, const size_t sourceSize) noexcept
{
    StringMatchHelper matchHelper;
    matchHelper.iMark(source, source + sourceSize);
    return matchHelper;
}


constexpr inline StringMatchHelper generateStringMatchHelper(const char* const source, const size_t sourceSize) noexcept
{
    StringMatchHelper matchHelper;
    matchHelper.mark(source, source + sourceSize);
    return matchHelper;
}



constexpr inline size_t find(const char* hayStack, const size_t haySize, const size_t startIndex, char needle) noexcept
{
    if (startIndex < haySize)
    {
        const char* match = findChar(hayStack + startIndex, haySize + 1 - startIndex, needle);
        if (match != nullptr)
        {
            return static_cast<size_t>(match - hayStack);
        }
    }
    return INDEX_NOT_FOUND;
}


constexpr inline size_t find(const char* hayStack, const size_t haySize, const size_t startIndex, const char* needle, const size_t needleSize) noexcept
{
    if (needleSize > haySize || startIndex > haySize - needleSize)
    {
        return INDEX_NOT_FOUND;
    }

    if (needleSize == 0)
    {
        return startIndex;
    }

    const char* const end = hayStack + (haySize + 1 - needleSize);
    for (const char* position = hayStack + startIndex;; ++position)
    {
        position = findChar(position, static_cast<size_t>(end - position), *needle); // Searches for the first character of needle
        if (position == nullptr)
        {
            return INDEX_NOT_FOUND;
        }

        if (equals(position, needle, needleSize)) // found match
        {
            return static_cast<size_t>(position - hayStack);
        }
    }
}

/**
* Implementation of string_view::find_first_of (MSVC implementation)
* Keep around for C++11 compatibility
*/
constexpr size_t findAnyOf(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (needlesSize != 0 && startIndex < haySize)
    {
        const StringMatchHelper matchHelper = generateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + startIndex; position < end; ++position)
        {
            if (matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
        }
    }
    return INDEX_NOT_FOUND;
}


/**
* Implementation of string_view::find_first_not_of (MSVC implementation)
* Keep around for C++11 compatibility
*/
constexpr size_t findAnyBut(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (startIndex < haySize)
    {
        const StringMatchHelper matchHelper = generateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + startIndex; position < end; ++position)
        {
            if (!matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
        }
    }
    return INDEX_NOT_FOUND;
}


constexpr inline size_t rFind(const char* const hayStack, const size_t haySize, const size_t startIndex, const char needle) noexcept
{
    if (startIndex < haySize)
    {
        const char* match = rFindChar(hayStack + startIndex, haySize + 1 - startIndex, needle);
        if (match != nullptr)
        {
            return static_cast<size_t>(match - hayStack);
        }
    }
    return INDEX_NOT_FOUND;
}


constexpr inline size_t rFind(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needle, const size_t needleSize) noexcept
{
    if (needleSize == 0)
    {
        return std::min(startIndex, haySize);
    }

    if (needleSize <= haySize)
    {
        for (const char* position = hayStack + std::min(startIndex, haySize - needleSize);; --position)
        {
            if (charEquals(*position, *needle) && equals(position, needle, needleSize))
            {
                return static_cast<size_t>(position - hayStack);
            }

            if (position == hayStack)
            {
                break;
            }
        }
    }
    return INDEX_NOT_FOUND;
}

/**
* Implementation of string_view::find_last_of (MSVC implementation)
* Keep around for C++11 compatibility
*/
constexpr size_t rFindAnyOf(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (needlesSize != 0 && startIndex < haySize)
    {
        const StringMatchHelper matchHelper = generateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + std::min(startIndex, haySize);; --position)
        {
            if (matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
            if (position == hayStack) {
                break;
            }
        }
    }
    return INDEX_NOT_FOUND;
}


/**
* Implementation of string_view::find_first_not_of (MSVC implementation)
* Keep around for C++11 compatibility
*/
constexpr size_t rFindAnyBut(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (startIndex < haySize)
    {
        const StringMatchHelper matchHelper = generateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + std::min(startIndex, haySize);; --position)
        {
            if (!matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
            if (position == hayStack)
            {
                break;
            }
        }
    }
    return INDEX_NOT_FOUND;
}



constexpr inline size_t iFind(const char* const hayStack, const size_t haySize, const size_t startIndex, const char needle) noexcept
{
    if (startIndex < haySize)
    {
        const char* match = iFindChar(hayStack + startIndex, haySize + 1 - startIndex, needle);
        if (match != nullptr)
        {
            return static_cast<size_t>(match - hayStack);
        }
    }
    return INDEX_NOT_FOUND;
}


constexpr inline size_t iFind(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needle, const size_t needleSize) noexcept
{
    if (needleSize > haySize || startIndex > haySize - needleSize)
    {
        return INDEX_NOT_FOUND;
    }

    if (needleSize == 0)
    {
        return startIndex;
    }

    const char* const end = hayStack + (haySize + 1 - needleSize);
    for (const char* position = hayStack + startIndex;; ++position)
    {
        position = iFindChar(position, static_cast<size_t>(end - position), *needle); // Searches for the first character of needle
        if (position == nullptr)
        {
            return INDEX_NOT_FOUND;
        }

        if (iEquals(position, needle, needleSize)) // found match
        {
            return static_cast<size_t>(position - hayStack);
        }
    }
}

/**
* Finds the first occurrence case insensitively of any of the chars in [needles, needles + needlesSize)
* Uses a lookup array for the needles => O(n) instead of O(n^2)
*/
constexpr size_t iFindAnyOf(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (needlesSize != 0 && startIndex < haySize)
    {
        const StringMatchHelper matchHelper = iGenerateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + startIndex; position < end; ++position)
        {
            if (matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
        }
    }
    return INDEX_NOT_FOUND;
}


/**
* Finds the first occurrence case insensitively of any except the chars in [needles, needles + needlesSize)
* Uses a lookup array for the needles => O(n) instead of O(n^2)
*/
constexpr size_t iFindAnyBut(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (startIndex < haySize)
    {
        const StringMatchHelper matchHelper = iGenerateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + startIndex; position < end; ++position)
        {
            if (!matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
        }
    }
    return INDEX_NOT_FOUND;
}








constexpr inline size_t irFind(const char* const hayStack, const size_t haySize, const size_t startIndex, const char needle) noexcept
{
    if (startIndex < haySize)
    {
        const char* match = irFindChar(hayStack + startIndex, haySize + 1 - startIndex, needle);
        if (match != nullptr)
        {
            return static_cast<size_t>(match - hayStack);
        }
    }
    return INDEX_NOT_FOUND;
}


constexpr inline size_t irFind(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needle, const size_t needleSize) noexcept
{

    if (needleSize == 0)
    {
        return std::min(startIndex, haySize);
    }
    if (needleSize <= haySize)
    {
        for (const char* position = hayStack + std::min(startIndex, haySize - needleSize);; --position)
        {
            if (charEqualsIgnoreCase(*position, *needle) && iEquals(position, needle, needleSize))
            {
                return static_cast<size_t>(position - hayStack);
            }

            if (position == hayStack)
            {
                break;
            }
        }
    }
    return INDEX_NOT_FOUND;
}




/**
* Finds the last occurrence case insensitively of any of the chars in [needles, needles + needlesSize)
* Uses a lookup array for the needles => O(n) instead of O(n^2)
*/
constexpr size_t irFindAnyOf(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (needlesSize != 0 && startIndex < haySize)
    {
        const StringMatchHelper matchHelper = iGenerateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + std::min(startIndex, haySize);; --position)
        {
            if (matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
            if (position == hayStack)
            {
                break;
            }
        }
    }
    return INDEX_NOT_FOUND;
}


/**
* Finds the last occurrence case insensitively of none of the chars in [needles, needles + needlesSize)
* Uses a lookup array for the needles => O(n) instead of O(n^2)
*/
constexpr size_t irFindAnyBut(const char* const hayStack, const size_t haySize, const size_t startIndex, const char* const needles, const size_t needlesSize) noexcept
{
    if (startIndex < haySize)
    {
        const StringMatchHelper matchHelper = iGenerateStringMatchHelper(needles, needlesSize);
        const char* const end = hayStack + haySize;
        for (const char* position = hayStack + std::min(startIndex, haySize);; --position)
        {
            if (!matchHelper.hasMatch(*position))
            {
                return static_cast<size_t>(position - hayStack);
            }
            if (position == hayStack)
            {
                break;
            }
        }
    }
    return INDEX_NOT_FOUND;
}














template<typename Delimiter>
static inline std::string join(const Delimiter&) noexcept
{
    return std::string();
}

template<typename Delimiter, typename Arg0, typename... Args>
static inline std::string join(const Delimiter& delimiter, Arg0&& arg0, Args&&... args)
{
    constexpr size_t argCount = sizeof...(args) + 1;
    if constexpr (argCount == 1)
    { // we only have one argument so we can return it
        return makeString(std::forward<Arg0>(arg0));
    }
    const size_t delimiterSize = Detail::getStringSize(delimiter);
    const auto sizes = Detail::makeSizesArray(arg0, args...);
    const size_t totalSize = Detail::totalSizeHelper(sizes) + delimiterSize * (argCount - 1);
    std::string result(totalSize, char{}); // note: initialization and memcpy is faster than reserve and +=
    char* copyDestination = &result[0];
    Detail::copyToString(std::forward<Arg0>(arg0), sizes[0], copyDestination);
    size_t index = 1;
    copyDestination += sizes[0];
    ((
         Detail::copyToString(delimiter, delimiterSize, copyDestination),
         Detail::copyToString(args, sizes[index], copyDestination + delimiterSize),
         copyDestination += delimiterSize + sizes[index++]),
        ...);
    return result;
}


template<typename Delimiter, typename... Args>
static inline std::string joinOptional(const Delimiter& delimiter, Args&&... args)
{
    constexpr size_t argCount = sizeof...(args);
    if constexpr (argCount == 0)
    {
        return std::string();
    }
    if constexpr (argCount == 1) // we only have one argument so we can return it
    {
        return Detail::makeString(std::forward<Args>(args)...);
    }
    const size_t delimiterSize = Detail::getStringSize(delimiter);
    const auto sizes = Detail::makeSizesArray(args...);
    const size_t validArgumentCount = Detail::joinOptionalArgumentCount(args...);
    const size_t delimiterCount = (validArgumentCount != 0) ? validArgumentCount - 1 : 0;
    const size_t totalSize = Detail::totalSizeHelper(sizes) + delimiterSize * delimiterCount;
    std::string result(totalSize, char{}); // note: initialization and memcpy is faster than reserve and +=
    char* copyDestination = &result[0];
    bool isFirst = true;
    size_t index = 0;
    ((
         isFirst = Detail::joinOptionalCopyHelper(isFirst, args, sizes[index], delimiter, delimiterSize, copyDestination),
         ++index),
        ...);
    return result;
}


template<typename... Args>
static inline std::string concat(Args&&... args)
{
    constexpr size_t argCount = sizeof...(args);
    if constexpr (argCount == 0)
    { // we only have one argument so we can return it
        return std::string();
    }
    if constexpr (argCount == 1)
    { // we only have one argument so we can return it
        return Detail::makeString(std::forward<Args>(args)...);
    }
    const auto sizes = Detail::makeSizesArray(args...);
    const size_t totalSize = Detail::totalSizeHelper(sizes);
    std::string result(totalSize, char{}); // note: initialization and memcpy is faster than reserve and +=
    char* copyDestination = &result[0];
    size_t index = 0;
    ((
         Detail::copyToString(args, sizes[index], copyDestination),
         copyDestination += sizes[index++]),
        ...);
    return result;
}


inline std::vector<std::string> splitWithEmptySeparatorKeepEmptyParts(std::string_view source)
{
    const size_t sourceLength = source.length();
    std::vector<std::string> list(sourceLength + 1, std::string(1, '?'));
    list[0].clear();
    for (size_t j = 0; j < sourceLength; ++j)
    {
        list[j + 1][0] = source[j];
    }
    return list;
}

inline std::vector<std::string> splitWithEmptySeparatorSkipEmptyParts(std::string_view source)
{
    const size_t sourceLength = source.length();
    std::vector<std::string> list(sourceLength, std::string(1, '?'));
    for (size_t j = 0; j < sourceLength; ++j)
    {
        list[j][0] = source[j];
    }
    return list;
}

inline std::vector<std::string> splitKeepEmptyParts(std::string_view source, std::string_view separator)
{
    const size_t separatorSize = separator.size();
    if (separatorSize == 0)
    {
        return splitWithEmptySeparatorKeepEmptyParts(source);
    }
    std::vector<std::string> list;
    for (;;)
    {
        const size_t end = source.find(separator, 0);
        if (end == INDEX_NOT_FOUND)
        {
            break;
        }

        list.emplace_back(source.data(), end);
        source.remove_prefix(end + separatorSize);
    }
    list.emplace_back(source);
    return list;
}


inline std::vector<std::string> splitSkipEmptyParts(std::string_view source, std::string_view separator)
{
    const size_t separatorSize = separator.size();
    if (separatorSize == 0)
    {
        return splitWithEmptySeparatorSkipEmptyParts(source);
    }
    std::vector<std::string> list;
    for (;;)
    {
        const size_t end = source.find(separator, 0);
        if (end == INDEX_NOT_FOUND)
        {
            break;
        }

        if (end != 0)
        {
            list.emplace_back(source.data(), end);
        }
        source.remove_prefix(end + separatorSize);
    }
    if (!source.empty())
    {
        list.emplace_back(source);
    }
    return list;
}

inline std::vector<std::string> splitKeepEmptyParts(std::string_view source, char separator)
{
    std::vector<std::string> list;
    for (;;)
    {
        const size_t end = source.find(separator, 0);
        if (end == INDEX_NOT_FOUND)
        {
            break;
        }

        list.emplace_back(source.data(), end);
        source.remove_prefix(end + 1);
    }
    list.emplace_back(source);
    return list;
}


inline std::vector<std::string> splitSkipEmptyParts(std::string_view source, char separator)
{
    std::vector<std::string> list;
    for (;;)
    {
        const size_t end = source.find(separator, 0);
        if (end == INDEX_NOT_FOUND)
        {
            break;
        }

        if (end != 0)
        {
            list.emplace_back(source.data(), end);
        }
        source.remove_prefix(end + 1);
    }
    if (!source.empty())
    {
        list.emplace_back(source);
    }
    return list;
}


} // namespace Detail


//#######################################################################################
//
//                                      toString
//
//#######################################################################################


namespace Custom { // Namespace for custom implementation of toString (e.g. vector or other classes) that must be done outside the class
template<typename T>
struct toStringImpl
{
};
} // namespace Custom

namespace Detail {

template<typename T>
struct builtInToStringImpl
{
};

template<>
struct builtInToStringImpl<int>
{
    std::string operator()(const int v) const
    {
        return std::string("BuiltIn int");
    }
};

namespace TypeTraits {

// void_t workaround taken from https://en.cppreference.com/w/cpp/types/void_t
template<typename... Ts>
struct make_void
{
    typedef void type;
};
template<typename... Ts>
using void_t = typename make_void<Ts...>::type;

template<class T>
struct AlwaysFalse: std::false_type
{
};


// Struct nonesuch taken from https://en.cppreference.com/w/cpp/experimental/nonesuch
struct nonesuch
{
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
};

// std::experimental::is_detected implementation taken and adjusted from https://en.cppreference.com/w/cpp/experimental/is_detected
template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
struct detector
{
    using value_t = std::false_type;
    using type = Default;
};

template<class Default, template<class...> class Op, class... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...>
{
    using value_t = std::true_type;
    using type = Op<Args...>;
};


// Note: On MSVC 19.14 C++11 there is a compiler bug regarding is_detected
// TODO: find a workaround
template<template<class...> class Op, class... Args>
using is_detected = typename TypeTraits::detector<TypeTraits::nonesuch, void, Op, Args...>::value_t;

} // namespace TypeTraits

#ifndef STRINGUTILS_NO_STRINGSTREAM
template<class T>
using stringstream_operator_t = decltype(std::declval<std::ostringstream>() << std::declval<T>());

template<class T>
using has_stringstream_operator = TypeTraits::is_detected<stringstream_operator_t, T>;

#else

template<class T>
using has_stringstream_operator = std::false_type;
#endif

// TODO check if return type is std::string (or convertible to string)
template<class T>
using custom_toString_member_t = decltype(std::declval<T>().toString());

template<class T>
using has_custom_toString_member = TypeTraits::is_detected<custom_toString_member_t, T>;

// TODO check if return type is std::string (or convertible to string)
template<class T>
using custom_toString_t = decltype(std::declval<STRINGUTILS_NAMESPACE::Custom::toStringImpl<T>>().operator()(std::declval<T>()));

template<class T>
using has_custom_toString = TypeTraits::is_detected<custom_toString_t, T>;


template<class T>
using builtIn_toString_t = decltype(std::declval<STRINGUTILS_NAMESPACE::Detail::builtInToStringImpl<T>>().operator()(std::declval<T>()));

template<class T>
using has_builtIn_toString = TypeTraits::is_detected<builtIn_toString_t, T>;


template<typename T>
inline std::string toHexBytesString(const T& value)
{
    static constexpr auto chars = "0123456789ABCDEF";

    std::string result(sizeof(T) * 2, char{});
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
    const uint8_t* const end = bytes + sizeof(T);
    char* destination = &result[0];
    for (; bytes != end; ++bytes)
    {
        *destination = chars[(*bytes >> 4) & 0x0F];
        *(destination + 1) = chars[*bytes & 0x0F];
        destination += 2;
    }
    return result;
}

// overload for custom implementations outside of class (TODO check if is possible to overload StringUtils builtins )
template<typename T,
    typename std::enable_if<has_custom_toString<T>::value, bool>::type = true>
inline std::string toString(const T& value)
{
    constexpr Custom::toStringImpl<T> impl;
    return impl(value);
}

// overload for custom implementations in form of a toString method inside the class
template<typename T,
    typename std::enable_if<!has_custom_toString<T>::value
                                && has_custom_toString_member<T>::value,
        bool>::type = true>
inline std::string toString(const T& value)
{
    return value.toString();
}
// overload for builtIn types if there is no custom implementation
template<typename T,
    typename std::enable_if<!has_custom_toString<T>::value
                                && !has_custom_toString_member<T>::value
                                && has_builtIn_toString<T>::value,
        bool>::type = true>
inline std::string toString(const T& value)
{
    constexpr Detail::builtInToStringImpl<T> impl;
    return impl(value);
}


#ifndef STRINGUTILS_NO_STRINGSTREAM
// overload for types that have an ostream operator << if there is no custom nor builtin implementation
template<typename T,
    typename std::enable_if<!has_custom_toString<T>::value
                                && !has_custom_toString_member<T>::value
                                && !has_builtIn_toString<T>::value
                                && has_stringstream_operator<T>::value,
        bool>::type = true>
inline std::string toString(const T& value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}
#endif


#ifndef STRINGUTILS_NO_DEFAULT_TOSTRING
// overload for types that are not caught by custom implementations, stringstream and builtIn implementations
template<typename T,
    typename std::enable_if<!has_custom_toString<T>::value
                                && !has_custom_toString_member<T>::value
                                && !has_builtIn_toString<T>::value
                                && !has_stringstream_operator<T>::value,
        bool>::type = true>
inline std::string toString(const T& value)
{
    return toHexBytesString(value);
}
#else

template<typename T,
    typename std::enable_if<!has_custom_toString<T>::value
                                && !has_custom_toString_member<T>::value
                                && !has_builtIn_toString<T>::value
                                && !has_stringstream_operator<T>::value,
        bool>::type = true>
inline std::string toString(const T& value)
{
    static_assert(TypeTraits::AlwaysFalse<T>::value, "Failed to find suitable overload for toString");
    return "";
}
#endif

// TODO: explicit enable_if for builtin functions ( maybe even an extra method builtInToString???? )
// Maybe that way we can have a builtIn toString function and a custom one

} // namespace Detail

} // namespace STRINGUTILS_NAMESPACE


#endif // !STRINGUTILS_PRIVATE_HPP
