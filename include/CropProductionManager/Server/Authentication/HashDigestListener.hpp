#include "digest-auth-session-lib/IDigestListener.h"
#include "digest-auth-session-lib/digeststruct.h"

#include <string>

namespace CropProductionManager::Server::Authentication
{
    class HashDigestListener : public IDigestListener
    {

    public:
        /**
         * @brief ClientSocketObj::ClientSocketObj
         *      Build one client
         */
        HashDigestListener() = default;

        ~HashDigestListener() = default;

        /**
         * @brief get_hash_for_username
         *      retrieve hash in database or in your configuration for specified username and realm
         * @return
         */
        std::string get_hash_for_username(std::string username, std::string realm, digest_algorithm algo);
    };
}