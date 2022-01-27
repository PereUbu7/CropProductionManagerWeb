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
            crops.push_back(Crop::FromInfrastructure(crop));
        }

        return crops;
    } 
    Crop CropCore::Get(const int id) const
    {
        Infrastructure::Crop infraCrop{_repository.Get(id)};
        return Crop::FromInfrastructure(infraCrop);
    }
    Crop CropCore::Post(Crop& crop)
    {
        crop.id = CropCore::createId();

        _repository.Post(crop.ToInfrastructure());

        return crop;
    }
    void CropCore::Put(Crop& crop, const std::string &id)
    {        
        try
        {
            // If id is supplied by argument, overwrite
            int idArg = std::stoi(id);
            crop.id = idArg;
        }
        catch(const std::invalid_argument& e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
        std::cout << "In Core Put, id is: " << crop.id << '\n';
        
        _repository.Put(crop.ToInfrastructure());
    }
    void CropCore::Remove(const std::string &id)
    {
        try
        {
            int idArg = std::stoi(id);
            std::cout << "In Core Remove, id is: " << id << '\n';
            _repository.Remove(0);
        }
        // TODO: Return something useful to web server if this happens
        catch(const std::invalid_argument& e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
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