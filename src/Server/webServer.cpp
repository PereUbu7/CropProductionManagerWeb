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
        auto completePath = std::string{getCurrentPath() + std::string("/appconfig.json")};
        auto i = std::ifstream(completePath);
        
        auto config = IConfiguration(i);

        // CropProductionManager::Infrastructure::RepositoryFake<Infrastructure::Crop> repository{};
        CropProductionManager::Infrastructure::CropRepository repository{config["databaseConnection"]};
        const auto cropResource = addCropResource(repository);

        auto settings = make_shared<Settings>();
        settings->set_port(_port);
        settings->set_bind_address(_ip);
        settings->set_default_headers({
            {"Connection", "close"},
            {"Content-Type", "application/json"} 
        });
        

        Service service;

        auto dnsHandler = make_shared<DnsHandler>(".*mysite.blaj.*");
        service.add_rule(dnsHandler);
        // service.set_authentication_handler(authentication_handler);
        service.publish(cropResource);
        service.start(settings);
    }

    shared_ptr<Resource> WebServer::addCropResource(CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop> &repo)
    {
        auto cropResource = make_shared<Resource>();
        cropResource->set_paths({"/crop", "/crop/{id: [0-9]*}"});
        cropResource->set_method_handler("GET", RequestHandler::CropMethod::Get);
        cropResource->set_method_handler("POST", RequestHandler::CropMethod::Post);
        cropResource->set_method_handler("PUT", RequestHandler::CropMethod::Put);
        cropResource->set_method_handler("DELETE", RequestHandler::CropMethod::Delete);
        RequestHandler::CropImpl cropImpl{repo};
        RequestHandler::CropMethod::implementationHolder = std::make_unique<RequestHandler::CropImpl>(cropImpl);
        return cropResource;
    }

    std::string WebServer::getCurrentPath()
    {
        char result[200];
        ssize_t count = readlink("/proc/self/exe", result, 200);
        if(count != -1)
        {
            return std::string{dirname(result)};
        }
        return std::string{};
    }
}