export module CropProductionManager.InternalModel.Infrastructure.Crop;

import <string>;

export namespace CropProductionManager::InternalModel::Infrastructure
{
    using String = std::string;

    class Crop
    {
    public:
        int id;
        String name;
        String variety;
        int batch;
        void Update(Crop crop);
    };
}

void CropProductionManager::InternalModel::Infrastructure::Crop::Update(Crop crop)
{
    id = crop.id;
    name = crop.name;
    variety = crop.variety;
    batch = crop.batch;
}