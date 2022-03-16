#include "string.h"

#include <algorithm>
#include <cctype>
#include <locale>
#include <string.h>

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

char &string::at(std::size_t index)
{
    return data.at(index);
}

int string::indexOf(const char& ch)
{
    for(int i{0}; i<data.length(); ++i)
    {
        if(data.data()[i] == ch) return i;
    }
    return -1;
}

int string::indexOf(const char (&ch)[])
{
    auto p = strstr(data.data(), ch);
    auto index = p == nullptr ? -1 : (p - data.data());
    return index;
}

string& string::trim()
{
    ltrim(data);
    rtrim(data);

    return *this;
}

string& string::toUpper()
{
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    return *this;
}

string& string::toLower()
{
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return *this;
}

string& string::toHex()
{
    return *this;
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(data.length() * 2);
    for (unsigned char c : data)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    data = output;
    return *this;
}

string& string::toBase64()
{
    return *this;
}

string& string::toLocal8Bit()
{
    return *this;
}

string& string::toUtf8()
{
    return *this;
}

std::string string::toStdString()
{
    return data;
}