#include "CropProductionManager/Server/RequestHandler/crop.h"

#include <iostream>

namespace CropProductionManager::Server::RequestHandler::CropMethod
{
    using namespace restbed;
    using namespace std;

    void Get(const shared_ptr<Session> session)
    {
        std::cout << "Get request\n";
        auto request = session->get_request();

        std::cout << "Path: " << request->get_path() << "\n";
        // Get actual data
        namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;

        CropProductionManager::Serializer::Serializer<std::vector<CropProductionManager::ModelApi::Crop>> serializer{};

        CropProductionManager::Core::CropCore core{implementationHolder->Repository};
        CropProductionManager::Api::CropApi api{core};

        auto crops{api.Get()};
        auto json{serializer.Serialize(crops).GetJson().dump()};

        std::cout << json << '\n';

        session->close(OK, json,
                       {{"Content-Length", std::to_string(json.length())},
                        {"Content-Type", "application/json"}});
    }

    void Post(const shared_ptr<Session> session)
    {
        std::cout << "Post request\n";
        auto request = session->get_request();

        int content_length{};
        request->get_header("Content-Length", content_length, 0);

        session->fetch(content_length, [](const shared_ptr<Session> session, const Bytes &body){});

        auto byteBody = request->get_body();
        std::cout << "body length: " << byteBody.size() << '\n';
        string body(byteBody.begin(), byteBody.end());

        for (auto c : byteBody)
        {
            std::cout << c;
        }
        std::cout << '\n';

        std::cout << "Path: " << request->get_path() << "\n";
        std::cout << "Body: " << body << '\n';
        // Post actual data
        namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;

        CropProductionManager::Serializer::Serializer<CropProductionManager::ModelApi::Crop> serializer{};

        CropProductionManager::Core::CropCore core{implementationHolder->Repository};
        CropProductionManager::Api::CropApi api{core};

        api.Post(serializer.Deserialize(body));
    }
}