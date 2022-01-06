export module CropProductionManager.Api.CropApi;

import CropProductionManager.InternalModel.Core.Crop;
import CropProductionManager.InternalModel.Infrastructure.Crop;
import CropProductionManager.Api.IApi;
import CropProductionManager.Core.ICore;

import <vector>;

export namespace CropProductionManager::Api
{
    using Crop = CropProductionManager::ModelApi::Crop;
    namespace Core = CropProductionManager::InternalModel::Core;

    class CropApi : public CropProductionManager::Api::IApi<Crop, Core::Crop>
    {
    protected:
        CropProductionManager::Core::ICore<Core::Crop, CropProductionManager::InternalModel::Infrastructure::Crop>& _core;

    public:
        // GET
        std::vector<Crop> Get() const;
        // POST
        Crop Post(const Crop& crop);
        // PUT
        void Put(const Crop& crop);
        // DELETE
        void Remove(int id);

        CropApi(CropProductionManager::Core::ICore<Core::Crop, CropProductionManager::InternalModel::Infrastructure::Crop>& core):
            _core{core} {}
    };
}

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
    Crop CropApi::Post(const Crop& crop)
    {
        auto coreCrop{ Core::Crop::FromApi(crop) };
        _core.Post(coreCrop);

        return crop;
    }
    void CropApi::Put(const Crop& crop)
    {
        auto coreCrop{ Core::Crop::FromApi(crop) };
        _core.Put(coreCrop);
    }
    void CropApi::Remove(int id)
    {
        _core.Remove(id);
    }
}