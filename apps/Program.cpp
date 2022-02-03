#include "CropProductionManager/Server/webServer.h"
#include "CropProductionManager/iConfiguration.hpp"
#include "CropProductionManager/inputParser.hpp"

#include <libgen.h>
#include <unistd.h>
#include <linux/limits.h>

#include <iostream>

namespace CropProductionManager
{
    class Program
    {
    public:
    // TODO: Move ILogger instantiation here
        void Run(const std::string enviroment)
        {
            auto currentPath = getCurrentPath();
            auto pathToDefaultConfig = std::string{currentPath + "/appconfig.json"};
            auto confStream = std::ifstream(pathToDefaultConfig);

            if(confStream.fail()) { throw std::ifstream::failure("Couldn't find config file: " + pathToDefaultConfig); }

            auto config = IConfiguration(confStream);
            
            /* Merge config with optional environment config */
            if(!enviroment.empty())
            {
                auto pathToEnvironmentConfig = std::string{currentPath + "/appconfig." + enviroment + ".json"};
                auto envStream = std::ifstream(pathToEnvironmentConfig);

                if(envStream.fail()) { throw std::ifstream::failure("Couldn't find env config file: " + pathToEnvironmentConfig); }

                config.MergeConfig(IConfiguration(envStream));
            }
        
            Server::WebServer service{config["service"]};
            service.Setup(config);
        }
    private:
        std::string getCurrentPath()
        {
            char result[200] = {};
            ssize_t count = readlink("/proc/self/exe", result, 200);
            if(count != -1)
            {
                return std::string{dirname(result)};
            }
            return std::string{};
        }
    };
}

namespace Help
{
    std::string getHelpText();
}

int main(int argc, char **argv)
{
    InputParser input(argc, argv);
    if(input.cmdOptionExists("-h")){
        std::cout << Help::getHelpText();
        return EXIT_SUCCESS;
    }

    const auto& environment = std::string{input.getCmdOption("-e")};

    CropProductionManager::Program program{};
    try
    {
        program.Run(environment);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return EXIT_SUCCESS;
}

std::string Help::getHelpText()
{
    return R"(  --  Crop Production Manager Web Api Service  --  

Arguments:
    -h  Prints this help and exits.
    -e  Sets enviroment name and loads it's config file with name 'appconfig.[environment].json'.
)";
}