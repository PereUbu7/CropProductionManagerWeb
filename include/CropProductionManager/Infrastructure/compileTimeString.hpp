#include <cstddef>
#include <iterator>

namespace compiletime {

template<std::size_t MaxSize = 30>
class string
{
    char m_data[MaxSize] = { 0 };
    std::size_t m_size;
public:
    constexpr string() : m_data({}), m_size(0) {}
    constexpr string(char* str, std::size_t length) : m_data(), m_size(length)
    {
        for(int i{0}; i<length; ++i)
        {
            m_data[i] = str[i];
        }
    }
    constexpr string(const char* str) : m_data(), m_size(0) {
        for(int i =0; i<MaxSize && str[i] != '\0'; ++i) {
            m_data[m_size++] = str[i];
        }
    }
    
    constexpr char const* data() const { return m_data; }
    constexpr operator const char*() const { return data(); } // for convenience
    constexpr void push_back(char c) { m_data[m_size++] = c; }
    constexpr char& operator[](std::size_t i) { return m_data[i]; } // set
    constexpr char const& operator[](std::size_t i) const { return m_data[i]; } // get
    constexpr size_t size() const { return m_size; }
    constexpr const char* begin() const { return std::begin(m_data); }
    constexpr const char* end() const { return std::begin(m_data) + m_size; }
    constexpr const bool operator==(const string& other) const { return compare(other.data(), other.size()); }
    constexpr const bool compare(const char *other, const int size) const
    {
        if(m_size != size)
            return false;
        return compare(other);
    }
    constexpr const bool compare(const char *other) const
    {
        for(int index{0}; other[index] != '\0' && index < m_size; ++index)
        {
            if(m_data[index] != other[index])
                return false;
        }
        return true;
    }

};

}