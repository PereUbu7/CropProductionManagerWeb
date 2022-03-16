#include "CropProductionManager/Server/Authentication/HashDigestListener.hpp"

namespace CropProductionManager::Server::Authentication
{
    std::string HashDigestListener::get_hash_for_username(std::string username, std::string realm, digest_algorithm algo)
    {
        return std::string{"c1fd2634910a0432e2bca0f681a769727f68631a41601c2cc72177a33f105634"};
    }
}