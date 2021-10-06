#pragma once
#include <string>

namespace CropProductionManager::InternalModel::Infrastructure
{
    using String = std::string;

    class Crop
    {
    public:
        int id;
        String name;
        String variety;
        int batch;
        void Update(Crop crop);
    };
}