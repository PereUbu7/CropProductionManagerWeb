#include <string>

class string
{
    public:
     string() : data{} {};
     string(const char* str) : data{str} {};
     string(const std::string& str) : data{str} {};
     int indexOf(const char& ch);
     int indexOf(const char (&ch)[]);
     char &at(std::size_t index);
     string& trim();
     string& toBase64();
     string& toHex();
     string& toUpper();
     string& toLower();
     string& toUtf8();
     string& toLocal8Bit();
     std::string toStdString();

     const bool operator==(const string& other) { return data == other.data; };
    
    private:
        std::string data;
};