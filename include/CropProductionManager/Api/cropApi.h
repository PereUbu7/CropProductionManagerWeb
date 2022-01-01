#pragma once

#include "CropProductionManager/ModelApi/crop.h"
#include "CropProductionManager/InternalModel/Core/crop.h"
#include "CropProductionManager/InternalModel/Infrastructure/crop.h"
#include "CropProductionManager/Api/iApi.h"
#include "CropProductionManager/Core/iCore.h"

namespace CropProductionManager::Api
{
    using Crop = CropProductionManager::ModelApi::Crop;
    namespace Core = CropProductionManager::InternalModel::Core;

    class CropApi : public CropProductionManager::Api::IApi<Crop, Core::Crop>
    {
    protected:
        CropProductionManager::Core::ICore<Core::Crop, CropProductionManager::InternalModel::Infrastructure::Crop>& _core;

    public:
        // GET
        std::vector<Crop> Get() const;
        // POST
        Crop Post(const Crop& crop);
        // PUT
        void Put(const Crop& crop);
        // DELETE
        void Remove(int id);

        CropApi(CropProductionManager::Core::ICore<Core::Crop, CropProductionManager::InternalModel::Infrastructure::Crop>& core):
            _core{core} {}
    };
}