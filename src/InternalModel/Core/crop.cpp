export module CropProductionManager.InternalModel.Core.Crop;

export import CropProductionManager.ModelApi.Crop;
export import CropProductionManager.InternalModel.Infrastructure.Crop;

import <string>;

export namespace CropProductionManager::InternalModel::Core
{
    using String = std::string;
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;
    namespace Api = CropProductionManager::ModelApi;

    class Crop
    {
    public:
        int id;
        String name;
        String variety;
        int batch;

        Crop(int id, String name, String variety, int batch) :
            id{id},
            name{name},
            variety{variety},
            batch{batch} {}

        Crop(const Infrastructure::Crop& infra) :
            id{infra.id},
            name{infra.name},
            variety{infra.variety},
            batch{infra.batch}
        { 
        }

        Crop(const Api::Crop& infra) :
            id{infra.id},
            name{infra.name},
            variety{infra.variety},
            batch{infra.batch}
        { 
        }

        void Update(Crop crop);
        Infrastructure::Crop ToInfrastructure();
        Api::Crop ToApi();
        Crop static FromInfrastructure(Infrastructure::Crop crop);
        Crop static FromApi(Api::Crop crop);
    };
}

namespace CropProductionManager::InternalModel::Core
{
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;
    namespace Api = CropProductionManager::ModelApi;

    void Crop::Update(const Crop crop)
    {
        id = crop.id;
        name = crop.name;
        variety = crop.variety;
        batch = crop.batch;
    }

    Infrastructure::Crop Crop::ToInfrastructure()
    {
        return Infrastructure::Crop{ id, name, variety, batch };
    }

    Crop Crop::FromInfrastructure(const Infrastructure::Crop crop)
    {
        return Crop{ crop };
    }

    Api::Crop Crop::ToApi()
    {
        return Api::Crop{ id, name, variety, batch };
    }

    Crop Crop::FromApi(const Api::Crop crop)
    {
        return Crop{ crop };
    }
}