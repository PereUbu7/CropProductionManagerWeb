#pragma once

#include "CropProductionManager/InternalModel/Core/crop.h"
#include "CropProductionManager/Core/iCore.h"
#include "CropProductionManager/Infrastructure/iRepository.h"

namespace CropProductionManager::Core
{
    using Crop = CropProductionManager::InternalModel::Core::Crop;
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;
    using IRepository = CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop>;

    class CropCore : public CropProductionManager::Core::ICore<Crop, Infrastructure::Crop>
    {
    protected:
        CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop>& _repository;
        std::vector<Crop> crops;

        static int createId();
    public:
        // GET
        std::vector<Crop> Get() const;
        Crop Get(const int id) const;
        // POST
        Crop Post(Crop& crop);
        // PUT
        void Put(Crop& crop);
        // DELETE
        void Remove(int id);

        Infrastructure::Crop ToInfrastructure(Crop& c) const;

        CropCore(CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop>& repository):
            _repository{repository} {}
    };
}