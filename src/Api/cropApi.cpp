#include "CropProductionManager/Api/cropApi.h"

namespace CropProductionManager::Api
{
    using Crop = CropProductionManager::ModelApi::Crop;
    
    std::vector<Crop> CropApi::Get() const
    {
        auto coreCrops = std::vector<Core::Crop>{_core.Get()};

        auto crops = std::vector<Crop>{};
        crops.reserve(coreCrops.size());
        for(auto crop : coreCrops) 
        { 
            crops.push_back(std::move(crop.ToApi()));
        }

        return crops;
    } 
    Crop CropApi::Get(const int id) const
    {
        auto coreCrop = _core.Get(id);
        return coreCrop.ToApi();
    }
    Crop CropApi::Post(const Crop& crop)
    {
        auto coreCrop{ Core::Crop::FromApi(crop) };
        _core.Post(coreCrop);

        return crop;
    }
    void CropApi::Put(const Crop& crop, const std::string &id)
    {
        auto coreCrop{ Core::Crop::FromApi(crop) };
        _core.Put(coreCrop, id);
    }
    void CropApi::Remove(const std::string &id)
    {
        _core.Remove(id);
    }
}