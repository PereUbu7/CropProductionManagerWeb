#include "CropProductionManager/Infrastructure/cropRepository.h"
#include "CropProductionManager/Core/cropCore.h"
#include "CropProductionManager/Api/cropApi.h"

#include <iostream>
#include <ranges>

#include <vector>
#include <functional>

namespace CropProductionManager
{
    class Program
    {
        using ApiCrop = CropProductionManager::InternalModel::Api::Crop;
    
    private:
        CropProductionManager::Api::CropApi& _api;

    public:
        Program(CropProductionManager::Api::CropApi& api) :
            _api{api}
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

            for(auto crop : _api.Get())
            {
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
    CropProductionManager::Infrastructure::CropRepository repository{};
    CropProductionManager::Core::CropCore core{repository};
    CropProductionManager::Api::CropApi api{core};
    CropProductionManager::Program program{api};

    program.Run();

    return EXIT_SUCCESS;
}