#pragma once

#include "CropProductionManager/ModelApi/crop.h"
#include "json.hpp"

namespace CropProductionManager::ModelApi
{
    void to_json(nlohmann::json& j, const Crop& crop);
    void from_json(const nlohmann::json& j, Crop& crop);
}