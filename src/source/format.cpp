#include "../headers/format.h"

int Format::cutf8CharacterWidth(const std::string& _str)
{
    int count = 0;
    for (size_t i = 0; i < _str.size(); ) {
        unsigned char ch = _str[i];
        if (ch < 0x80) {
            i += 1;
        } else if (ch < 0xE0) {
            i += 2;
        } else if (ch < 0xF0) {
            i += 3;
        } else if (ch < 0xF8) {
            i += 4;
        } else {
            i += 1; // Invalid character, skip
        }
        count++;
    }
    return count;
}


bool Format::cisNumber(const std::string& _str)
{
    std::istringstream iss(_str);
    double num;
    iss >> std::noskipws >> num; // Try to convert to a number
    return iss.eof() && !iss.fail();
}

template<>
bool Format::ccheckNull(std::string _element)
{
    if (_element == "" || 
        _element == "#NULL!" ||
        _element == "NA")
    {
        return true;
    }

    return false;
}

template<>
bool Format::ccheckNull(double _element)
{
    if (_element == std::numeric_limits<double>::min())
    {
        return true;
    }

    return false;
}