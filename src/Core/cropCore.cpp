export module CropProductionManager.Core.CropCore;

import CropProductionManager.InternalModel.Core.Crop;
import CropProductionManager.InternalModel.Infrastructure.Crop;
import CropProductionManager.ModelApi.Crop;
import CropProductionManager.Core.ICore;
import CropProductionManager.Infrastructure.IRepository;

import <iostream>;
import <algorithm>;

export namespace CropProductionManager::Core
{
    using Crop = CropProductionManager::InternalModel::Core::Crop;
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;
    using IRepository = CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop>;

    class CropCore : public CropProductionManager::Core::ICore<Crop, Infrastructure::Crop>
    {
    protected:
        CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop>& _repository;
        std::vector<Crop> crops;

        static int createId();
    public:
        // GET
        std::vector<Crop> Get() const;
        // POST
        Crop Post(Crop& crop);
        // PUT
        void Put(Crop& crop);
        // DELETE
        void Remove(int id);

        Infrastructure::Crop ToInfrastructure(Crop& c) const;

        CropCore(CropProductionManager::Infrastructure::IRepository<Infrastructure::Crop>& repository):
            _repository{repository} {}
    };
}

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