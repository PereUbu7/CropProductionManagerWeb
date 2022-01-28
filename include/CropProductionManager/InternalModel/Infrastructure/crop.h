#pragma once
#include <string>

namespace CropProductionManager::InternalModel::Infrastructure
{
    using String = std::string;

    class Crop final
    {
    public:
        int id;
        String name;
        String variety;
        int batch;
        void Update(const Crop& crop);
    };
}