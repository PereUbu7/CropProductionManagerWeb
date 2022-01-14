#include "CropProductionManager/Server/webServer.h"

namespace CropProductionManager
{
    class Program
    {
    public:
        void Run()
        {
            Server::WebServer service{"0.0.0.0", 1984};
            service.Setup();
        }
    };
}

int main()
{
    CropProductionManager::Program program{};

    program.Run();

    return EXIT_SUCCESS;
}