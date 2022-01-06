export module CropProductionManager.ModelApi.Crop;

import <string>;

export namespace CropProductionManager::ModelApi
{
    using String = std::string;

    class Crop
    {
    public:
        Crop() = default;
        Crop(int id, String name, String variety, int batch) :
            id{id}, 
            name{name}, 
            variety{variety}, 
            batch{batch} {}
        int id;
        String name;
        String variety;
        int batch;
    };
}