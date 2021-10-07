#include "CropProductionManager/InternalModel/Core/crop.h"
#include "CropProductionManager/InternalModel/Infrastructure/crop.h"

namespace CropProductionManager::InternalModel::Core
{
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;

    void Crop::Update(const Crop crop)
    {
        id = crop.id;
        name = crop.name;
        variety = crop.variety;
        batch = crop.batch;
    }

    Infrastructure::Crop Crop::ToInfrastructure()
    {
        return Infrastructure::Crop{ id, name, variety, batch };
    }

    Crop Crop::FromInfrastructure(const Infrastructure::Crop crop)
    {
        return Crop{ crop };
    }

    Api::Crop Crop::ToApi()
    {
        return Api::Crop{ id, name, variety, batch };
    }

    Crop Crop::FromApi(const Api::Crop crop)
    {
        return Crop{ crop };
    }
}