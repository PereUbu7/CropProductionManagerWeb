#include "CropProductionManager/Server/dnsHandler.h"

namespace CropProductionManager::Server
{
    using namespace std;
    DnsHandler::DnsHandler(std::string dns) :
    _dns{dns} {}
    
    DnsHandler::~DnsHandler(void) { return; }
    bool DnsHandler::condition(const std::shared_ptr<Session> session)
    {
        const auto request = session->get_request();

        Utils::Url uri(request->get_header("Host", ""));
        cout << uri << std::endl;
        if (regex_match(uri.GetHost(), regex(_dns)))
        {
            cout << "Matched DNS" << endl;
            return true;
        }
        else
        {
            cout << "Not matched DNS" << endl;
            return false;
        }
    }

    void DnsHandler::action(
        const std::shared_ptr<Session> session,
        const std::function<void(const std::shared_ptr<Session>)> &callback)
    {
        session->close(NOT_FOUND, "");
    }
}