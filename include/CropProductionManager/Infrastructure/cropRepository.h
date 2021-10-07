#pragma once

#include "CropProductionManager/InternalModel/Infrastructure/crop.h"
#include "CropProductionManager/Infrastructure/iRepository.h"
#include <vector>

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    class CropRepository : public CropProductionManager::Infrastructure::IRepository<Crop>
    {
    private:
        std::vector<Crop> crops;
    public:
        // GET
        std::vector<Crop> Get() const;
        // POST
        void Post(Crop crop);
        // PUT
        void Put(Crop crop);
        // DELETE
        void Remove(int id);

        CropRepository();
    };
}