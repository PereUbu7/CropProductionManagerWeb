#include "CropProductionManager/Infrastructure/cropRepository.h"
#include "CropProductionManager/Core/iCore.h"
#include "CropProductionManager/Core/cropCore.h"
#include "CropProductionManager/InternalModel/Infrastructure/crop.h"

#include <iostream>
#include <ranges>

#include <vector>
#include <functional>

namespace CropProductionManager
{
    class Program
    {
        using InfraCrop = CropProductionManager::InternalModel::Infrastructure::Crop;
        using CoreCrop = CropProductionManager::InternalModel::Core::Crop;
    
    private:
        CropProductionManager::Core::ICore<CoreCrop, InfraCrop>& _core;

    public:
        Program(CropProductionManager::Core::ICore<CoreCrop, InfraCrop>& core) :
            _core{core}
        {}

        void Run()
        {
            CoreCrop c1{0, "Broccoli", "Gnöuda", 1};
            CoreCrop c2{0, "Broccoli", "Gnöuda", 2};
            CoreCrop c3{0, "Vaxböna", "Groucha", 1};
            _core.Post(c1);
            _core.Post(c2);
            _core.Post(c3);

            CoreCrop c4{2, "Vaxböna", "Grouchas", 1};
            _core.Put(c4);

            _core.Remove(1);

            for(auto crop : _core.Get())
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
    CropProductionManager::Program program{core};

    program.Run();

    return EXIT_SUCCESS;
}