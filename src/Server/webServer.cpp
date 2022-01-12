#include "CropProductionManager/Server/webServer.h"

namespace CropProductionManager::Server
{
    WebServer::WebServer(const string ip, const uint16_t port) :
        _ip{ip}, _port{port} {}

    string WebServer::build_authenticate_header(void)
    {
        std::cout << "Building auth header\n";

        string header = "Digest realm=\"Restbed\",";
        header += "algorithm=\"MD5\",";
        header += "stale=false,";
        header += "opaque=\"0000000000000000\",";
        header += "nonce=\"Ny8yLzIwMDIgMzoyNjoyNCBQTQ\"";

        return header;
    }

    void WebServer::authentication_handler(const shared_ptr<Session> session,
                                           const function<void(const shared_ptr<Session>)> &callback)
    {
        const auto request = session->get_request();

        auto authorization = request->get_header("Authorization");

        // Password = "martin" hashed with MD5
        bool authorized = regex_match(authorization, regex(".*response=\"d66b9e169da3476040fe923c2bca9b3b\".*"));

        if (authorized)
        {
            cout << "Is authorized\n"
                 << authorization << '\n';
            callback(session);
        }
        else
        {
            cout << "Is NOT authorized\n"
                 << authorization << '\n';
            session->close(UNAUTHORIZED, {{"WWW-Authenticate", WebServer::build_authenticate_header()}});
        }
    }

    void WebServer::Setup()
    {
        auto cropResource = make_shared<Resource>();
        cropResource->set_path("/crop");
        cropResource->set_method_handler("GET", RequestHandler::CropMethod::Get);
        cropResource->set_method_handler("POST", RequestHandler::CropMethod::Post);
        CropProductionManager::Infrastructure::RepositoryFake<Infrastructure::Crop> repository{};
        RequestHandler::CropImpl cropImpl{repository};
        RequestHandler::CropMethod::implementationHolder = make_unique<RequestHandler::CropImpl>(cropImpl);

        auto settings = make_shared<Settings>();
        settings->set_port(_port);
        settings->set_bind_address(_ip);
        settings->set_default_header("Connection", "close");

        Service service;

        auto dnsHandler = make_shared<DnsHandler>(".*mysite.blaj.*");
        service.add_rule(dnsHandler);
        // service.set_authentication_handler(authentication_handler);
        service.publish(cropResource);
        service.start(settings);
    }
}