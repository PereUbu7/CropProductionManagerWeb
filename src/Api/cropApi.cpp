#include "CropProductionManager/Api/cropApi.h"

namespace CropProductionManager::Api
{
    using Crop = CropProductionManager::ModelApi::Crop;
    
    std::vector<Crop> CropApi::Get() const
    {
        std::vector<Core::Crop> coreCrops{_core.Get()};

        std::vector<Crop> crops{};
        crops.reserve(coreCrops.size());
        for(auto crop : coreCrops) 
        { 
            crops.push_back(std::move(crop.ToApi()));
        }

        return crops;
    } 
    Crop CropApi::Get(const int id) const
    {
        Core::Crop coreCrop{_core.Get(id)};
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