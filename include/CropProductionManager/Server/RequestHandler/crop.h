#pragma once

#include "CropProductionManager/Infrastructure/iRepository.h"
#include "CropProductionManager/InternalModel/Infrastructure/crop.h"

#include "CropProductionManager/Infrastructure/cropRepository.h"
#include "CropProductionManager/Core/cropCore.h"
#include "CropProductionManager/Api/cropApi.h"

#include "CropProductionManager/Serializer/serializer.h"

#include <restbed>
#include <memory>

namespace CropProductionManager::Server::RequestHandler
{
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;
    class CropImpl final
    {
    public:
        CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop> &Repository;
        CropImpl(CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop> &repository) : Repository{repository} {}
    };

    namespace CropMethod
    {
        using namespace restbed;
        using namespace std;

        inline unique_ptr<CropProductionManager::Server::RequestHandler::CropImpl> implementationHolder;

        void Get(const shared_ptr<Session> session);
        void Post(const shared_ptr<Session> session);
        void Put(const shared_ptr<Session> session);
        void Delete(const shared_ptr<Session> session);
    }
}