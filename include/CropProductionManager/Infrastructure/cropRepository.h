#pragma once

#include "CropProductionManager/InternalModel/Infrastructure/crop.h"
#include "CropProductionManager/Infrastructure/iRepository.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    class CropRepository final : public CropProductionManager::Infrastructure::IRepository<Crop>
    {
    private:
        std::vector<Crop> crops;
    public:
        CropRepository();
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