#ifndef FORMAT_H
#define FORMAT_H

#include <limits>
#include <string>

class Format
{
    public:

        /// @brief Finds size of value in utf8 character set
        /// @param _str Textual value
        /// @return Size of textual value in UTF8 char set
        static int cutf8CharacterWidth(const std::string& _str);

        /// @brief Checks if value is number
        /// @param _str Textual value
        /// @return True if it is number and false if not
        static bool cisNumber(const std::string& _str);

        /// @brief Checks if value is null
        /// @param _element Textual value
        /// @return True if the value is null and false if not
        template<class T>
        static bool ccheckNull(T _element);
};

#include "../source/format.cpp"

#endif // FORMAT_H