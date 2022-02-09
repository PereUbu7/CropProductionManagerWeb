#pragma once

#include "CropProductionManager/InternalModel/Infrastructure/crop.h"
#include "CropProductionManager/Infrastructure/iRepository.h"
#include "CropProductionManager/Infrastructure/dbContext.hpp"
#include "CropProductionManager/iConfiguration.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    class CropRepository final : public CropProductionManager::Infrastructure::IRepository<Crop>
    {
    private:
        std::vector<Crop> crops;
        DbContext context;

    public:
        CropRepository(IConfiguration config);
        // GET
        std::vector<Crop> Get() const;
        Crop Get(const int id) const;
        // POST
        void Post(Crop crop);
        // PUT
        void Put(Crop crop);
        // DELETE
        void Remove(int id);
    };
}