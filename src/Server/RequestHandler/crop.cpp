#include "CropProductionManager/Server/RequestHandler/crop.h"

#include <iostream>

namespace CropProductionManager::Server::RequestHandler::CropMethod
{
    using namespace restbed;
    using namespace std;

    void Get(const shared_ptr<Session> session)
    {
        try
        {
            auto request = session->get_request();

            auto requestedId = std::stoi(request->get_path_parameter("id", "0"));

            // Get actual data

            CropProductionManager::Core::CropCore core{implementationHolder->Repository};
            CropProductionManager::Api::CropApi api{core};
            std::string json;

            if (request->has_path_parameter("id"))
            {
                CropProductionManager::Serializer::Serializer<CropProductionManager::ModelApi::Crop> serializer{};
                auto crop{api.Get(requestedId)};
                json = serializer.Serialize(crop).GetJson().dump();
            }
            else
            {
                CropProductionManager::Serializer::Serializer<std::vector<CropProductionManager::ModelApi::Crop>> serializer{};
                auto crops{api.Get()};
                json = serializer.Serialize(crops).GetJson().dump();
            }

            session->close(OK, json,
                           {{"Content-Length", std::to_string(json.length())}});
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            session->close(NOT_FOUND, "",
                           {{"Content-Length", "0"}});
        }
    }

    void Post(const shared_ptr<Session> session)
    {
        try
        {
            auto request = session->get_request();

            int content_length{};
            request->get_header("Content-Length", content_length, 0);

            session->fetch(content_length, [](const shared_ptr<Session> session, const Bytes &body) {});

            auto byteBody = request->get_body();
            auto body = string(byteBody.begin(), byteBody.end());

            // Post actual data
            CropProductionManager::Serializer::Serializer<CropProductionManager::ModelApi::Crop> serializer{};

            CropProductionManager::Core::CropCore core{implementationHolder->Repository};
            CropProductionManager::Api::CropApi api{core};

            api.Post(serializer.Deserialize(body));

            session->close(OK, "",
                           {{"Content-Length", "0"}});
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            session->close(NOT_FOUND, "",
                           {{"Content-Length", "0"}});
        }
    }

    void Put(const shared_ptr<Session> session)
    {
        auto request = session->get_request();

        int content_length{};
        request->get_header("Content-Length", content_length, 0);

        session->fetch(content_length, [](const shared_ptr<Session> session, const Bytes &body) {});

        auto byteBody = request->get_body();
        auto body = string(byteBody.begin(), byteBody.end());

        // Post actual data
        CropProductionManager::Serializer::Serializer<CropProductionManager::ModelApi::Crop> serializer{};

        CropProductionManager::Core::CropCore core{implementationHolder->Repository};
        CropProductionManager::Api::CropApi api{core};

        api.Put(serializer.Deserialize(body), request->get_path_parameter("id", ""));
    }

    void Delete(const shared_ptr<Session> session)
    {
        auto request = session->get_request();

        int content_length{};
        request->get_header("Content-Length", content_length, 0);

        session->fetch(content_length, [](const shared_ptr<Session> session, const Bytes &body) {});

        // Delete actual data
        CropProductionManager::Serializer::Serializer<CropProductionManager::ModelApi::Crop> serializer{};

        CropProductionManager::Core::CropCore core{implementationHolder->Repository};
        CropProductionManager::Api::CropApi api{core};

        api.Remove(request->get_path_parameter("id", ""));
    }
}