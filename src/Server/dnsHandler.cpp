#include "CropProductionManager/Server/dnsHandler.h"

namespace CropProductionManager::Server
{
    using namespace std;
    DnsHandler::DnsHandler(std::string dns) :
    _dns{dns} {}
    
    bool DnsHandler::condition(const std::shared_ptr<Session> session)
    {
        const auto request = session->get_request();

        auto uri = Utils::Url(request->get_header("Host", ""));
        cout << uri << std::endl;
        if (regex_match(uri.GetHost(), regex(_dns)))
        {
            cout << "Matched DNS\n";
            return false;
        }
        else
        {
            cout << "Not matched DNS - invoking action\n";
            return true;
        }
    }

    void DnsHandler::action(
        const std::shared_ptr<Session> session,
        const std::function<void(const std::shared_ptr<Session>)> &callback)
    {
        std::cout << "action: 404 - NOT FOUND\n";
        session->close(NOT_FOUND, "");
    }
}