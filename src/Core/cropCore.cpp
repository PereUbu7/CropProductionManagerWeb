#include "CropProductionManager/Core/cropCore.h"
#include <iostream>
#include <algorithm>

namespace CropProductionManager::Core
{
    using Crop = CropProductionManager::InternalModel::Core::Crop;
    // CropCore::CropCore() {}
    
    std::vector<Crop> CropCore::Get() const
    {
        std::vector<Infrastructure::Crop> infraCrops{_repository.Get()};

        std::vector<Crop> crops{};
        crops.reserve(infraCrops.size());
        for(auto crop : infraCrops) 
        { 
            crops.push_back(std::move(Crop::FromInfrastructure(crop)));
        }

        return crops;
    } 
    Crop CropCore::Post(Crop& crop)
    {
        crop.id = CropCore::createId();

        _repository.Post(crop.ToInfrastructure());

        return crop;
    }
    void CropCore::Put(Crop& crop)
    {
        _repository.Put(crop.ToInfrastructure());
    }
    void CropCore::Remove(int id)
    {
        _repository.Remove(id);
    }

    Infrastructure::Crop CropCore::ToInfrastructure(Crop& c) const
    {
        return c.ToInfrastructure();
    }

    int CropCore::createId()
    {
        static int id{0};

        return id++;
    }
}