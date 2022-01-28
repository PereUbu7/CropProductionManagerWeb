#include "CropProductionManager/InternalModel/Infrastructure/crop.h"

namespace CropProductionManager::InternalModel::Infrastructure
{
    void Crop::Update(const Crop& crop)
    {
        id = crop.id;
        name = crop.name;
        variety = crop.variety;
        batch = crop.batch;
    }
}