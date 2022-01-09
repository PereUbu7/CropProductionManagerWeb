#include "CropProductionManager/Server/url.h"

#include <string>
#include <iostream>

namespace CropProductionManager::Server::Utils
{
    Url::Url(std::string s) { this->url = s; }
    std::string const Url::GetHost() { return this->url; }
    void Url::Set(std::string str) { this->url = str; }

    std::ostream &operator<<(std::ostream &strm, Url &a)
    {
        return strm << "Url(" << a.GetHost() << ")";
    }
}