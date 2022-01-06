import CropProductionManager.Infrastructure.RepositoryFake;
import CropProductionManager.Infrastructure.CropRepository;
import CropProductionManager.Core.CropCore;
import CropProductionManager.Api.CropApi;

import CropProductionManager.InternalModel.Infrastructure.Crop;

import CropProductionManager.Serializer.serializer;

import <iostream>;

namespace CropProductionManager
{
    class Program
    {
        using ApiCrop = CropProductionManager::ModelApi::Crop;
    
    private:
        CropProductionManager::Api::CropApi& _api;
        CropProductionManager::Serializer::Serializer<ApiCrop>& _serializer;

    public:
        Program(CropProductionManager::Api::CropApi& api,
            CropProductionManager::Serializer::Serializer<ApiCrop>& serializer) :
            _api{api},
            _serializer{serializer}
        {}

        void Run()
        {
            ApiCrop c1{0, "Broccoli", "Gnöuda", 1};
            ApiCrop c2{0, "Broccoli", "Gnöuda", 2};
            ApiCrop c3{0, "Vaxböna", "Groucha", 1};
            _api.Post(c1);
            _api.Post(c2);
            _api.Post(c3);

            ApiCrop c4{2, "Vaxböna", "Grouchas", 1};
            _api.Put(c4);

            _api.Remove(1);

            const auto j = nlohmann::json::parse("{\"name\":\"Majs\",\"variety\":\"Socker\",\"batch\":3}");
            ApiCrop c5{_serializer.Deserialize(j)};
            _api.Post(c5);

            for(auto crop : _api.Get())
            {
                std::cout << _serializer.Serialize(crop) << '\t';
                std::cout << "Id:" << crop.id
                        << "\tName:" << crop.name
                        << "\tVariety:" << crop.variety
                        << "\tBatch:" << crop.batch
                        << std::endl;
            }
        }

    };
}

int main()
{
    namespace Infrastructure = CropProductionManager::InternalModel::Infrastructure;

    CropProductionManager::Serializer::Serializer<CropProductionManager::ModelApi::Crop> serializer{};

    CropProductionManager::Infrastructure::RepositoryFake<Infrastructure::Crop> repository{};
    CropProductionManager::Core::CropCore core{repository};
    CropProductionManager::Api::CropApi api{core};
    CropProductionManager::Program program{api, serializer};

    program.Run();

    return EXIT_SUCCESS;
}