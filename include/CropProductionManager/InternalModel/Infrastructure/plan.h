#pragma once
#include <vector>

namespace CropProductionManager::InternalModel::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    class Plan
    {
    public:
        int year;
        std::vector<Crop> crops;
    };
}