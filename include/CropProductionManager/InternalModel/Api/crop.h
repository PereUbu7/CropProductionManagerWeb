#pragma once
#include <string>

namespace CropProductionManager::InternalModel::Api
{
    using String = std::string;

    class Crop
    {
    public:
        int id;
        String name;
        String variety;
        int batch;
    };
}