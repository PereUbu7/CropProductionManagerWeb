export module CropProductionManager.Infrastructure.CropRepository;

import CropProductionManager.InternalModel.Infrastructure.Crop;
import CropProductionManager.Infrastructure.IRepository;
import <algorithm>;
import <vector>;


export namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    class CropRepository : public CropProductionManager::Infrastructure::IRepository<Crop>
    {
    private:
        std::vector<Crop> crops;
    public:
        // GET
        std::vector<Crop> Get() const;
        // POST
        void Post(Crop crop);
        // PUT
        void Put(Crop crop);
        // DELETE
        void Remove(int id);

        CropRepository();
    };
}

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    CropRepository::CropRepository() {}

    // GET
    std::vector<Crop> CropRepository::Get() const
    {
        return crops;
    }
    // POST
    void CropRepository::Post(Crop crop) 
    {
        crops.push_back(crop);
    }
    // PUT
    void CropRepository::Put(Crop crop) 
    {
        auto cropToModify
        {
            std::find_if(begin(crops), end(crops), [crop](Crop c){ return c.id == crop.id; })
        };

        if(cropToModify != end(crops))
        {
            (*cropToModify).Update(crop);
        }
    }
    // DELETE
    void CropRepository::Remove(int id) 
    { 
        auto it = std::find_if(begin(crops), end(crops), [id](Crop crop){ return crop.id == id; });

        if(it != end(crops))
        {
            crops.erase(it);
        }
    }
}