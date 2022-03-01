#include "CropProductionManager/Server/webServer.h"


namespace CropProductionManager::Server
{
    WebServer::WebServer(IConfiguration config) :
        _logger{},
        _ip{config["server"]}, 
        _port{config["port"]}
         {}

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

    void WebServer::Setup(IConfiguration& config)
    {
        _logger.LogDebug("Initializing database connection");
        CropProductionManager::Infrastructure::CropRepository repository{config["databaseConnection"]};
        const auto cropResource = addCropResource(repository);

        _logger.LogDebug("Setting up service with address: " + _ip + ":" + std::to_string(_port));
        auto settings = make_shared<Settings>();
        settings->set_port(_port);
        settings->set_bind_address(_ip);
        settings->set_default_headers({
            {"Connection", "close"},
            {"Content-Type", "application/json"} 
        });
        

        Service service;

        _logger.LogDebug("Setting dns to match: " + std::string(config["service"]["dnsMatcher"]));
        auto dnsHandler = make_shared<DnsHandler>(config["service"]["dnsMatcher"]);
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
}