#include "CropProductionManager/Server/url.h"

#include <restbed>
#include <regex>
#include <string>

namespace CropProductionManager::Server
{
    using namespace restbed;
    class DnsHandler final : public Rule 
    {
        public:
            DnsHandler(std::string dns);
            ~DnsHandler() = default;
            bool condition(const std::shared_ptr<Session> session) override;
            void action(
            const std::shared_ptr<Session> session,
            const std::function<void(const std::shared_ptr<Session>)> &callback) override;

        private:
            std::string _dns;
    };
}