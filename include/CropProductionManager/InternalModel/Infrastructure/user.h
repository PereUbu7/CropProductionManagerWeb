#pragma once
#include <string>
#include <vector>

namespace CropProductionManager::InternalModel::Infrastructure
{
    using Plan = CropProductionManager::InternalModel::Infrastructure::Plan;
    class User
    {
    public:
        std::string name;
        std::string gui;

        std::vector<Plan> plans;
    private:
        std::string passwordHash;
    };
}