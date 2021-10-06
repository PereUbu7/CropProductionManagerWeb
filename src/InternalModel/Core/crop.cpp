#include "CropProductionManager/InternalModel/Core/crop.h"
#include "CropProductionManager/InternalModel/Infrastructure/crop.h"

namespace CropProductionManager::InternalModel::Core
{
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;

    void Crop::Update(Crop crop)
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

    Crop Crop::FromInfrastructure(Infrastructure::Crop crop)
    {
        return Crop{ crop };
    }
}