#include "CropProductionManager/Server/webServer.h"

namespace CropProductionManager::Server
{
    
    DigestManager digest_manager;
    WebServer::WebServer(IConfiguration config) : _logger{},
                                                  _ip{config["server"]},
                                                  _port{config["port"]}
    {
    }

    string WebServer::build_authenticate_header(void)
    {
        std::cout << "Building auth header\n";

        std::string header = "Digest realm=\"CropProductionManagerApi\",";
        header += "algorithm=\"MD5-sess\",";
        header += "stale=false,";
        header += "opaque=\"0000000000000000\",";
        header += "nonce=\"Ny8yLzIwMDIgMzoyNjoyNCBQTQ\"";

        return header;
    } // 24ad34a092a0396f60feb0f81d9195ab"
      // 7c8c0e29d129144bc194eb1086064944

    std::map<std::string, std::string> toMap(auto multimap)
    {
        auto map = std::map<std::string, std::string>{};

        for (const auto &[key, value] : multimap)
            map.try_emplace(std::pair{key, value});
        return map;
    }

    void WebServer::authentication_handler(const shared_ptr<Session> session,
                                           const function<void(const shared_ptr<Session>)> &callback)
    {
        // Following instructions here:
    // https://github.com/bertrandmartel/digest-auth-session-cpp
    
    digest_manager.set_digest_algorithm(ALGO_SHA256);
    digest_manager.set_session_type(SESSION_COOKIE);

    Authentication::HashDigestListener digest_listener;
    digest_manager.set_digest_listener(&digest_listener);

    const auto request = session->get_request();
    auto headers = request->get_headers();
    auto authorization = request->get_header("Authorization");

    std::map<std::string, std::string> mappedHeaders;

    for (const auto &[key, value] : headers)
    {
        mappedHeaders.try_emplace(key, value);
        // out.emplace_back();
        // for (; it1->first == it2->first; ++it2)
        // {
        //     out.back().push_back(it2->second);
        // }
    }

    const auto digestInfo = digest_manager.process_digest(
        request->get_method(),
        request->get_path(),
        mappedHeaders,
        "CropProductionManagerApi");

    bool authorized = digestInfo.get_status_code() == 200;

    // // Password = "martin" hashed with MD5
    // bool authorized = regex_match(authorization, regex(".*response=\"d66b9e169da3476040fe923c2bca9b3b\".*"));

    if (authorized)
    {
        std::cout << "Is authorized\n"
                  << authorization << '\n';
        callback(session);
    }
    else
    {
        // const auto headers = ;
        std::cout << "Is NOT authorized\n"
                  << authorization << '\n';
        session->close(UNAUTHORIZED, digestInfo.get_headers());
    }
    }

    void WebServer::Setup(IConfiguration &config)
    {
        _logger.LogDebug("Initializing database connection");
        CropProductionManager::Infrastructure::CropRepository repository{config["databaseConnection"]};
        const auto cropResource = addCropResource(repository);

        _logger.LogDebug("Setting up service with address: " + _ip + ":" + std::to_string(_port));
        auto settings = std::make_shared<Settings>();
        settings->set_port(_port);
        settings->set_bind_address(_ip);
        settings->set_default_headers({{"Content-Type", "application/json"}});

        Service service;

        _logger.LogDebug("Setting dns to match: " + std::string(config["service"]["dnsMatcher"]));
        auto dnsHandler = std::make_shared<DnsHandler>(config["service"]["dnsMatcher"]);
        service.add_rule(dnsHandler);
        service.set_authentication_handler(authentication_handler);
        service.publish(cropResource);
        service.start(settings);
    }

    std::shared_ptr<Resource> WebServer::addCropResource(CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop> &repo)
    {
        auto cropResource = std::make_shared<Resource>();
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