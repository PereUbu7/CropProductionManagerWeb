#pragma once

#include "CropProductionManager/Server/dnsHandler.h"
#include "CropProductionManager/Server/RequestHandler/crop.h"
#include "CropProductionManager/Infrastructure/repositoryFake.h"
#include "CropProductionManager/iConfiguration.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <restbed>
#include <regex>


namespace CropProductionManager::Server
{
    using namespace restbed;
    using namespace std;
    using namespace RequestHandler;

    class WebServer final
    {
        public:
            WebServer(IConfiguration config);
            void Setup(IConfiguration& config);
            static string build_authenticate_header(void);
            static void authentication_handler(const shared_ptr<Session> session,
                                const function<void(const shared_ptr<Session>)> &callback);
            static void get_method_handler(const shared_ptr<Session> session);
        private:
            const string _ip;
            const uint16_t _port;
            shared_ptr<Resource> addCropResource(CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop> &repo);
    };
}