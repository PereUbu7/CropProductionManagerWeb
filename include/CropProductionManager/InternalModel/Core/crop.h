#pragma once

#include "CropProductionManager/InternalModel/Infrastructure/crop.h"
#include "CropProductionManager/ModelApi/crop.h"

#include <string>

namespace CropProductionManager::InternalModel::Core
{
    using String = std::string;
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;
    namespace Api = CropProductionManager::ModelApi;

    class Crop final
    {
    public:
        int id;
        String name;
        String variety;
        int batch;

        Crop(int id, const String& name, const String& variety, int batch) :
            id{id},
            name{name},
            variety{variety},
            batch{batch} {}

        Crop(const Infrastructure::Crop& infra) :
            id{infra.id},
            name{infra.name},
            variety{infra.variety},
            batch{infra.batch}
        { 
        }

        Crop(const Api::Crop& infra) :
            id{infra.id},
            name{infra.name},
            variety{infra.variety},
            batch{infra.batch}
        { 
        }

        void Update(const Crop& crop);
        Infrastructure::Crop ToInfrastructure();
        Api::Crop ToApi();
        Crop static FromInfrastructure(const Infrastructure::Crop& crop);
        Crop static FromApi(const Api::Crop& crop);
    };
}