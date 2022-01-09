#include <string>
#include <iostream>

namespace CropProductionManager::Server::Utils
{
    class Url
    {
    private:
        std::string url;
    public:
        Url(std::string s);
        std::string const GetHost();
        void Set(std::string str);
    };

    std::ostream& operator<<(std::ostream& strm, Url& a);
}